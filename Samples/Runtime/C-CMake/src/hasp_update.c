/****************************************************************************
*
* Demo program for the Sentinel LDK update process
*
*
* Copyright (C) 2021 Thales Group. All rights reserved.
*
*
* Sentinel LDK DEMOMA key required
*
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hasp_api.h"

#include "hasp_vcode.h"   /* this header file holds the Sentinel DEMOMA vendor code */

static void usage(void)
{
    printf("This is a simple demo program for the Sentinel Update "
           "and Transfer functions\n");
    printf("Copyright (C) Thales Group. All rights reserved.\n\n");

    printf("Usage:      hasp_update <option> [filename]\n");
    printf("Options:    u: updates a Sentinel protection key/attaches a "
                          "detached license\n");
    printf("            i: retrieves Sentinel protection key information\n");
    printf("            d: detaches a license from a Sentinel SL-AdminMode/SL-Legacy key (Not supported for SL-UserMode)\n");
    printf("            r: rehost a license from a Sentinel SL-AdminMode/SL-UserMode key\n");
    printf("            f: retrieves fingerprint information\n");
    printf("Filename:   Path to the V2C/H2R file (in case of 'u'pdate/attach), "
                        "OR\n"
           "            Path to the C2V file; Optional: uses \"stdout\" if\n"
           "            file name is not specified (in case of 'i'nfo)\n");
}

static unsigned char *read_file(FILE *fp)
{
    unsigned char *buffer;
    size_t   buffer_length, file_size;

    /* get the file size */
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = (unsigned char *)malloc(file_size + 1);
    if (!buffer)
    {
        printf("out of memory\n");
        return 0;
    }

    buffer_length = fread(buffer, sizeof(char), file_size, fp);
    if (buffer_length <= 0)
    {
        printf("failed to read v2c data\n");
        free(buffer);
        return 0;
    }

    /* terminate update information */
    buffer[buffer_length] = 0;

    return buffer;
}

int main(int argc, char **argv)
{
    hasp_status_t  status;
    FILE          *fp;
    char          *info = 0;

    if ((argc < 2) || (argc > 3))
    {
        usage();
        return -1;
    }

    if (strcmp(argv[1], "d") == 0)  /* use hasp_transfer to detach a license */
    {
        char          *recipient;
        char          *h2r = 0;

        /* get local recipient information */
        status = hasp_get_info(
                         "<haspscope>"
                         "  <license_manager hostname=\"localhost\" />"
                         "</haspscope>",
                         HASP_RECIPIENT,
                         (hasp_vendor_code_t)vendor_code, &info);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_SCOPE_RESULTS_EMPTY:
                    printf("Unable to locate a Feature matching the scope\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_INV_VCODE:
                    printf("Invalid Vendor Code\n");
                    break;
                case HASP_UNKNOWN_VCODE:
                    printf("Unknown Vendor Code\n");
                    break;
                case HASP_INVALID_PARAMETER:
                    printf("Invalid parameter (scope/format too long?)\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_TOO_MANY_KEYS:
                    printf("Too many keys connected\n");
                    break;
                case HASP_INV_FORMAT:
                    printf("Unrecognized format string\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;
                default:
                    printf("hasp_get_info failed with status %u\n", status);
                    break;
            }

            return -1;
        }

        recipient = info;

        /* detach license for local recipient (duration 120 seconds.) 
         * Note: Please change the product ID to which you want to
         *       operate.
         */
        status = hasp_transfer(
                        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                        "<detach><duration>120</duration></detach>",
                        "<haspscope><product id=\"123\" /></haspscope>",
                        (hasp_vendor_code_t)vendor_code,
                        recipient,
                        &h2r);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_INV_DETACH_ACTION:
                    printf("Invalid XML detach_action parameter\n");
                    break;
                case HASP_INV_RECIPIENT:
                    printf("Invalid XML recipient parameter\n");
                    break;
                case HASP_TOO_MANY_PRODUCTS:
                    printf("Scope for hasp_detach does not select a unique Parameter\n");
                    break;
                case HASP_ACCESS_DENIED:
                    printf("Request not possible because of ACC restrictions\n");
                    break;
                case HASP_INV_PRODUCT:
                    printf("Invalid Product information\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;
                default:
                    printf("hasp_transfer failed with status %u\n", status);
                    break;
            }

            return -1;
        }

        /* write h2r to file or stdout */
        if (argc == 3)
        {
            fp = fopen(argv[2], "wt");
            if (!fp)
            {
                printf("could not open file %s\n", argv[2]);
                return -1;
            }
        }
        else
        {
            fp = stdout;
        }

        fprintf(fp, "%s\n", h2r);

        hasp_free(info);
        hasp_free(h2r);

        if (fp != stdout)
            fclose(fp);

        return 0;
    }
    else if (strcmp(argv[1], "r") == 0)   /* use hasp_transfer to rehost a license */
    {
        char          *info = 0;
        char          *recipient;
        char          *v2c = 0;

        /* get local recipient information */
        status = hasp_get_info(
                            "<haspscope>"
                            "  <license_manager hostname=\"localhost\" />"
                            "</haspscope>",
                            HASP_RECIPIENT,
                            (hasp_vendor_code_t)vendor_code, &info);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_SCOPE_RESULTS_EMPTY:
                    printf("Unable to locate a Feature matching the scope\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_INV_VCODE:
                    printf("Invalid Vendor Code\n");
                    break;
                case HASP_UNKNOWN_VCODE:
                    printf("Unknown Vendor Code\n");
                    break;
                case HASP_INVALID_PARAMETER:
                    printf("Invalid parameter (scope/format too long?)\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_TOO_MANY_KEYS:
                    printf("Too many keys connected\n");
                    break;
                case HASP_INV_FORMAT:
                    printf("Unrecognized format string\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;

                default:
                    printf("hasp_get_info failed with status %u\n", status);
                    break;
            }

            return -1;
        }
        else
        {
            printf("\n hasp_get_info function is executed successfully\n");
        }

        recipient = info;

        /* hasp_transfer function being used. This function is used to rehost
         * the v2c form one machine to another machine.
         * For this we will use the recipient information generated from
         * hasp_get_info function.
         * Note: please change the value '123456789' to your key ID that you 
         *       want to operate.
         */

        status = hasp_transfer(
                        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                        "<rehost><hasp id=\"123456789\"/></rehost>",
                        "<haspscope><hasp id=\"123456789\"/></haspscope>",
                        (hasp_vendor_code_t)vendor_code,
                        recipient,
                        &v2c);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {

                case HASP_INV_RECIPIENT:
                    printf("Invalid XML recipient parameter\n");
                    break;
                case HASP_ACCESS_DENIED:
                    printf("Request not possible because of ACC restrictions\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;
                case HASP_REHOST_NOT_ALLOWED:
                     printf("Rehost not allowed\n");
                     break;
                case HASP_TOO_MANY_KEYS:
                     printf("Scope for hasp_transfer does not specify a unique Parameter\n");
                     break;
                case HASP_OLD_LM:
                     printf("License Manager is too old for SL-AdminMode/SL-UserMode licenses\n");
                     break;
                case HASP_HASP_INACTIVE:
                     printf("Container is inactive\n");
                     break;
                case HASP_CLONE_DETECTED:
                     printf("Cloned secure storage area detected\n");
                     break;
                case HASP_REQ_NOT_SUPP:
                     printf("Rehost request not supported for SL-AdminMode/SL-UserMode licenses\n");
                     break;
                default:
                    printf("hasp_transfer failed with status %u\n", status);
                    break;
            }
			
            return -1;
        }
        else
        {
            printf("\n hasp_transfer command executed successfully\n");
        }

        /* write v2c to stdout */
        if (argc == 3)
        {
            fp = fopen(argv[2], "wt");
            if (!fp)
            {
                printf("could not open file %s\n", argv[2]);

                return -1;
            }
        }
        else
        {
            fp = stdout;
        }

        fprintf(fp, "%s\n", v2c);

        hasp_free(info);
        hasp_free(v2c);

        if (fp != stdout)
            fclose(fp);

        return 0;
    }
    else if (strcmp(argv[1], "u") == 0) /* use hasp_update to install a v2c/h2r */
    {
        unsigned char *buffer = 0;

        /* read update from file */
        if (argc != 3)
        {
            usage();
            return -1;
        }

        fp = fopen(argv[2], "r");
        if (!fp)
        {
            printf("could not open file %s\n", argv[2]);
            free(buffer);
            return -1;
        }

        /* read the file; this function allocates 'buffer' with 'malloc' */
        buffer = read_file(fp);
        if (!buffer){
            return -1;
        }

        fclose(fp);

        status = hasp_update((void*)buffer, &info);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_INV_UPDATE_DATA:
                    printf("Update data is invalid\n");
                    break;
                case HASP_INV_UPDATE_NOTSUPP:
                    printf("Update request is not supported\n");
                    break;
                case HASP_INV_UPDATE_CNTR:
                    printf("Update counter not set correctly\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_NO_ACK_SPACE:
                    printf("Acknowledge pointer is NULL\n");
                    break;
                case HASP_UNKNOWN_ALG:
                    printf("Unknown v2c algorithm\n");
                    break;
                case HASP_INV_SIG:
                    printf("v2c signature broken\n");
                    break;
                case HASP_TOO_MANY_KEYS:
                    printf("Too many keys connected\n");
                    break;
                case HASP_HARDWARE_MODIFIED:
                    printf("v2c data and SL key data mismatch\n");
                    break;
                case HASP_UPDATE_TOO_OLD:
                    printf("v2c was already installed\n");
                    break;
                case HASP_UPDATE_TOO_NEW:
                    printf("v2c is too new (update counter too high)\n");
                    break;
                case HASP_LICENSE_REHOSTED:
                    printf("License is rehosted to other machine\n");
                    break;
                case HASP_REHOST_ALREADY_APPLIED:
                    printf("Old rehost license try to apply\n");
                    break;
                case HASP_NO_VLIB :
                    printf("Unable to find Vendor Library\n");
                    break;
                case HASP_INV_VLIB :
                    printf("Unable to load Vendor Library\n");
                    break;
                case HASP_OLD_VLIB :
                    printf("Vendor Library version too old\n");
                    break;
                case HASP_OLD_LM :
                    printf("License Manager version too old\n");
                    break;
                case HASP_MISSING_LM :
                    printf("License Manager is required for this operation\n");
                    break;
                case HASP_STORAGE_CORRUPT :
                    printf("Corrupt data exists in secure storage area\n");
                    break;
                case HASP_VM_DETECTED :
                    printf("Running on a virtual machine\n");
                    break;
                case HASP_RECIPIENT_OLD_LM :
                    printf("Recipient of the requested operation is older than expected\n");
                    break;
                case HASP_SECURE_STORE_ID_MISMATCH :
                    printf("Secure storage ID mismatch\n");
                    break;
                case HASP_CLONE_DETECTED :
                    printf("Cloned secure storage area detected\n");
                    break;
                case HASP_INCOMPATIBLE_PLATFORM :
                    printf("Attempting to perform an operation not compatible with target platform\n");
                    break;
                default:
                    printf("hasp_update failed with status %u\n", status);
                    break;
            }
            free(buffer);
            return -1;
        }

        /* print acknowledge data to stdout */
        if (info)
            printf("%s\n", info);
        hasp_free(info);

        free(buffer);
        return 0;
    }
    else if (strcmp(argv[1], "i") == 0)   /* use hasp_get_info to retrieve a c2v */
    {
        /* restrict the c2v to local Sentinel keys */
        status = hasp_get_info(
                        "<haspscope>"
                        "  <license_manager hostname=\"localhost\" />"
                        "</haspscope>",
                        HASP_UPDATEINFO,
                        (hasp_vendor_code_t)vendor_code, &info);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_SCOPE_RESULTS_EMPTY:
                    printf("Unable to locate a Feature matching the scope\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_INV_VCODE:
                    printf("Invalid Vendor Code\n");
                    break;
                case HASP_UNKNOWN_VCODE:
                    printf("Unknown Vendor Code\n");
                    break;
                case HASP_INVALID_PARAMETER:
                    printf("Invalid parameter (scope/format too long?)\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_TOO_MANY_KEYS:
                    printf("Too many keys connected\n");
                    break;
                case HASP_INV_FORMAT:
                    printf("Unrecognized format string\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;
                default:
                    printf("hasp_get_info failed with status %u\n", status);
                    break;
            }
            return -1;
        }

        /* write info to file or stdout */
        if (argc == 3)
        {
            fp = fopen(argv[2], "wt");
            if (!fp)
            {
                printf("Could not open file %s\n", argv[2]);

                return -1;
            }
        }
        else
        {
            fp = stdout;
        }

        fprintf(fp, "%s\n", info);
        hasp_free(info);

        if (fp != stdout)
            fclose(fp);

        return 0;
    }
    else if (strcmp(argv[1], "f") == 0)   /* use hasp_get_info to retrieve a c2v */
    {
        /* restrict the c2v to local Sentinel keys */
        status = hasp_get_info(
                        "<haspscope>"
                        "  <license_manager hostname=\"localhost\" />"
                        "</haspscope>",
                        HASP_FINGERPRINT,
                        (hasp_vendor_code_t)vendor_code, &info);

        /* check if operation was successful */
        if (status != HASP_STATUS_OK)
        {
            switch (status)
            {
                case HASP_SCOPE_RESULTS_EMPTY:
                    printf("Unable to locate a Feature matching the scope\n");
                    break;
                case HASP_INSUF_MEM:
                    printf("Out of memory\n");
                    break;
                case HASP_INV_VCODE:
                    printf("Invalid Vendor Code\n");
                    break;
                case HASP_UNKNOWN_VCODE:
                    printf("Unknown Vendor Code\n");
                    break;
                case HASP_INVALID_PARAMETER:
                    printf("Invalid parameter (scope/format too long?)\n");
                    break;
                case HASP_DEVICE_ERR:
                    printf("Input/Output error\n");
                    break;
                case HASP_LOCAL_COMM_ERR:
                    printf("Communication error\n");
                    break;
                case HASP_REMOTE_COMM_ERR:
                    printf("Remote communication error\n");
                    break;
                case HASP_TOO_MANY_KEYS:
                    printf("Too many keys connected\n");
                    break;
                case HASP_INV_FORMAT:
                    printf("Unrecognized format string\n");
                    break;
                case HASP_INV_SCOPE:
                    printf("Unrecognized scope string\n");
                    break;
                default:
                    printf("hasp_get_info failed with status %u\n", status);
                    break;
            }
            return -1;
        }

        /* write info to file or stdout */
        if (argc == 3)
        {
            fp = fopen(argv[2], "wt");
            if (!fp)
            {
                printf("Could not open file %s\n", argv[2]);

                return -1;
            }
        }
        else
        {
            fp = stdout;
        }

        fprintf(fp, "%s\n", info);
        hasp_free(info);

        if (fp != stdout)
            fclose(fp);

        return 0;
    }
    else
    {
        usage();
    }

    return -1;

} /* main */
