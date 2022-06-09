//
// Demo program for Sentinel LDK license generation API
//
// Copyright (C) 2021 Thales Group. All rights reserved.
//

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "sntl_licgen.h" // license generation header file
#include "sntl_vcode.h"  // contains DEMOMA vendor code

char option;
int license_template_number;

char *c2v;
char *dxml[3];

char v2c_filename[256] = {0};
char uxml_filename[256] = {0};
char *current_state_filename;

char base64_alphabet[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int nothing_to_clear = 0;
int nothing_to_format = 0;
int dynamic_memory_supported_key = 0;   

enum
{
    SNTL_LG_SAMPLE_RETURN_ERROR = 0,
    SNTL_LG_SAMPLE_PARAMETER_ERROR,
    SNTL_LG_SAMPLE_DECODE_STATE_FAILED,
    SNTL_LG_SAMPLE_GENERATE_PROVISIONAL_FAILED,
    SNTL_LG_SAMPLE_GENERATE_BASE_INDEPENDENT_FAILED,
    SNTL_LG_SAMPLE_GENERATE_REHOST_FAILED,
    SNTL_LG_SAMPLE_GENERATE_DETACH_FAILED,
    SNTL_LG_SAMPLE_CLEAR_KEY_FAILED,
    SNTL_LG_SAMPLE_FORMAT_KEY_FAILED,
    SNTL_LG_SAMPLE_GENERATE_NEW_FAILED,
    SNTL_LG_SAMPLE_GENERATE_MODIFY_FAILED,
    SNTL_LG_SAMPLE_GENERATE_CANCEL_FAILED,
    SNTL_LG_SAMPLE_APPLY_TEMPLATE_FAILED,
    SNTL_LG_SAMPLE_LOAD_FILE_FAILED,
    SNTL_LG_SAMPLE_STORE_FILE_FAILED,
    SNTL_LG_SAMPLE_INITIALIZE_FAILED,
    SNTL_LG_SAMPLE_START_FAILED,
};

enum
{
    SNTL_LG_SAMPLE_LICENSE_TYPE = 0,
    SNTL_LG_SAMPLE_LICENSE_HL,
    SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE,
    SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE,
};


//
// get length of encoded string
//
int base64_encode_get_length(int input_length)
{
    int pad_count = 0;
    int length = 0;

    pad_count = (3 - (input_length % 3)) % 3;

    length = (input_length + pad_count) / 3 * 4;

    return length + 1;
}

//
// use BASE64 to encode string
//
int base64_encode(unsigned char *input, int input_length, char *output)
{
    int i = 0;
    int j = 0;
    int k = 0;

    int length = 0;
    int pad_count = 0;

    char *input_string = 0;
    unsigned char *temp = 0;

    if (input == 0 || output == 0)
    {
        return 1;
    }

    length = input_length;
    pad_count = (3 - (length % 3)) % 3;

    if (pad_count > 0)
    {
        input_string = (char *)malloc(length + pad_count);
        if (input_string == 0)
        {
            return 2;
        }
        memset(input_string, 0, length + pad_count);
        memcpy(input_string, input, length);
        temp = input_string;
    }
    else
    {
        temp = input;
    }

    for (i = 0; i < length; i += 3)
    {
        unsigned long val = 0;
        for (j = 0; j < 3; j++)
        {
            val |= *temp++;
            val <<= 8;
        }

        for (k = 0; k < 4; k++)
        {
            unsigned char index = (unsigned char)((val >> 26) & 0x3f);
            *output++ = base64_alphabet[index];
            val <<= 6;
        }
    }

    temp = 0;

    if (pad_count > 0)
    {
        free(input_string);
        input_string = 0;

        *(output - 1) = '=';
        pad_count--;

        if (pad_count > 0)
        {
            *(output - 2) = '=';
        }
    }

    *output = 0;

    return 0;
}

//
// get length of decoded string
//
int base64_decode_get_length(int input_length)
{
    if ((input_length % 4) != 0)
    {
        return 0;
    }

    return  input_length / 4 * 3;
}

//
// decode a single char
//
unsigned char base64_decode_char(char input)
{
    if ((input >= 'A') && (input <= 'Z'))
    {
        return input - 'A' + 0; // 0 range starts at 'A'
    }

    if ((input >= 'a') && (input <= 'z'))
    {
        return input - 'a' + 26; // 26 range starts at 'a'
    }

    if ((input >= '0') && (input <= '9'))
    {
        return input - '0' + 52; // 52 range starts at '0'
    }

    if (input == '+')
    {
        return 62;
    }

    if (input == '/')
    {
        return 63;
    }

    if (input == '=')
    {
        return 64;
    }

    return 65;
}

//
// use BASE64 to decode string
//
int base64_decode(char *input, unsigned char *output, int *output_length)
{
    int i = 0;
    int j = 0;
    int k = 0;

    int length = 0;
    int pad_count = 0;

    int temp = 0;

    if (input == 0 || output == 0)
    {
        return 1;
    }

    length = (int)strlen(input);

    // verify if input is a legal BASE64 string
    if (length % 4 != 0)
    {
        return 2;
    }

    for (i = 0; i < length; i++)
    {
        temp = base64_decode_char(input[i]);
        if (temp == 64)
        {
            if ((i != (length - 1)) && (i != (length - 2)))
            {
                return 3;
            }

            if (i == (length - 2))
            {
                if (base64_decode_char(input[i + 1]) != 64)
                {
                    return 4;
                }
                pad_count++;
            }
            else
            {
                pad_count++;
            }
        }

        if (temp == 65)
        {
            return 5;
        }
    }

    for (i = 0; i < length; i += 4)
    {
        unsigned long val = 0;
        for (j = 0; j < 4; j++)
        {
            unsigned char index = base64_decode_char(*input++);
            val |= index & 0x3f;
            val <<= 6;
        }
        val <<= 2;

        for (k = 0; k < 3; k++)
        {
            *output++ = (char)(val >> 24);
            val <<= 8;
        }
    }

    *output_length = length / 4 * 3 - pad_count;

    return 0;
}

//
// read content of file "filename" to "buffer"
//
int load_file(char *filename, char **buffer)
{
    int size = 0;
    FILE *fp = 0;

    fp = fopen(filename, "rb");
    if (!fp)
    {
        printf(" fopen failed.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size)
    {
        if (*buffer)
        {
            free(*buffer);
        }
        *buffer = 0;
        *buffer = (char *)malloc(size + 1);
        if (!*buffer)
        {
            printf(" malloc failed.\n");
            fclose(fp);
            return 1;
        }
        memset(*buffer, 0, size + 1);
        fread(*buffer, size, 1, fp);
        (*buffer)[size] = '\0';
    }
    else
    {
        fclose(fp);
        return 1;
    }
    fclose(fp);

    return 0;
}

void free_memory()
{
    int i = 0;
    for (i = 0; i < license_template_number; i++)
    {
        if (dxml[i] != NULL)
        {
            free(dxml[i]);
            dxml[i] = NULL; 
        }
    }
    
    if (c2v)
    {
        free(c2v);
        c2v = NULL;
    }
}

//
// save content of "buffer" to file "filename"
//
int store_file(char *buffer, char *filename)
{
    int size = 0;
    FILE *fp = 0;

    if (buffer == 0)
    {
        printf(" NULL buffer.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }
    size = (int)strlen(buffer);

    // Check for existence.
#ifdef _WIN32
    if( (_access( "output", 0 )) == -1 )
    {

        if (_mkdir( "output" ) == 0)
        {
            if (_mkdir( "output/HL" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }

            if (_mkdir( "output/SL-AdminMode" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }

            if (_mkdir( "output/SL-UserMode" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }
        else
        {
            printf(" Create directory failed.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else
    {
        if ( (_access( "output/HL", 0 )) == -1 )
        {
            if (_mkdir( "output/HL" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }

        if ( (_access( "output/SL-AdminMode", 0 )) == -1 )
        {
            if (_mkdir( "output/SL-AdminMode" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }

        if ( (_access( "output/SL-UserMode", 0 )) == -1 )
        {
            if (_mkdir( "output/SL-UserMode" ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }
    }
#else
    if( (access( "output", 0 )) == -1 )
    {
        if (mkdir( "output",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) == 0)
        {
            if (mkdir( "output/HL",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }

            if (mkdir( "output/SL-AdminMode",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }

            if (mkdir( "output/SL-UserMode",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }
        else
        {
            printf(" Create directory failed.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else
    {
        if ( (access( "output/HL", 0 )) == -1 )
        {
            if (mkdir( "output/HL",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }

        if ( (access( "output/SL-AdminMode", 0 )) == -1 )
        {
            if (mkdir( "output/SL-AdminMode",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }

        if ( (access( "output/SL-UserMode", 0 )) == -1 )
        {
            if (mkdir( "output/SL-UserMode",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
            {
                printf(" Create directory failed.\n");
                return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
            }
        }
    }
#endif

    fp = fopen(filename, "w+");
    if (!fp)
    {
        printf(" fopen failed.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }

    fwrite(buffer, size, 1, fp);
    fclose(fp);

    return 0;
}

//
// one license generation routine
//
int generate_license()
{
    int i = 0;
    int status = 0;

    char *init_param_xml = 0;
    char *start_param_xml = 0;
    char *license_definition_xml = 0;
    char *generation_param_xml = 0;

    char *license = 0;
    char *resultant_state = 0;

    sntl_lg_handle_t handle = SNTL_LG_INVALID_HANDLE_VALUE;

    char *error_msg = 0;

    //
    // sntl_lg_initialize
    //     Initializes license generation library
    //     and returns handle to work further
    //
    status = sntl_lg_initialize(init_param_xml, &handle);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_initialize: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_INITIALIZE_FAILED;
    }

    //
    // sntl_lg_start
    //     Starts the license generation.
    //
    status = sntl_lg_start(handle, start_param_xml, (sntl_lg_vendor_code_t)vendor_code, SNTL_LG_LICENSE_TYPE_UPDATE , license_definition_xml, c2v);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_start: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_START_FAILED;
    }

    //
    // sntl_lg_apply_template
    //     Apply license definition to the license state
    //     associated with the handle.
    //     You can call this API multiple times
    //     in one license generation routine.
    //
    for (i = 0; i < license_template_number; i++)
    {
        status = sntl_lg_apply_template(handle, dxml[i]);
        sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
        printf("  sntl_lg_apply_template: %s\n", error_msg);
        sntl_lg_free_memory(error_msg);
        if (status)
        {
            return SNTL_LG_SAMPLE_APPLY_TEMPLATE_FAILED;
        }
    }

    //
    // sntl_lg_generate_license
    //     Generates the license.
    //
    status = sntl_lg_generate_license(handle, generation_param_xml, &license, &resultant_state);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_generate_license: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    status = store_file(license, v2c_filename);
    if (status)
    {
        printf("  license file fail to save.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }

    status = store_file(resultant_state, uxml_filename);
    if (status)
    {
        printf("  resultant license container state file fail to save.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }

    if (handle != SNTL_LG_INVALID_HANDLE_VALUE)
    {
        sntl_lg_cleanup(&handle);
    }

    if (license != 0)
    {
        sntl_lg_free_memory(license);
        license = 0;
    }

    if (resultant_state != 0)
    {
        sntl_lg_free_memory(resultant_state);
        license = 0;
    }

    return 0;
}

void show_copyright()
{
    printf(" \n");
    printf(" A demo program for the LDK license generation functions\n");
    printf(" Copyright (C) Thales Group. All rights reserved.\n");
    printf(" \n");
}

void show_licgen_version()
{
    sntl_lg_status_t lg_status;

    unsigned int major_version = 0;
    unsigned int minor_version = 0;
    unsigned int build_server = 0;
    unsigned int build_number =0;

    lg_status = sntl_lg_get_version(&major_version, &minor_version, &build_server, &build_number);
    if (lg_status != SNTL_LG_STATUS_OK)
    {
        return;
    }

    printf(" \n");
    printf(" Sentinel LDK License Generation Windows DLL %d.%d build %d\n", major_version, minor_version, build_number);
    printf(" \n");
}

void show_help()
{
    printf(
        "\n"
        " Command line parameter usage of this program:\n"
        "     -h        Display this help message and exit.\n"
        "     -c        [default] Generate new, modify and cancel licenses.\n"
        "     -m        Generate new and modify licenses.\n"
        "     -n        Generate new license only.\n"
        "\n\n");
}

//
// deal with command line parameters
//
int handle_parameter(int argc, char *argv[])
{
    // if no parameter is given, -c will be taken as default.
    if (argc == 1)
    {
        option = 'c';
        return 0;
    }

    // take only 1 parameter
    if (argc > 2)
    {
        printf(" Error: unrecongnized or incomplete command line.\n");
        show_help();
        return 1;
    }

    if (!strcmp(argv[1], "-c"))
    {
        option = 'c';
        return 0;
    }

    if (!strcmp(argv[1], "-m"))
    {
        option = 'm';
        return 0;
    }

    if (!strcmp(argv[1], "-n"))
    {
        option = 'n';
        return 0;
    }

    // help menu
    if ( 0 != strcmp(argv[1], "-h") )
    {
        printf(" Error: unrecongnized or incomplete command line.\n");
    }

    show_help();

    return 2;
}

//
// load key type and key configuration from key state
//
void load_key_type_and_config(
    char *readable_key_state,
    char *key_type,
    char *key_config)
{
    char *temp_ptr = NULL;
    char *sub_str = 0;
    const char *field = 0;
#ifdef _WIN32
    temp_ptr = _strdup(readable_key_state);
#else
    temp_ptr = strdup(readable_key_state);
#endif

    if (readable_key_state == NULL)
    {
        printf ("Couldn't load key type and configuration from key state. Readable key state empty!\n");
        return;
    }

    // fetching key type
    sub_str = strstr(temp_ptr, "<type>");
    sub_str += strlen("<type>");
    sscanf(sub_str, "%40[^<]", key_type);

    //fetching key-configuration
    sub_str = strstr(temp_ptr, "<configuration_info>");
    sub_str += strlen("<configuration_info>");
    field = strtok(sub_str, " </>");
    do
    {
        if ((strcmp(field, "hasphl") == 0) ||
            (strcmp(field, "sentinelhl") == 0) ||
            (strcmp(field, "driverless") == 0))
        {
            if ((strlen(key_config) + strlen(field)) < 100)
            {
                strcat(key_config, field);
                strcat(key_config, " ");
            }
        }
        field = strtok(NULL, " </>");
    }
    while (strcmp(field, "configuration_info") != 0);

    if (temp_ptr)
    {
        free(temp_ptr);
        temp_ptr = NULL;
    }
}

//
// decode current state
//
int process_decode_current_state(int license_type)
{
    sntl_lg_handle_t handle = SNTL_LG_INVALID_HANDLE_VALUE;
    int status = 0;

    char *init_param_xml = 0;

    char *decoded_current_state = 0;

    char *error_msg = 0;

    char key_type[40] = "";

    char key_config[100] = "";

    printf(" Decode current state:\n");

    c2v = 0;

    // load c2v file
    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (load_file("input/HL/original_state.c2v", &c2v))
        {
            printf("  error in loading input/HL/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

    } 
    else if (  SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("input/SL-AdminMode/original_state.c2v", &c2v))
        {
            printf("  error in loading input/SL-AdminMode/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else if (  SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("input/SL-UserMode/original_state.c2v", &c2v))
        {
            printf("  error in loading input/SL-UserMode/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }

    //
    // sntl_lg_initialize
    // Initializes license generation library
    // and returns handle to work further
    //
    status = sntl_lg_initialize(init_param_xml, &handle);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_initialize: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_INITIALIZE_FAILED;
    }

    //
    // sntl_lg_decode_current_state
    // Decodes the current state.
    //
    status = sntl_lg_decode_current_state(handle, (sntl_lg_vendor_code_t)vendor_code, c2v, &decoded_current_state);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_decode_current_state: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_DECODE_STATE_FAILED;
    }

    //load key_type and key_configuration from key readable state
    load_key_type_and_config(decoded_current_state, key_type, key_config);

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        status = store_file(decoded_current_state, "output/HL/decoded_current_state.xml");

        // Check for dynamic memory supported key
        if (strstr(key_config, "driverless"))
        {
            if (strcmp(key_type, "Sentinel-HL-Pro") != 0 &&
                strcmp(key_type,  "Sentinel-HL-Basic") != 0)
            {
                dynamic_memory_supported_key = 1;
            }
        }
        else
        {
            printf("  This key doesn't support dynamic memory!\n"); 
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        status = store_file(decoded_current_state, "output/SL-AdminMode/decoded_current_state.xml");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE== license_type )
    {
        status = store_file(decoded_current_state, "output/SL-UserMode/decoded_current_state.xml");
    }
    else
    {
        ;//do nothing
    }

    if (status)
    {
        printf("  decoded_current_state.xml file fail to save.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }

    if (handle != SNTL_LG_INVALID_HANDLE_VALUE)
    {
        sntl_lg_cleanup(&handle);
    }

    if (decoded_current_state != 0)
    {
        sntl_lg_free_memory(decoded_current_state);
        decoded_current_state = 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
         printf("  Decoded current state file \"output/HL/decoded_current_state.xml\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  Decoded current state file \"output/SL-AdminMode/decoded_current_state.xml\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  Decoded current state file \"output/SL-UserMode/decoded_current_state.xml\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }   

    return 0;
}

//
// process generation of provisional license
//
int process_provisional(int license_type)
{    
    sntl_lg_handle_t handle = SNTL_LG_INVALID_HANDLE_VALUE;
    int status = 0;

    char *init_param_xml = 0;
    char *start_param_xml = 0;
    char *license_definition_xml = 0;
    char *generation_param_xml = 0;

    char *license = 0;
    char *resultant_state = 0;

    char *error_msg = 0;

    c2v = 0;

    printf(" Process provisional license:\n");  

    //
    // sntl_lg_initialize
    //     Initializes license generation library
    //     and returns handle to work further
    //
    status = sntl_lg_initialize(init_param_xml, &handle);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_initialize: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_INITIALIZE_FAILED;
    }

    // load provisional license definition file
    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("input/SL-AdminMode/provisional_template.xml", &license_definition_xml))
        {
            printf("  error in loading input/SL-AdminMode/provisional_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("input/SL-UserMode/provisional_template.xml", &license_definition_xml))
        {
            printf("  error in loading input/SL-UserMode/provisional_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }
    

    //
    // sntl_lg_start
    //     Starts the license generation.
    //
    status = sntl_lg_start(handle, start_param_xml, (sntl_lg_vendor_code_t)vendor_code, SNTL_LG_LICENSE_TYPE_PROVISIONAL, license_definition_xml, c2v);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_start: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_START_FAILED;
    }

    //
    // sntl_lg_generate_license
    //     Generates the license.
    //
    status = sntl_lg_generate_license(handle, generation_param_xml, &license, &resultant_state);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_generate_license: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status &&  status != SNTL_LG_NOTHING_TO_GENERATE)
    {
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }  

    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        status = store_file(license, "output/SL-AdminMode/provisional_license.v2c");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        status = store_file(license, "output/SL-UserMode/provisional_license.v2c");
    }
    else
    {
        ;//do nothing
    }
    
    if (status)
    {
        printf("  license file fail to save.\n");
        return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
    }   

    if (handle != SNTL_LG_INVALID_HANDLE_VALUE)
    {
        sntl_lg_cleanup(&handle);
    }

    if (license != 0)
    {
        sntl_lg_free_memory(license);
        license = 0;
    }

    if (resultant_state != 0)
    {
        sntl_lg_free_memory(resultant_state);
        resultant_state = 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/provisional_license.v2c\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/provisional_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }
    
    return 0;
}

//
// process generation of clear key license
//
int process_clear_key(int license_type)
{
    sntl_lg_handle_t handle = SNTL_LG_INVALID_HANDLE_VALUE;
    int status = 0;

    char *init_param_xml = 0;
    char *start_param_xml = 0;
    char *license_definition_xml = 0;
    char *generation_param_xml = 0;

    char *license = 0;
    char *resultant_state = 0;

    char *error_msg = 0;

    printf(" Process clear license:\n");

    c2v = 0;

    // load c2v file
    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (load_file("input/HL/original_state.c2v", &c2v))
        {
            printf("  error in loading input/HL/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("input/SL-AdminMode/original_state.c2v", &c2v))
        {
            printf("  error in loading input/SL-AdminMode/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("input/SL-UserMode/original_state.c2v", &c2v))
        {
            printf("  error in loading input/SL-UserMode/original_state.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }

    //
    // sntl_lg_initialize
    //     Initializes license generation library
    //     and returns handle to work further
    //
    status = sntl_lg_initialize(init_param_xml, &handle);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_initialize: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_INITIALIZE_FAILED;
    }

    //
    // sntl_lg_start
    //     Starts the license generation.
    //
    status = sntl_lg_start(handle, start_param_xml, (sntl_lg_vendor_code_t)vendor_code, SNTL_LG_LICENSE_TYPE_CLEAR_AND_UPDATE, license_definition_xml, c2v);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_start: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_START_FAILED;
    }

    //
    // sntl_lg_generate_license
    //     Generates the license.
    //
    status = sntl_lg_generate_license(handle, generation_param_xml, &license, &resultant_state);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_generate_license: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status &&  status != SNTL_LG_NOTHING_TO_GENERATE)
    {
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    if (status == SNTL_LG_NOTHING_TO_GENERATE)
    {
        nothing_to_clear = 1;
        printf("\n");
        return 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        status = store_file(license, "output/HL/clear_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/HL/resultant_state_after_clear.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        status = store_file(license, "output/SL-AdminMode/clear_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/SL-AdminMode/resultant_state_after_clear.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        status = store_file(license, "output/SL-UserMode/clear_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/SL-UserMode/resultant_state_after_clear.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }


    if (handle != SNTL_LG_INVALID_HANDLE_VALUE)
    {
        sntl_lg_cleanup(&handle);
    }

    if (license != 0)
    {
        sntl_lg_free_memory(license);
        license = 0;
    }

    if (resultant_state != 0)
    {
        sntl_lg_free_memory(resultant_state);
        license = 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf("  License file \"output/HL/clear_license.v2c\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/clear_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/clear_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }
    
    return 0;
}

//
// process generation of format key license
//
int process_format_key(int license_type)
{
    sntl_lg_handle_t handle = SNTL_LG_INVALID_HANDLE_VALUE;
    int status = 0;

    char *init_param_xml = 0;
    char *start_param_xml = 0;
    char *license_definition_xml = 0;
    char *generation_param_xml = 0;

    char *license = 0;
    char *resultant_state = 0;

    char *error_msg = 0;

    printf(" Process format license:\n");

    c2v = 0;

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (nothing_to_clear)
        {
            current_state_filename = "input/HL/original_state.c2v";
        }
        else
        {
            current_state_filename = "output/HL/resultant_state_after_clear.xml";
        }

    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (nothing_to_clear)
        {
            current_state_filename = "input/SL-AdminMode/original_state.c2v";
        }
        else
        {
            current_state_filename = "output/SL-AdminMode/resultant_state_after_clear.xml";
        }

    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (nothing_to_clear)
        {
            current_state_filename = "input/SL-UserMode/original_state.c2v";
        }
        else
        {
            current_state_filename = "output/SL-UserMode/resultant_state_after_clear.xml";
        }

    }
    else
    {
        ;//do nothing
    }

    // load c2v file
    if (load_file(current_state_filename, &c2v))
    {
        printf("  error in loading resultant_state_after_clear.xml file.\n");
        return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
    }

    //
    // sntl_lg_initialize
    //     Initializes license generation library
    //     and returns handle to work further
    //
    status = sntl_lg_initialize(init_param_xml, &handle);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_initialize: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_INITIALIZE_FAILED;
    }

    //
    // sntl_lg_start
    //     Starts the license generation.
    //
    status = sntl_lg_start(handle, start_param_xml, (sntl_lg_vendor_code_t)vendor_code, SNTL_LG_LICENSE_TYPE_FORMAT_AND_UPDATE, license_definition_xml, c2v);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_start: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status)
    {
        return SNTL_LG_SAMPLE_START_FAILED;
    }

    //
    // sntl_lg_generate_license
    //     Generates the license.
    //
    status = sntl_lg_generate_license(handle, generation_param_xml, &license, &resultant_state);
    sntl_lg_get_info(handle, SNTL_LG_INFO_LAST_ERROR_MESSAGE, &error_msg);
    printf("  sntl_lg_generate_license: %s\n", error_msg);
    sntl_lg_free_memory(error_msg);
    if (status &&  status != SNTL_LG_NOTHING_TO_GENERATE)
    {
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    if (status == SNTL_LG_NOTHING_TO_GENERATE)
    {
        nothing_to_format = 1;
        printf("\n");
        return 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        status = store_file(license, "output/HL/format_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/HL/resultant_state_after_format.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        status = store_file(license, "output/SL-AdminMode/format_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/SL-AdminMode/resultant_state_after_format.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        status = store_file(license, "output/SL-UserMode/format_license.v2c");
        if (status)
        {
            printf("  license file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }

        status = store_file(resultant_state, "output/SL-UserMode/resultant_state_after_format.xml");
        if (status)
        {
            printf("  resultant license container state file fail to save.\n");
            return SNTL_LG_SAMPLE_STORE_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }


    if (handle != SNTL_LG_INVALID_HANDLE_VALUE)
    {
        sntl_lg_cleanup(&handle);
    }

    if (license != 0)
    {
        sntl_lg_free_memory(license);
        license = 0;
    }

    if (resultant_state != 0)
    {
        sntl_lg_free_memory(resultant_state);
        license = 0;
    }

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf("  License file \"output/HL/format_license.v2c\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/format_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/format_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }

    return 0;
}

//
// process new license generation routine from original_state.c2v,
// new_license_template1.xml and new_license_template2.xml
//
int process_new(int license_type)
{
    c2v = 0;
    dxml[0] = 0;
    dxml[1] = 0;
    dxml[2] = 0;

    printf(" Process new license:\n");

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (nothing_to_format && nothing_to_clear)
        {
            current_state_filename = "input/HL/original_state.c2v";
        }
        else if (nothing_to_format && !nothing_to_clear)
        {
            current_state_filename = "output/HL/resultant_state_after_clear.xml";
        }
        else
        {
            current_state_filename = "output/HL/resultant_state_after_format.xml";
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (nothing_to_format && nothing_to_clear)
        {
            current_state_filename = "input/SL-AdminMode/original_state.c2v";
        }
        else if (nothing_to_format && !nothing_to_clear)
        {
            current_state_filename = "output/SL-AdminMode/resultant_state_after_clear.xml";
        }
        else
        {
            current_state_filename = "output/SL-AdminMode/resultant_state_after_format.xml";
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (nothing_to_format && nothing_to_clear)
        {
            current_state_filename = "input/SL-UserMode/original_state.c2v";
        }
        else if (nothing_to_format && !nothing_to_clear)
        {
            current_state_filename = "output/SL-UserMode/resultant_state_after_clear.xml";
        }
        else
        {
            current_state_filename = "output/SL-UserMode/resultant_state_after_format.xml";
        }
    }
    else
    {
        ;//do nothing
    }

    // load c2v file
    if (load_file(current_state_filename, &c2v))
    {
        printf("  error in loading resultant_state_after_format.xml file.\n");
        return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
    }

    // load license definition file
    license_template_number = 2;

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (load_file("input/HL/new_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/HL/new_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/HL/new_license_template2.xml", &dxml[1]))
        {
            printf("  error in loading input/HL/new_license_template2.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (dynamic_memory_supported_key == 1)
        { 
            if (load_file("input/HL/new_license_template3.xml", &dxml[2]))
            {
                printf("  error in loading input/HL/new_license_template3.xml file.\n");
                return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
            }
            // license definition for dynamic memory
            license_template_number += 1;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("input/SL-AdminMode/new_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/new_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-AdminMode/new_license_template2.xml", &dxml[1]))
        {
            printf("  error in loading input/SL-AdminMode/new_license_template2.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }		
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("input/SL-UserMode/new_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-UserMode/new_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-UserMode/new_license_template2.xml", &dxml[1]))
        {
            printf("  error in loading input/SL-UserMode/new_license_template2.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }		
    }
    else
    {
        ;//do nothing
    }

    if (  SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/HL/new_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/HL/resultant_state_after_new.xml");
    }
    else if (  SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-AdminMode/new_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_new.xml");
    }
    else if (  SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-UserMode/new_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-UserMode/resultant_state_after_new.xml");
    }
    else
    {
        ;//do nothing
    }	

    if (generate_license())
    {

        free_memory(); 

        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory(); 

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf("  License file \"output/HL/new_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/new_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/new_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }
    
    return 0;
}

//
// process modify license generation routine from resultant_state_after_new.xml
// and modify_license_template1.xml
//
int process_modify(int license_type)
{
    c2v = 0;
    dxml[0] = 0;
    dxml[1] = 0;
    printf(" Process modify license:\n");

    // load c2v file
    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (load_file("output/HL/resultant_state_after_new.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_new.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("output/SL-AdminMode/resultant_state_after_new.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_new.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("output/SL-UserMode/resultant_state_after_new.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_new.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }


    // load license definition file
    license_template_number = 1;

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        if (load_file("input/HL/modify_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/HL/modify_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (dynamic_memory_supported_key == 1)
        {
            if (load_file("input/HL/modify_license_template2.xml", &dxml[1]))
            {
                printf("  error in loading input/HL/modify_license_template2.xml file.\n");
                return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
            }
            license_template_number += 1;
        }
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("input/SL-AdminMode/modify_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/modify_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("input/SL-UserMode/modify_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-UserMode/modify_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }
    }
    else
    {
        ;//do nothing
    }


    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/HL/modify_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/HL/resultant_state_after_modify.xml");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-AdminMode/modify_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_modify.xml");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-UserMode/modify_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-UserMode/resultant_state_after_modify.xml");
    }
    else
    {
        ;//do nothing
    }


    if (generate_license())
    {
        free_memory();
        
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory();

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf("  License file \"output/HL/modify_license.v2c\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/modify_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/modify_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }
    
    return 0;
}

//
// process cancel license generation routine from resultant_state_after_modify.xml
// and cancel_license_template1.xml
//
int process_cancel(int license_type)
{
    c2v = 0;
    dxml[0] = 0;
    dxml[1] = 0;

    printf(" Process cancel license:\n");

    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        // load c2v file
        if (load_file("output/HL/resultant_state_after_modify.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_modify.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        // load license definition file
        license_template_number = 1;
        if (load_file("input/HL/cancel_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/HL/cancel_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (dynamic_memory_supported_key == 1)
        {
            if (load_file("input/HL/cancel_license_template2.xml", &dxml[1]))
            {
                printf("  error in loading input/HL/cancel_license_template2.xml file.\n");
                return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
            }
            license_template_number += 1;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/HL/cancel_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/HL/resultant_state_after_cancel.xml");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        if (load_file("output/SL-AdminMode/resultant_state_after_modify.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_modify.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        // load license definition file
        license_template_number = 1;
        if (load_file("input/SL-AdminMode/cancel_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/cancel_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-AdminMode/cancel_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_cancel.xml");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        if (load_file("output/SL-UserMode/resultant_state_after_modify.xml", &c2v))
        {
            printf("  error in loading resultant_state_after_modify.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        // load license definition file
        license_template_number = 1;
        if (load_file("input/SL-UserMode/cancel_license_template1.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/cancel_license_template1.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-UserMode/cancel_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-UserMode/resultant_state_after_cancel.xml");
    }
    else
    {
        ;//do nothing
    }

    if (generate_license())
    {
        free_memory(); 
        
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory(); 
        
    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf("  License file \"output/HL/cancel_license.v2c\" has been generated successfully.\n\n");
    } 
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/cancel_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/cancel_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }

    return 0;
}

//demonstrate the License type
void show_sample_license_type(int license_type)
{
    if ( SNTL_LG_SAMPLE_LICENSE_HL == license_type )
    {
        printf(" \n");
        printf(" The following is for Sentinel LDK HL key type\n");
        printf(" \n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf(" \n");
        printf(" The following is for Sentinel LDK SL Admin Mode key type\n");
        printf(" \n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf(" \n");
        printf(" The following is for Sentinel LDK SL User Mode key type\n");
        printf(" \n");
    }
    else
    {
        ;//do nothing
    }
}

//
// process generation of base independent license
//
int process_new_base_independent(int license_type)
{    
    dxml[0] = 0;
    dxml[1] = 0;

    printf(" Process base independent license:\n");

    // load license definition file
    license_template_number = 1;
  
    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        current_state_filename = "input/SL-AdminMode/fingerprint.c2v";

        // load c2v file
        if (load_file(current_state_filename, &c2v))
        {
            printf("  error in loading input/SL-AdminMode/fingerprint.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-AdminMode/base_independent_template.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/base_independent_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-AdminMode/new_base_independent_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_new_base_independent.xml");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        current_state_filename = "input/SL-UserMode/fingerprint.c2v";

        // load c2v file
        if (load_file(current_state_filename, &c2v))
        {
            printf("  error in loading input/SL-UserMode/fingerprint.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-UserMode/base_independent_template.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-UserMode/base_independent_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-UserMode/new_base_independent_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-UserMode/resultant_state_after_new_base_independent.xml");
    }
    else
    {
        ;//do nothing
    }
    
    if (generate_license())
    {
        free_memory(); 
        
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory(); 
        
    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/new_base_independent_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/new_base_independent_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }
    
    return 0;
}


//
// process generation of base independent license that allows rehosting 
//
int process_new_rehost(int license_type)
{    
    dxml[0] = 0;
    dxml[1] = 0;

    printf(" Process base independent license that allows rehosting:\n");

    // load license definition file
    license_template_number = 1;

    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        current_state_filename = "input/SL-AdminMode/fingerprint.c2v";

        // load c2v file
        if (load_file(current_state_filename, &c2v))
        {
            printf("  error in loading input/SL-AdminMode/fingerprint.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-AdminMode/rehost_license_template.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-AdminMode/rehost_license_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-AdminMode/rehost_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_rehost.xml");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        current_state_filename = "input/SL-UserMode/fingerprint.c2v";

        // load c2v file
        if (load_file(current_state_filename, &c2v))
        {
            printf("  error in loading input/SL-UserMode/fingerprint.c2v file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        if (load_file("input/SL-UserMode/rehost_license_template.xml", &dxml[0]))
        {
            printf("  error in loading input/SL-UserMode/rehost_license_template.xml file.\n");
            return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
        }

        memset(v2c_filename, 0, 256);
        strcpy(v2c_filename, "output/SL-UserMode/rehost_license.v2c");
        memset(uxml_filename, 0, 256);
        strcpy(uxml_filename, "output/SL-UserMode/resultant_state_after_rehost.xml");
    }
    else
    {
        ;//do nothing
    }

    if (generate_license())
    {
        free_memory(); 
        
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory(); 

    if ( SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE == license_type )
    {
        printf("  License file \"output/SL-AdminMode/rehost_license.v2c\" has been generated successfully.\n\n");
    }
    else if ( SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE == license_type )
    {
        printf("  License file \"output/SL-UserMode/rehost_license.v2c\" has been generated successfully.\n\n");
    }
    else
    {
        ;//do nothing
    }

    return 0;
}

//
// process generation of base independent detachable license
//
int process_new_detach()
{    
    dxml[0] = 0;
    dxml[1] = 0;

    printf(" Process base independent detachable license:\n");

    current_state_filename = "input/SL-AdminMode/fingerprint.c2v";

    // load c2v file
    if (load_file(current_state_filename, &c2v))
    {
        printf("  error in loading fingerprint.c2v file.\n");
        return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
    }

    // load license definition file
    license_template_number = 1;

    if (load_file("input/SL-AdminMode/detach_license_template.xml", &dxml[0]))
    {
        printf("  error in loading input/SL-AdminMode/detach_license_template.xml file.\n");
        return SNTL_LG_SAMPLE_LOAD_FILE_FAILED;
    }

    memset(v2c_filename, 0, 256);
    strcpy(v2c_filename, "output/SL-AdminMode/detach_license.v2c");
    memset(uxml_filename, 0, 256);
    strcpy(uxml_filename, "output/SL-AdminMode/resultant_state_after_detach.xml");
  
    if (generate_license())
    {
        free_memory(); 
        
        return SNTL_LG_SAMPLE_GENERATE_NEW_FAILED;
    }

    free_memory(); 
    
    printf("  License file \"output/SL-AdminMode/detach_license.v2c\" has been generated successfully.\n\n");

    return 0;
}

//
// entry point of program
//
int main(int argc, char *argv[])
{
    show_copyright();

    show_licgen_version();

    if (handle_parameter(argc, argv))
    {
        // wrong parameter
        return SNTL_LG_SAMPLE_PARAMETER_ERROR;
    }

    //demonstrate the HL example
    show_sample_license_type(SNTL_LG_SAMPLE_LICENSE_HL);

    if (process_decode_current_state(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to decode current state.\n\n");
    }

    if (process_clear_key(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to generate clear license.\n\n");
    }

    if (process_format_key(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to generate format license.\n\n");
    }

    if (process_new(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to generate new license.\n\n");
    }

    if (option == 'n')
    {
        goto GOTO_SL_AM_MODULE;
    }

    if (process_modify(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to generate modify license.\n\n");
    }
    if (option == 'm')
    {
        goto GOTO_SL_AM_MODULE;
    }

    if (process_cancel(SNTL_LG_SAMPLE_LICENSE_HL))
    {
        printf(" Fail to generate cancel license.\n\n");
    }

GOTO_SL_AM_MODULE:
    //demonstrate the SL-AdminMode example
    show_sample_license_type(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE);

    if (process_provisional(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate provisional license.\n\n");
    }

    if (process_new_base_independent(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate base independent license.\n\n");
    }

    if (process_new_rehost(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate base independent license that allows rehosting.\n\n");
    }

    if (process_new_detach())
    {
        printf(" Fail to generate base independent detachable license.\n\n");
    }

    if (process_decode_current_state(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to decode current state.\n\n");
    }

    if (process_clear_key(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate clear license.\n\n");
    }

    if (process_format_key(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate format license.\n\n");
    }

    if (process_new(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate new license.\n\n");
    }

    if (option == 'n')
    {
        goto GOTO_SL_UM_MODULE;
    }

    if (process_modify(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate modify license.\n\n");
    }

    if (option == 'm')
    {
        goto GOTO_SL_UM_MODULE;
    }

    if (process_cancel(SNTL_LG_SAMPLE_LICENSE_SL_ADMIN_MODE))
    {
        printf(" Fail to generate cancel license.\n\n");
    }

GOTO_SL_UM_MODULE:
    //demonstrate the SL-UserMode example
    show_sample_license_type(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE);

    if (process_provisional(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate provisional license.\n\n");
    }

    if (process_new_base_independent(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate base independent license.\n\n");
    }

    if (process_new_rehost(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate base independent license that allows rehosting.\n\n");
    }

    if (process_decode_current_state(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to decode current state.\n\n");
    }

    if (process_clear_key(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate clear license.\n\n");
    }

    if (process_format_key(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate format license.\n\n");
    }

    if (process_new(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate new license.\n\n");
    }

    if (option == 'n')
    {
        return 0;
    }

    if (process_modify(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate modify license.\n\n");
    }

    if (option == 'm')
    {
        return 0;
    }

    if (process_cancel(SNTL_LG_SAMPLE_LICENSE_SL_USER_MODE))
    {
        printf(" Fail to generate cancel license.\n\n");
    }

    return 0;
}
