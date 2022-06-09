/****************************************************************************
*
* Demo program for Sentinel LDK licensing services
*
*
* Copyright (C) 2021 Thales Group. All rights reserved.
*
*
* Sentinel LDK DEMOMA key with features 1 and 42 required
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hasp_api.h"
#include "hasp_vcode.h"       /* contains HASP DEMOMA vendor code */

#define CUSTOM_FEATURE 42

unsigned char data[] =
{
    0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74, 0x72,
    0x69, 0x6E, 0x67, 0x20, 0x31, 0x32, 0x33, 0x34
};

hasp_size_t datalen = sizeof(data);

#define DEMO_MEMBUFFER_SIZE   128

unsigned char membuffer[DEMO_MEMBUFFER_SIZE];

/****************************************************************************
 * helper function: dumps a given block of data, in hex
 */

#define DUMP_COLS_PER_ROW     16
#define DUMP_SPACE_COLS        8

void dump_hex(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump character in hex */
        printf("%02X ", data[i]);
    }
} /* dump_hex */

/****************************************************************************
 * helper function: dumps a given block of data, in ascii
 */

void dump_ascii(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i;

    for (i = 0; i < datalen; i++)
    {
        if (((i % DUMP_SPACE_COLS) == 0) && (i != 0))
            printf(" ");

        if ((i % DUMP_COLS_PER_ROW) == 0)
        {
            if (i != 0)
                printf("\n");

            if (margin != NULL)
                printf("%s", margin);
        }

        /* dump printable character in ascii */
        printf("%c", ((data[i] > 31) && (data[i] < 128)) ? data[i] : '.');
    }
} /* dump_ascii */

/****************************************************************************
 * helper function: dumps a given block of data, in hex and ascii
 */

void dump(unsigned char *data, unsigned int datalen, char *margin)
{
    unsigned int i, icols;

    for (i = 0; i < datalen; )
    {
        icols = datalen - i;

        if (icols > DUMP_COLS_PER_ROW)
            icols = DUMP_COLS_PER_ROW;

        dump_hex(&data[i], icols, margin);
        dump_ascii(&data[i], icols, "  ");
        printf("\n");

        i += icols;
    }
} /* dump */

/***************************************************************************/

void wait4key(char *hint)
{
    fflush(stdout);
    fflush(stdin);
    printf("\npress ENTER %s\n", hint);
    while (getchar() == EOF)
        ;
}

/***************************************************************************/

hasp_status_t encrypt_decrypt(hasp_handle_t handle)
{
    hasp_status_t status;

    /*
     * hasp_encrypt
     *   encrypts a block of data using the HASP key
     *   (minimum buffer size is 16 bytes)
     */

    printf("\nencrypting a data buffer:\n");
    dump(data, datalen, "    ");

    status = hasp_encrypt(handle, data, datalen);
    switch (status)
    {
        case HASP_STATUS_OK:
            printf("encryption ok:\n");
            dump(data, datalen, "    ");
            break;

        case HASP_INV_HND:
            printf("handle not active\n");
            break;

        case HASP_TOO_SHORT:
            printf("data length too short\n");
            break;

        case HASP_ENC_NOT_SUPP:
            printf("attached key does not support AES encryption\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("key/license container not available\n");
            break;

        default:
            printf("encryption failed\n");
    }
    if (status) return status;

    /*
     * hasp_decrypt
     *   decrypts a block of data using the HASP key
     *   (minimum buffer size is 16 bytes)
     */

    status = hasp_decrypt(handle, data, datalen);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("decryption ok:\n");
            dump(data, datalen, "    ");
            break;

        case HASP_INV_HND:
            printf("handle not active\n");
            break;

        case HASP_TOO_SHORT:
            printf("data length too short\n");
            break;

        case HASP_ENC_NOT_SUPP:
            printf("attached key does not support AES encryption\n");
            break;

        default:
            printf("decryption failed\n");
    }
    if (status) return status;

    return HASP_STATUS_OK;
} /* encrypt_decrypt */

/***************************************************************************/

int main(void)
{
    hasp_status_t   status;
    hasp_handle_t   handle, handle2;
    hasp_time_t     time;
    unsigned int    day, month, year, hour, minute, second;
    const char     *scope;
    const char     *view;
    char           *info;
    hasp_size_t     fsize;
    unsigned int    i;

    printf("A simple demo program for the Sentinel LDK licensing functions\n");
    printf("Copyright (C) Thales Group. All rights reserved.\n\n");

    /************************************************************************
     * hasp_login
     *   establishes a context for HASP services
     */

    printf("login to default feature         : ");

    /* login to default feature (0)                 */
    /* this default feature is available on any key */
    /* search for local and remote HASP key         */

    status = hasp_login(HASP_DEFAULT_FID,
                    (hasp_vendor_code_t)vendor_code,
                    &handle);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_FEATURE_NOT_FOUND:
            printf("login to default feature failed\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("no sentinel key with vendor code DEMOMA found\n");
            break;

        case HASP_OLD_DRIVER:
            printf("outdated driver version installed\n");
            break;

        case HASP_NO_DRIVER:
            printf("sentinel driver not installed\n");
            break;

        case HASP_INV_VCODE:
            printf("invalid vendor code\n");
            break;

        default:
            printf("login to default feature failed with status %d\n", status);
    }
    if (status) {
        exit(-1);
    }

    /************************************************************************
     * hasp_get_sessioninfo
     *   retrieve Sentinel key attributes
     *
     * Please note: In case of performing an activation we recommend to use
     *              hasp_get_info() instead of hasp_get_sessioninfo(), as
     *              demonstrated in the activation sample. hasp_get_info()
     *              can be called without performing a login.
     */

    printf("\nget session info                 : ");

    status = hasp_get_sessioninfo(handle,
                    HASP_KEYINFO,
                    &info);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK, Sentinel key attributes retrieved\n\n"
                    "Key info:\n===============\n%s===============\n", info);
            hasp_free(info);
            break;

        case HASP_INV_HND:
            printf("handle not active\n");
            break;

        case HASP_INV_FORMAT:
            printf("unrecognized format\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("key/license container not available\n");
            break;

        default:
            printf("hasp_get_sessioninfo failed\n");
    }
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }


    /***********************************************************************/

    wait4key("to continue");

    /************************************************************************
     * hasp_get_size
     *   retrieve the memory size of the HASP key
	 *   you can also retrieve dynamic memory file size, 
	 *   only need to pass the dynamic memory file id which 
	 *	 created in token
     */

    printf("\nretrieving the key's memory size : ");

    status = hasp_get_size(handle,
                    HASP_FILEID_RW,
                    &fsize);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("sentinel memory size is %d bytes\n", fsize);
            break;

        case HASP_INV_HND:
            printf("handle not active\n");
            break;

        case HASP_INV_FILEID:
            printf("invalid file id\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("key/license container not available\n");
            break;

        default:
            printf("could not retrieve memory size\n");
    }
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }

    if (fsize != 0)       /* skip memory access if no memory available */
    {

        /********************************************************************
         * hasp_read
         *   read from HASP memory
		 *   you can also read dynamic memory file, 
		 *   only need to pass the dynamic memory file id which 
		 *	 created in token
         */

        /* limit memory size to be used in this demo program */

        if (fsize > DEMO_MEMBUFFER_SIZE)
            fsize = DEMO_MEMBUFFER_SIZE;

        printf("\nreading %4d bytes from memory   : ", fsize);

        status = hasp_read(handle,
                       HASP_FILEID_RW,    /* read/write file ID */
                       0,                 /* offset */
                       fsize,             /* length */
                       &membuffer[0]);    /* file data */

        switch (status)
        {
            case HASP_STATUS_OK:
                printf("OK\n");
                dump(membuffer, fsize, "    ");
                break;

            case HASP_INV_HND:
                printf("handle not active\n");
                break;

            case HASP_INV_FILEID:
                printf("invalid file id\n");
                break;

            case HASP_MEM_RANGE:
                printf("beyond memory range of attached sentinel key\n");
                break;

            case HASP_CONTAINER_NOT_FOUND:
                printf("key/license container not available\n");
                break;

            default:
                printf("read memory failed\n");
        }
        if (status) {
            hasp_logout(handle);
            exit(-1);
        }

        /********************************************************************
         * hasp_write
         *   write to HASP memory
		 *   you can also write dynamic memory file, 
		 *   only need to pass the dynamic memory file id which 
		 *	 created in token
         */

        printf("\nincrementing every byte in memory buffer\n");
        for (i = 0; i < fsize; i++)
            membuffer[i]++;

        printf("\nwriting %4d bytes to memory     : ", fsize);

        status = hasp_write(handle,
                        HASP_FILEID_RW,
                        0,                /* offset */
                        fsize,            /* length */
                        &membuffer);

        switch (status)
        {
            case HASP_STATUS_OK:
                printf("OK\n");
                break;

            case HASP_INV_HND:
                printf("handle not active\n");
                break;

            case HASP_INV_FILEID:
                printf("invalid file id\n");
                break;

            case HASP_MEM_RANGE:
                printf("beyond memory range of attached sentinel key\n");
                break;

            case HASP_CONTAINER_NOT_FOUND:
                printf("key/license container not available\n");
                break;

            default:
                printf("write memory failed\n");
        }
        if (status) {
            hasp_logout(handle);
            exit(-1);
        }

        /********************************************************************
         * hasp_read
         *   read from HASP memory
         */

        printf("\nreading %4d bytes from memory   : ", fsize);

        status = hasp_read(handle,
                        HASP_FILEID_RW,
                        0,                 /* offset */
                        fsize,             /* length */
                        &membuffer);

        switch (status)
        {
            case HASP_STATUS_OK:
                printf("OK\n");
                dump(membuffer, fsize, "    ");
                break;

            case HASP_INV_HND:
                printf("handle not active\n");
                break;

            case HASP_INV_FILEID:
                printf("invalid file id\n");
                break;

            case HASP_MEM_RANGE:
                printf("beyond memory range of attached sentinel key\n");
                break;

            case HASP_CONTAINER_NOT_FOUND:
                printf("key/license container not available\n");
                break;

            default:
                printf("read memory failed\n");
        }
        if (status) {
            hasp_logout(handle);
            exit(-1);
        }

    } /* end of memory demo */


    /***********************************************************************/
    /* hasp_encrypt and hasp_decrypt */

    status = encrypt_decrypt(handle);
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }

    /************************************************************************
     * hasp_login
     *   establishes a context for HASP services
     *   a new handle is used to log into another feature
     */

    printf("\n\nlogin to feature %4d            : ", CUSTOM_FEATURE);

    status = hasp_login(CUSTOM_FEATURE,
                    (hasp_vendor_code_t)vendor_code,
                    &handle2);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_FEATURE_NOT_FOUND:
            printf("no sentinel DEMOMA key found with feature 42\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("key not available\n");
            break;

        case HASP_OLD_DRIVER:
            printf("outdated driver version installed\n");
            break;

        case HASP_NO_DRIVER:
            printf("sentinel driver not installed\n");
            break;

        case HASP_INV_VCODE:
            printf("invalid vendor code\n");
            break;

        default:
            printf("failed with status %d\n", status);
    }
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }

    /***********************************************************************/

    printf("\nencrypt/decrypt again to see different "
            "encryption for different features:\n");

    status = encrypt_decrypt(handle2);
    if (status) {
        hasp_logout(handle);
        hasp_logout(handle2);
        exit(-1);
    }


    /************************************************************************
     * hasp_logout
     *   closes established session and releases allocated memory
     */

    printf("\nlogout from feature %4d         : ", CUSTOM_FEATURE);

    status = hasp_logout(handle2);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_INV_HND:
            printf("failed: handle not active\n");
            break;

        default:
            printf("failed with status %d\n", status);
    }
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }


    /************************************************************************
     * hasp_get_rtc
     *   read current time from HASP Time key
     */

    printf("\nreading current time and date    : ");

    status = hasp_get_rtc(handle, &time);

    switch (status)
    {
        case HASP_STATUS_OK:
            /* convert HASP time value to time and date */
            status = hasp_hasptime_to_datetime(time, &day, &month, &year,
                                         &hour, &minute, &second);

            switch (status)
            {
                case HASP_STATUS_OK:
                    printf("time: %d:%d:%d H/M/S\n                  "
                           "                 date: %d/%d/%d D/M/Y\n",
                           hour, minute, second, day, month, year);
                    break;

                case HASP_INV_TIME:
                    printf("time value outside supported range\n");
                    break;

                default:
                    printf("time conversion failed\n");
            }
            break;

        case HASP_INV_HND:
            printf("handle not active\n");
            break;

        case HASP_NO_TIME:
            printf("no sentinel Time connected\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("key/license container not available\n");
            break;

        default:
            printf("could not read time from sentinel key, status %d\n", status);
    }
    if ( (status) && (status != HASP_NO_TIME) ) {
        hasp_logout(handle);
        exit(-1);
    }

    /************************************************************************
     * hasp_logout
     *   closes established session and releases allocated memory
     */

    printf("\nlogout from default feature      : ");

    status = hasp_logout(handle);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_INV_HND:
            printf("failed: handle not active\n");
            break;

        default:
            printf("failed\n");
    }
    if (status) {
        exit(-1);
    }

    /***********************************************************************/

    wait4key("to continue");

    /************************************************************************
     * hasp_login_scope
     *   establishes a context for HASP services
     *   allows specification of several restrictions
     */

    printf("restricting the license to 'local':\n");

    scope = "<haspscope>\n"
                  " <license_manager hostname=\"localhost\" />\n"
                  "</haspscope>\n";
    printf("%s\n", scope);
    printf("\nhasp_login_scope                 : ");

    status = hasp_login_scope(HASP_DEFAULT_FID,
                    scope,
                    (hasp_vendor_code_t)vendor_code,
                    &handle);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_FEATURE_NOT_FOUND:
            printf("login to default feature failed\n");
            break;

        case HASP_CONTAINER_NOT_FOUND:
            printf("no sentinel key with vendor code DEMOMA found\n");
            break;

        case HASP_OLD_DRIVER:
            printf("outdated driver version installed\n");
            break;

        case HASP_NO_DRIVER:
            printf("sentinel driver not installed\n");
            break;

        case HASP_INV_VCODE:
            printf("invalid vendor code\n");
            break;

        case HASP_INV_SCOPE:
            printf("invalid XML scope\n");
            break;

        default:
            printf("login to default feature failed with status %d\n", status);
    }
    if (status) {
        exit(-1);
    }

    /***********************************************************************/

    wait4key("to continue");

    /***********************************************************************/

    printf("getting information about connected keys and usage:  ");
    scope = "<haspscope />\n";
    view = "<haspformat root=\"my_custom_scope\">\n"
                  "  <hasp>\n"
                  "    <attribute name=\"id\" />\n"
                  "    <attribute name=\"type\" />\n"
                  "    <feature>\n"
                  "      <attribute name=\"id\" />\n"
                  "      <element name=\"concurrency\" />\n"
                  "      <element name=\"license\" />\n"
                  "      <session>\n"
                  "        <element name=\"username\" />\n"
                  "        <element name=\"hostname\" />\n"
                  "        <element name=\"ip\" />\n"
                  "        <element name=\"apiversion\" />\n"
                  "      </session>\n"
                  "    </feature>\n"
                  "  </hasp>\n"
                  "</haspformat>\n";

    status = hasp_get_info(scope, view, vendor_code, &info);

    switch (status)
    {
        case HASP_STATUS_OK:
            /* use the info you received ... */
            printf("OK\n%s\n", info);

            /****************************************************************
             * hasp_free
             *   frees memory allocated by hasp_get_info, hasp_get_sessioninfo
             *   or hasp_update (if an acknowledge was requested)
             */
            hasp_free(info);
            break;

        case HASP_INV_FORMAT:
            printf("invalid XML info format\n");
            break;

        case HASP_INV_SCOPE:
            printf("invalid XML scope\n");
            break;

        default:
            printf("hasp_get_info failed with status %d\n", status);
    }
    if (status) {
        hasp_logout(handle);
        exit(-1);
    }

    /************************************************************************
     * hasp_logout
     *   closes established session and releases allocated memory
     */
    printf("\nlogout                           : ");
    status = hasp_logout(handle);

    switch (status)
    {
        case HASP_STATUS_OK:
            printf("OK\n");
            break;

        case HASP_INV_HND:
            printf("failed: handle not active\n");
            break;

        default:
            printf("failed\n");
    }
    if (status) {
        exit(-1);
    }

    /***********************************************************************/

    wait4key("to close the sample");

    return 0;

} /* main */

