/**
 * Sentinel LDK Administration API Demo
 * sntl_adminapi_demo.c
 *
 * Copyright (C) 2021 Thales Group. All rights reserved.
 *
 * 
 */


/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "sntl_adminapi.h"
#include "hasp_vcode.h"

/**************************************************************************************************/
/**
 * some definitions for demonstration environment
 */

#define VENDOR_ID   "37515"
#define KEY_ID      "123456"

/**************************************************************************************************/

#define LOG_FILE_NAME "sntl_admin_demo.log"
#define LINE  "-------------------------------------------------------------------------------\n"
#define DLINE "===============================================================================\n"

sntl_admin_context_t *ctx;
char                 *data = NULL, *backup = NULL;
int                   rc, error_count = 0;
FILE                 *fp = 0;

/**************************************************************************************************/

static void usage(char *progname)
{
    printf("Usage:      %s [hostname]\n"
           "            runs a sequence of AdminAPI requests for demonstration\n"
           "\n"
           "hostname:   Host name or IP address of the License Manager to query\n"
           "            Example: %s localhost\n"
           "            (if you are using RTLessAPI, you can access the integrated\n"
           "            License Manager by using sntl_integrated_lm as hostname\n",
           progname, progname);

    /* wait for enter (helps if started from explorer) */
    fflush(stdout);
    fflush(stdin);
    printf("\npress ENTER to exit ");
    while (getchar() == EOF);
    printf("\n");

}

/**************************************************************************************************
 * helper function: print a comment to screen and log file
 */

void comment ( char *s )
{
    if (s) {
        printf("%s\n", s);
        if (fp) {
            fprintf(fp, "\n" DLINE "%s\n", s);
        }
    }
}

/**************************************************************************************************
 * helper function: get readable constant name for status code
 */

char *error_text ( sntl_admin_status_t status )
{
    switch (status)
    {
        case SNTL_ADMIN_STATUS_OK           : return "SNTL_ADMIN_STATUS_OK";
        case SNTL_ADMIN_INSUF_MEM           : return "SNTL_ADMIN_INSUF_MEM";
        case SNTL_ADMIN_INVALID_CONTEXT     : return "SNTL_ADMIN_INVALID_CONTEXT";
        case SNTL_ADMIN_LM_NOT_FOUND        : return "SNTL_ADMIN_LM_NOT_FOUND";
        case SNTL_ADMIN_LM_TOO_OLD          : return "SNTL_ADMIN_LM_TOO_OLD";
        case SNTL_ADMIN_BAD_PARAMETERS      : return "SNTL_ADMIN_BAD_PARAMETERS";
        case SNTL_ADMIN_LOCAL_NETWORK_ERR   : return "SNTL_ADMIN_LOCAL_NETWORK_ERR";
        case SNTL_ADMIN_CANNOT_READ_FILE    : return "SNTL_ADMIN_CANNOT_READ_FILE";
        case SNTL_ADMIN_SCOPE_ERROR         : return "SNTL_ADMIN_SCOPE_ERROR";
        case SNTL_ADMIN_PASSWORD_REQUIRED   : return "SNTL_ADMIN_PASSWORD_REQUIRED";
        case SNTL_ADMIN_CANNOT_SET_PASSWORD : return "SNTL_ADMIN_CANNOT_SET_PASSWORD";
        case SNTL_ADMIN_UPDATE_ERROR        : return "SNTL_ADMIN_UPDATE_ERROR";
        case SNTL_ADMIN_LOCAL_ONLY          : return "SNTL_ADMIN_LOCAL_ONLY";
        case SNTL_ADMIN_BAD_VALUE           : return "SNTL_ADMIN_BAD_VALUE";
        case SNTL_ADMIN_READ_ONLY           : return "SNTL_ADMIN_READ_ONLY";
        case SNTL_ADMIN_ELEMENT_UNDEFINED   : return "SNTL_ADMIN_ELEMENT_UNDEFINED";
        case SNTL_ADMIN_INVALID_PTR         : return "SNTL_ADMIN_INVALID_PTR";
        case SNTL_ADMIN_NO_INTEGRATED_LM    : return "SNTL_ADMIN_NO_INTEGRATED_LM";
        case SNTL_ADMIN_RESULT_TOO_BIG      : return "SNTL_ADMIN_RESULT_TOO_BIG";
        case SNTL_ADMIN_SCOPE_RESULTS_EMPTY : return "SNTL_ADMIN_SCOPE_RESULTS_EMPTY";
        case SNTL_ADMIN_INV_VCODE           : return "SNTL_ADMIN_INV_VCODE";
        case SNTL_ADMIN_UNKNOWN_VCODE       : return "SNTL_ADMIN_UNKNOWN_VCODE";
        case SNTL_ADMIN_ENTROPY_SOURCE_FAILED: return "SNTL_ADMIN_ENTROPY_SOURCE_FAILED";
        case SNTL_ADMIN_INV_CERT             : return "SNTL_ADMIN_INV_CERT";
        case SNTL_ADMIN_CERT_VERIFY_FAILED   : return "SNTL_ADMIN_CERT_VERIFY_FAILED";
        case SNTL_ADMIN_CONNECT_MISSING      : return "SNTL_ADMIN_CONNECT_MISSING";
        case SNTL_ADMIN_NET_DLL_BROKEN       : return "SNTL_ADMIN_NET_DLL_BROKEN";
        case SNTL_ADMIN_DLL_LOADED_IN_ANOTHER_CLASSLOADER                    : return "SNTL_ADMIN_DLL_LOADED_IN_ANOTHER_CLASSLOADER";
        case SNTL_ADMIN_IDENTITY_CODE_REQUIRED                               : return "SNTL_ADMIN_IDENTITY_CODE_REQUIRED";
        case SNTL_ADMIN_ISSUED_TO_REQUIRED                                   : return "SNTL_ADMIN_ISSUED_TO_REQUIRED";
        case SNTL_ADMIN_MAXIMUM_NUMBER_OF_AUTO_REGISTERED_MACHINES_REQUIRED  : return "SNTL_ADMIN_MAXIMUM_NUMBER_OF_AUTO_REGISTERED_MACHINES_REQUIRED";
        case SNTL_ADMIN_IDENTITY_CODE_DOES_NOT_EXIST                         : return "SNTL_ADMIN_IDENTITY_CODE_DOES_NOT_EXIST";
        case SNTL_ADMIN_IDENTITY_CODE_CREATION_FAILED                        : return "SNTL_ADMIN_IDENTITY_CODE_CREATION_FAILED";
        case SNTL_ADMIN_IDENTITY_NOTHING_TO_UNREGISTER                        : return "SNTL_ADMIN_IDENTITY_NOTHING_TO_UNREGISTER";
        default                              : return "";
    }

    return "";
}

/**************************************************************************************************
 * helper function: remove leading and trailing whitespace
 */

void trim(char* s)
{
    char* begin = s;
    char* end = s + strlen(s);

    while (begin < end && begin[0] <= ' ') ++begin;
    while (begin < end && end[-1] <= ' ') --end;
    *end = 0;
    if (begin != s) memmove(s, begin, end - begin + 1);
}


/**************************************************************************************************
 * wrapper function for sntl_admin_get, logging data and results
 */

int my_sntl_admin_get( sntl_admin_context_t  *admin_context,
                       const char            *scope,
                       const char            *input,
                       char                 **info)
{
    printf("  sntl_admin_get()   ");

    if (fp)
    {
        if (scope) fprintf(fp, LINE "SCOPE:\n%s\n", scope);
        if (input) fprintf(fp, LINE "REQUEST: sntl_admin_get\n%s\n", input);
    }

    rc = sntl_admin_get(admin_context, scope, input, info);

    if (fp)
    {
        fprintf(fp, LINE "RESULT: Status %d %s\n", rc, error_text(rc));
        if (*info)
        {
            trim(*info);
            fprintf(fp, "%s\n", *info);
        }
    }
    printf("Status: %4d %s\n", rc, error_text(rc));
    return rc;
}


/**************************************************************************************************
 * wrapper function for sntl_admin_set, logging data and results
 */

int my_sntl_admin_set( sntl_admin_context_t  *admin_context,
                       const char            *input,
                       char                 **data)
{
    printf("  sntl_admin_set()   ");

    if (fp)
    {
        if (input) fprintf(fp, LINE "REQUEST: sntl_admin_set\n%s\n", input);
    }

    rc = sntl_admin_set(admin_context, input, data);

    if (fp)
    {
        fprintf(fp, LINE "RESULT: Status %d %s\n", rc, error_text(rc));
        if (*data)
        {
            trim(*data);
            fprintf(fp, "%s\n", *data);
        }
    }
    printf("Status: %4d %s\n", rc, error_text(rc));
    return rc;
}

/**************************************************************************************************
 * helper function: display return code in plain text
 */

void check_result ( int expected, int actual )
{
  if (actual != expected)
  {
      error_count++;
      printf("*** Unexpected AdminAPI result - Expected: %u %s, Actual: %u %s ***\n",
             expected, error_text(expected),
             actual, error_text(actual));
      if (fp)
          fprintf(fp,
                  "*** Unexpected AdminAPI result\n"
                  "***   Expected: %u %s\n"
                  "***   Actual: %u %s\n",
                  expected, error_text(expected),
                  actual, error_text(actual));
  }
  if (fp)
      fprintf(fp, DLINE "\n");
}

/**************************************************************************************************/

int main(int argc, char **argv)
{
    char scope[4096];
    char my_host[256];

    printf("\nA simple demo program for the Sentinel LDK Administration API functions\n");
    printf("Copyright (C) Thales Group. All rights reserved.\n\n");

    error_count = 0;


    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }

    strcpy(my_host, argv[1]);
    printf("Using License Manager: %s\n", my_host);

    /************************************************************************************************
  	 * create the log file
     */
    printf("creating log file " LOG_FILE_NAME "\n\n");
    fp = fopen(LOG_FILE_NAME, "wt");
    if (!fp) {
      printf("could not create log file\n");
    }

    /**********************************************************************************************
     * Create a context for License Manager as specified in command line
     */
    sprintf(scope, "<haspscope>"
                   "  <host>%s</host>"
                   "  <vendor_code>%s</vendor_code>"   /* required for sntl_integrated_lm only */
                   "</haspscope>",
                   my_host,
                   vendor_code);

    rc = sntl_admin_context_new_scope(&ctx, scope);
    if (rc) check_result(SNTL_ADMIN_STATUS_OK, rc);

    comment("Retrieve current context");
    rc = my_sntl_admin_get(ctx,
                           0,
                           "<context></context>",
                           &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);

    /**********************************************************************************************/

    comment("Retrieve all configuration settings (for backup)");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"*\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);

    /* extract the <config></config> block and build restore string */
    if ( (rc==SNTL_ADMIN_STATUS_OK) && (data) ) {
      char *p1, *p2;
      if ((p1 = strstr(data, "<config>")) !=0) {
        if ((p2 = strstr(data, "</config>")) !=0) {
          *p2 = '\0';
          backup = malloc(strlen(p1)+1024);
          if (backup) {
            strcpy(backup, p1);
            strcat(backup, "<writeconfig />\n" /* on restore, write back to ini file */
                           "</config>");
          }
        }
      }
    }

    sntl_admin_free(data);

    /**********************************************************************************************/


    comment("Retrieve selected configuration settings\n"
            "(For a list of available elements, see reply for \"*\" element above)");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"friendlyname\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /**********************************************************************************************
     * Password related
     *   this part is disabled because it cannot be automatically restored
     */

#if 0
    comment("Attempt to change bad password to 'password'");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <adminpassold>f1bfc72887902986b95f3dfdf1b81a5b</adminpassold>\n" /* "badpass"  */
                    " <adminpassnew>5f4dcc3b5aa765d61d8327deb882cf99</adminpassnew>\n" /* "password" */
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_CANNOT_SET_PASSWORD, rc);
    sntl_admin_free(data);


    comment("change empty password to 'password'");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <adminpassold>d41d8cd98f00b204e9800998ecf8427e</adminpassold>\n" /* (empty)    */
                    " <adminpassnew>5f4dcc3b5aa765d61d8327deb882cf99</adminpassnew>\n" /* "password" */
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve a configuration setting without password");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"friendlyname\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_PASSWORD_REQUIRED, rc);
    sntl_admin_free(data);

    sntl_admin_context_delete(ctx);



    comment("Establish context with the new password");
    sprintf(scope, "<haspscope>"
                   "  <host>%s</host>"
                   "  <vendor_code>%s</vendor_code>"   /* required for sntl_integrated_lm only */
                   "  <password>password</password>"
                   "</haspscope>",
                   my_host,
                   vendor_code);

    rc = sntl_admin_context_new_scope(&ctx, scope);
    check_result(SNTL_ADMIN_STATUS_OK, rc);

    rc = my_sntl_admin_get(ctx,
                           0,
                           "<context></context>",
                           &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve a configuration setting with the new password");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"friendlyname\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("change password back to empty");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <adminpassold>5f4dcc3b5aa765d61d8327deb882cf99</adminpassold>\n" /* "password" */
                    " <adminpassnew>d41d8cd98f00b204e9800998ecf8427e</adminpassnew>\n" /* (empty)    */
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);

    sntl_admin_context_delete(ctx);


    sprintf(scope, "<haspscope>"
                   "  <host>%s</host>"
                   "  <vendor_code>%s</vendor_code>"   /* required for sntl_integrated_lm only */
                   "</haspscope>",
                   my_host,
                   vendor_code);

    rc = sntl_admin_context_new_scope(&ctx, scope);
    check_result(SNTL_ADMIN_STATUS_OK, rc);


    comment("Retrieve a configuration setting");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"friendlyname\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);
#endif

    /*********************************************************************************************
     * Examples for incorrect requests and error handling
     */
#if 0
    comment("Request with an error in scope");
    rc = my_sntl_admin_get(ctx,
           "<haspscope>\n"
           " <vndor id=\"37517\" />\n"
           "</haspscope>\n",

           "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
           "<admin>\n"
           " <hasp>\n"
           "  <element name=\"haspid\" />\n"
           " </hasp>\n"
           "</admin>\n",

           &data);
    check_result(SNTL_ADMIN_SCOPE_ERROR, rc);
    sntl_admin_free(data);


    comment("Multiple errors in request");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <i_cannot_do_this />\n"
                    " <nor_that />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_BAD_PARAMETERS, rc);
    sntl_admin_free(data);


    comment("Request containing both valid and invalid items\n"
            "Valid items will be accepted");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <i_cannot_do_this>Dave</i_cannot_do_this>\n"
                    " <enabledetach>1</enabledetach>\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_BAD_PARAMETERS, rc);
    sntl_admin_free(data);
#endif


    comment("Simple configuration setting");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <enabledetach>1</enabledetach>\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /**********************************************************************************************
     * examples for configuration settings
     */

    comment("Change some settings and write to ini file\n"
            "(For a list of available elements, see reply for \"*\" element below)");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"

                    " <serveraddrs_clear/>\n"
                    " <serveraddr>10.24.2.16   </serveraddr>\n"
                    " <serveraddr>    10.24.2.16</serveraddr>\n"
                    " <serveraddr>p4p</serveraddr>\n"
                    " <serveraddr>10.24.2.255</serveraddr>\n"

                    " <user_restrictions_clear/>\n"
                    " <user_restriction>deny=baerbel@all</user_restriction>\n"

                    " <access_restrictions_clear />\n"
                    " <access_restriction>deny=10.23.*</access_restriction>\n"

                    " <enabledetach>1</enabledetach>\n"

                    " <writeconfig />\n"

                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all configuration settings");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"*\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Add some access restrictions");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <access_restriction>allow=123</access_restriction>\n"
                    " <access_restriction>allow=abcd</access_restriction>\n"
                    " <access_restriction>allow=hello_world</access_restriction>\n"
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all access restrictions");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"access_restriction\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Add some more access restrictions");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <access_restriction>allow=more_123</access_restriction>\n"
                    " <access_restriction>allow=more_abcd</access_restriction>\n"
                    " <access_restriction>allow=more_hello_world</access_restriction>\n"
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all access restrictions");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"access_restriction\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Delete existing access restrictions and add some new ones");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <access_restrictions_clear/>\n"
                    " <access_restriction>allow=new_123</access_restriction>\n"
                    " <access_restriction>allow=new_abcd</access_restriction>\n"
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all access restrictions");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <config>\n"
                    "  <element name=\"access_restriction\" />\n"
                    " </config>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);

    /**********************************************************************************************
     * using haspscope to retrieve filtered data
     */

    comment("Retrieve some key data for specified vendor (scope with attribute notation)\n"
            "(for a list of available elements, see reply for \"*\" element below)");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <vendor id=\"" VENDOR_ID "\" />\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <hasp>\n"
                    "  <element name=\"vendorid\" />\n"
                    "  <element name=\"haspid\" />\n"
                    "  <element name=\"typename\" />\n"
                    "  <element name=\"local\" />\n"
                    "  <element name=\"localname\" />\n"
                    " </hasp>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve key data for specified vendor (scope with element notation)");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <vendor><id>" VENDOR_ID "</id></vendor>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <hasp>\n"
                    "  <element name=\"vendorid\" />\n"
                    "  <element name=\"haspid\" />\n"
                    "  <element name=\"typename\" />\n"
                    "  <element name=\"local\" />\n"
                    "  <element name=\"localname\" />\n"
                    " </hasp>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /* key-specific examples disabled; please specify an existing KEY_ID */
#if 0
    comment("Retrieve all key data for specified key id");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <hasp><id>" KEY_ID "</id></hasp>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <hasp>\n"
                    "  <element name=\"*\" />\n"
                    " </hasp>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all feature data for specified key id");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <hasp><id>" KEY_ID "</id></hasp>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <feature>\n"
                    "  <element name=\"*\" />\n"
                    " </feature>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve all product data for specified key id");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <hasp><id>" KEY_ID "</id></hasp>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <product>\n"
                    "  <element name=\"productid\" />\n"
                    "  <element name=\"productname\" />\n"
                    "  <element name=\"detachable\" />\n"
                    "  <element name=\"maxseats\" />\n"
                    "  <element name=\"seatsfree\" />\n"
                    " </product>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);
#endif


    comment("Retrieve all product data for specified vendor id");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <vendor><id>" VENDOR_ID "</id></vendor>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <product>\n"
                    "  <element name=\"*\" />\n"
                    " </product>\n"
                    "</admin>",

                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);



    /**********************************************************************************************
     * retrieve License Manager and license related data
     */
    comment("Retrieve license manager data in XML format (default)");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <license_manager>\n"
                    "  <element name=\"*\" />\n"
                    " </license_manager>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve license manager data in JSON format");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <license_manager format=\"json\">\n"
                    "  <element name=\"*\" />\n"
                    " </license_manager>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Retrieve list of detached licenses");
    rc = my_sntl_admin_get(ctx,
                    0,

                    "<admin>\n"
                    " <detached>\n"
                    "  <element name=\"*\" />\n"
                    " </detached>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /**********************************************************************************************
     * Setting a key description (disabled because it was not backed up)
     */
#if 0
    comment("Add a key description (legacy format)");
    rc = my_sntl_admin_set(ctx,
                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                    "<keydescription>\n"
                    " <hasp>\n"
                    "  <id>123456</id>\n"
                    "  <name>One two three four five six</name>\n"
                    " </hasp>\n"
                    "</keydescription>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);
#endif

    /**********************************************************************************************
     * Examples for uploading of files (disabled because reverting not possible)
     */
#if 0
    comment("Upload a detach location data file");
    rc = my_sntl_admin_set(ctx,
                    "file://test_location.xml",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Upload a key names metadata file");
    rc = my_sntl_admin_set(ctx,
                    "file://test_key.xml",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Upload a product names metadata file");
    rc = my_sntl_admin_set(ctx,
                    "file://test_product.xml",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Upload a vendor names metadata file");
    rc = my_sntl_admin_set(ctx,
                    "file://test_vendor.xml",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    comment("Applying V2C");
    rc = my_sntl_admin_set(ctx,
                    "file://test_update.v2c",
                    &data);
    check_result(SNTL_ADMIN_UPDATE_ERROR, rc);
    sntl_admin_free(data);
#endif

    /**********************************************************************************************
     * Session functions
     */
    comment("Retrieve list of all current sessions");
    rc = my_sntl_admin_get(ctx,
                    0,
                    "<admin>\n"
                    " <session>\n"
                    "  <element name=\"*\" />\n"
                    " </session>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /* key-specific examples disabled; please specify an existing KEY_ID */
#if 0
    comment("Retrieve list of current sessions for a specified key");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <hasp><id>" KEY_ID "</id></hasp>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <session>\n"
                    "  <element name=\"*\" />\n"
                    " </session>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);
#endif

    comment("Retrieve selected session data for all keys of a specified vendor");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <vendor><id>" VENDOR_ID "</id></vendor>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <session>\n"
                    "  <element name=\"user\" />\n"
                    "  <element name=\"machine\" />\n"
                    "  <element name=\"logintime\" />\n"
                    " </session>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /**********************************************************************************************
     * Deleting of sessions (disabled here because not reversible)
     */
#if 0
    rc = my_sntl_admin_set(ctx,
                    "<admin>\n"
                    " <deletesession>\n"
                    "  <sessionid>1</sessionid>\n"
                    "  <session id=\"2\" />\n"
                    "  <session id=\"3\" />\n"
                    "  <sessionid>4</sessionid>\n"
                    " </deletesession>\n"
                    "</admin>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);
#endif

    /**********************************************************************************************
     * Certificate License related examples
     */
#if 0
    comment("Retrieve list of certificates for specified key (XML)");
    rc = my_sntl_admin_get(ctx,
                    "<haspscope>\n"
                    " <hasp><id>" KEY_ID "</id></hasp>\n"
                    "</haspscope>",

                    "<admin>\n"
                    " <certificates>\n"
                    "  <element name=\"*\" />\n"
                    " </certificates>\n"
                    "</admin>",
                    &data);
    sntl_admin_free(data);
#endif

    /**********************************************************************************************
     * restore original configuration
     */
    if (backup) {
      comment("Restore backup of original configuration");

      /* Clear any list entries we may have added */
      rc = sntl_admin_set(ctx,
                    "<config>\n"
                    " <serveraddrs_clear/>\n"
                    " <user_restrictions_clear/>\n"
                    " <access_restrictions_clear/>\n"
                    " <writeconfig/>\n"
                    "</config>",
                    &data);
      sntl_admin_free(data);

      /*
       * Now restore the configuration settings backup
       * This command returns an error code due the attempt to set read-only attributes, but all the others are set
       */
      rc = sntl_admin_set(ctx,
                      backup,
                      &data);
      sntl_admin_free(data);

      free(backup);
    }

    /**********************************************************************************************
     * set defaults configuration
     */
    comment("Set configuration defaults and write to ini file");
    rc = my_sntl_admin_set(ctx,
                    "<config>\n"
                    " <set_defaults />\n"
                    " <writeconfig />\n"
                    "</config>",
                    &data);
    check_result(SNTL_ADMIN_STATUS_OK, rc);
    sntl_admin_free(data);


    /**********************************************************************************************
     * show and log summary of results
     */
    if (error_count) {
      printf("\n*** Unexpected AdminAPI results: %u ***\n", error_count);
      if (fp)
        fprintf(fp, "*** Unexpected AdminAPI results: %u ***\n", error_count);
    } else {
      printf("\n*** All results as expected ***\n");
      if (fp)
        fprintf(fp, "*** All results as expected ***\n");
    }

    /**********************************************************************************************
     * close log file
     */
    printf("\nclosing log file " LOG_FILE_NAME "\n");
    if (fp) fclose(fp);

    sntl_admin_context_delete(ctx);
    return 0;

} /* main */

