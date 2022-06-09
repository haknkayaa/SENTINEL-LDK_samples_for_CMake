/**
 * \file sntl_licgen.h Sentinel LDK License Generation API declarations
 * Copyright (C) 2021 Thales Group. All rights reserved.
 *
 */

#ifndef __SNTL_LICGEN_H__
#define __SNTL_LICGEN_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup sntl_lg_general Sentinel LDK License Generation Typedefs and Macros
 *
 * @{
 */

/** Sentinel LDK LicGen Calling Convention for APIs */
#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER) || defined(__BORLANDC__)
#  define SNTL_LG_CALLCONV __stdcall
#else
#  define SNTL_LG_CALLCONV
#endif

/**
 * @def SNTL_LG_INVALID_HANDLE_VALUE
 * Invalid handle value for \ref sntl_lg_initialize() function.
 */
#define SNTL_LG_INVALID_HANDLE_VALUE 0

/**
  * @def SNTL_LG_CURRENT_SCHEMA_VER
  * Xml based license definition's supported schema version .
  */
#define SNTL_LG_CURRENT_SCHEMA_VER 100

/** Sentinel LDK LicGen 32-bit unsigned type */
#if !defined(WITH_AKSTYPES) && !defined(WITH_OEFTYPES)
#  if defined(_MSC_VER)
typedef unsigned long sntl_lg_u32_t;
#  else
typedef unsigned int sntl_lg_u32_t;
#  endif
#endif

/** Sentinel LDK LicGen handle */
typedef sntl_lg_u32_t sntl_lg_handle_t;

/** Sentinel LDK Vendor Code buffer */
typedef const void *sntl_lg_vendor_code_t;

/** License type to be passed in \ref sntl_lg_start() API */
typedef enum
{
   /** Invalid value for the license type, to be used for initialization purpose */
   SNTL_LG_LICENSE_TYPE_INVALID = 0,
   /** Generates a license for new or modified features */
   SNTL_LG_LICENSE_TYPE_UPDATE  = 1,
   /** Generates a license that deletes all existing Features on the Sentinel protection key and burns the new or modified features */
   SNTL_LG_LICENSE_TYPE_CLEAR_AND_UPDATE = 2,
   /** Generates a license that deletes all existing Features on the Sentinel protection key and burns the new or modified features. 
       The license also erases the key memory area before applying the update of memory */
   SNTL_LG_LICENSE_TYPE_FORMAT_AND_UPDATE = 3,
   /** Generates a provisional license for SL-AdminMode or SL-UserMode keys */
   SNTL_LG_LICENSE_TYPE_PROVISIONAL = 4,
   /** Generates a generic license that can be applied to all keys of a certain vendor. 
       Reserved for Thales Internal use */
   SNTL_LG_LICENSE_TYPE_GENERIC_UPDATE = 5,
   /** Generates a license to clear clone for SL Keys */ 
   SNTL_LG_LICENSE_TYPE_CLEAR_CLONE = 6,
   /** Generates a license to re-enable the disabled HL keys */ 
   SNTL_LG_LICENSE_TYPE_CLEAR_DISABLED_STATE = 7,
   /** Generates a license for Trusted License Storage */ 
   SNTL_LG_LICENSE_TYPE_VENDOR_AUTHORIZATION = 8,
} sntl_lg_license_type_t;

/** Info type to be passed in \ref sntl_lg_get_info() API */
typedef enum
{
   /** Invalid value for the info type, to be used for initialization purpose */
   SNTL_LG_INFO_INVALID = 0,
   /** To get capable device information. It will be used to identify the devices that are capable
       to hold the license state attached with the handle   */
   SNTL_LG_INFO_CAPABLE_DEVICES = 1,
   /** To get the last error message associated with a handle */
   SNTL_LG_INFO_LAST_ERROR_MESSAGE = 2,
} sntl_lg_info_type_t;

/**
 * @}
 */

/**
 * @defgroup sntl_lg_error_codes Sentinel LDK License Generation API Status Codes
 *
 * @{
 */

/** Sentinel LDK LicGen Error Codes */
enum sntl_lg_error_codes
{
   /** Request successfully completed. */
   SNTL_LG_STATUS_OK = 0,

   /** Library initialization failed. */
   SNTL_LG_LIB_INITIALIZATION_FAILED = 5001,

   /** Failure in locking a resource. */
   SNTL_LG_RESOURCE_LOCK_FAILURE = 5002,

   /** System is out of memory. */
   SNTL_LG_OUT_OF_MEMORY = 5003,

   /** Invalid function parameter. */
   SNTL_LG_INVALID_PARAMETER = 5004,

   /** Invalid handle passed to function. */
   SNTL_LG_INVALID_HANDLE = 5005,

   /** Too many open handles. */
   SNTL_LG_TOO_MANY_OPEN_HANDLES = 5006,

   /** Invalid API call sequence. */
   SNTL_LG_INVALID_API_CALL_SEQUENCE = 5007,

   /** Invalid Vendor Code passed. */
   SNTL_LG_INVALID_VENDOR_CODE = 5008,

   /** Invalid license definition. */
   SNTL_LG_INVALID_LICENSE_DEFINITION = 5009,

   /** Invalid key current state. */
   SNTL_LG_INVALID_KEY_CURRENT_STATE = 5010,

   /** Invalid schema version. */
   SNTL_LG_INVALID_SCHEMA_VERSION = 5011,

   /** Internal error occurred. */
   SNTL_LG_INTERNAL_ERROR = 5012,

   /** Key is full. */
   SNTL_LG_KEY_FULL = 5013,

   /** Key type is not supported. */
   SNTL_LG_NOT_SUPPORTED_KEY_TYPE = 5014,

   /** HL Basic key cannot be programmed. */
   SNTL_LG_HL_BASIC_KEY_CANNOT_BE_PROGRAMMED = 5015,

   /** Key does not support license based on days to expiration. */
   SNTL_LG_DAYS_TO_EXPIRATION_NOT_SUPPORTED = 5016,

   /** Key does not support license based on expiration date. */
   SNTL_LG_EXPIRY_DATE_NOT_SUPPORTED = 5017,

   /** Unlimited concurrency count is not supported. */
   SNTL_LG_UNLIMITED_CONCURRENCY_COUNT_NOT_SUPPORTED = 5018,

   /** Concurrency is not supported. */
   SNTL_LG_CONCURRENCY_NOT_SUPPORTED = 5019,

   /** Data size is larger than RO memory capacity. */
   SNTL_LG_TOO_BIG_RO_MEM_DATA = 5020,

   /** Data size is larger than RW memory capacity. */
   SNTL_LG_TOO_BIG_RW_MEM_DATA = 5021,

   /** Concurrency count exceeds the maximum limit. */
   SNTL_LG_EXCEEDING_MAX_CONCURRENCY_COUNT_LIMIT = 5022,

   /** Execution count exceeds the maximum limit. */
   SNTL_LG_EXCEEDING_MAX_EXECUTION_COUNT_LIMIT = 5023,

   /** Error occurred while connecting the Master key. */
   SNTL_LG_MASTER_KEY_CONNECT_ERROR = 5024,

   /** Communication with Master key failed. Master key might not be present. */
   SNTL_LG_MASTER_KEY_IO_ERROR = 5025,

   /** Some error is observed in accessing the Master key. */
   SNTL_LG_MASTER_KEY_ACCESS_ERROR = 5026,

   /** Current state of key is mandatory to generate a license. */
   SNTL_LG_KEY_CURRENT_STATE_MISSING = 5027,

   /** Expiration date exceeds the maximum expiration date supported. */
   SNTL_LG_EXCEEDING_MAX_EXPIRY_DATE_LIMIT = 5028,

   /** Expiration date is below the minimum expiration date supported. */
   SNTL_LG_BELOW_MIN_EXPIRY_DATE_LIMIT = 5029,

   /** There is nothing to be generated as a license. */
   SNTL_LG_NOTHING_TO_GENERATE = 5030,

   /** No capable device found. */
   SNTL_LG_NO_CAPABLE_DEVICE_FOUND = 5032,

   /** Mismatch in enforcement type. */
   SNTL_LG_INCOMPATIBLE_ENFORCEMENT_TYPE = 5034,

   /** Invalid fingerprint. */
   SNTL_LG_INVALID_FINGERPRINT =  5035,

   /** Clear operation not allowed on the given key. */
   SNTL_LG_CLEAR_NOT_ALLOWED = 5036,

   /** Format operation not allowed on the given key. */
   SNTL_LG_FORMAT_NOT_ALLOWED = 5037,

   /** Current state not allowed. */
   SNTL_LG_CURRENT_STATE_NOT_ALLOWED = 5038,

   /** Clone detected. */
   SNTL_LG_CLONE_DETECTED = 5039,

   /** No license definition. */
   SNTL_LG_NO_LICENSE_DEFINITION = 5040,

   /** Insufficient number of seats available on Master Key for charging. */
   SNTL_LG_INSUFFICIENT_SEAT_POOL = 5041,

   /** Feature not found on Master Key. */
   SNTL_LG_MASTER_FEATURE_NOT_FOUND = 5043,

   /** Feature on Master Key has expired. */
   SNTL_LG_MASTER_FEATURE_EXPIRED = 5044,

   /** Battery in the Master key is depleted. */
   SNTL_LG_MASTER_NO_BATTERY_POWER = 5045,

   /** Concurrency count below the minimum limit. */
   SNTL_LG_BELOW_MIN_CONCURRENCY_COUNT_LIMIT = 5046,

   /** Execution count below the minimum limit. */
   SNTL_LG_BELOW_MIN_EXECUTION_COUNT_LIMIT = 5047,

   /** Invalid enforcement type. */
   SNTL_LG_INVALID_ENFORCEMENT_TYPE = 5048,

   /** Enforcement type not supported. */
   SNTL_LG_ENFORCEMENT_TYPE_NOT_SUPPORTED = 5049,

   /** Either Sentinel HL driver (update to 1.22 or later) is or Sentinel HL Firmware (update to 3.25 or later) too old. */
   SNTL_LG_OLD_DRIVER = 5050,

   /** Sentinel License Manager is too old. */
   SNTL_LG_OLD_LM = 5051,

   /** VClock not enabled on the key. */
   SNTL_LG_VCLOCK_NOT_ENABLED = 5052,

   /** Sentinel HL Firmware is too old. */
   SNTL_LG_OLD_FIRMWARE = 5053,

   /** Key does not support dynamic resources. */
   SNTL_LG_DYNAMIC_RESOURCE_NOT_SUPPORTED = 5054,

   /** Clear clone operation not allowed on the given key. */
   SNTL_LG_CLEAR_CLONE_NOT_ALLOWED = 5055,

   /** License definition not allowed. */
   SNTL_LG_LICENSE_DEFINITION_NOT_ALLOWED = 5056,

   /** License not allowed on virtual machine. */
   SNTL_LG_LICENSE_DENIED_ON_VM = 5057,

   /** Clear disabled state operation not allowed on the given key. */
   SNTL_LG_CLEAR_DISABLED_STATE_NOT_ALLOWED = 5058,

   /** Sentinel License Manager on vendor machine too old (upgrade runtime to latest). */
   SNTL_LG_VENDOR_OLD_LM = 5059 ,

   /** Product count exceeds the maximum limit. */
   SNTL_LG_EXCEEDING_MAX_PRODUCT_COUNT_LIMIT = 5060,

   /** Feature count exceeds the maximum limit. */
   SNTL_LG_EXCEEDING_MAX_FEATURE_COUNT_LIMIT = 5061,

   /** File count exceeds the maximum limit. */
   SNTL_LG_EXCEEDING_MAX_FILE_COUNT_LIMIT = 5062,

   /** Unlimited concurrency count and cloud license cannot be combined. */
   SNTL_LG_UNLIMITED_CONCURRENCY_AND_CLOUD_NOT_ALLOWED = 5063,

   /** Authorization of the vendor is not allowed on the specified key. */
   SNTL_LG_VENDOR_AUTHORIZATION_NOT_ALLOWED = 5064,
   
   /** License not allowed on trusted storage. */
   SNTL_LG_NOT_ALLOWED_TRUSTED_STORAGE = 5065,
};

/** A Sentinel LDK LicGen status code */
typedef enum sntl_lg_error_codes sntl_lg_status_t;

/**
 * @}
 */

/**
 * @defgroup sntl_licgen_apis Sentinel LDK License Generation APIs
 *
 * @{
 */

/**
 * \brief  Initialize the license generator library and return a handle that is used for subsequent functions.
 *
 * <b>Usage Notes</b><br>
 * This should be the first function to be called when you use the License Generation APIs.
 * You can initialize up to 512 simultaneous license generator handles.
 *
 * <b>Warning:</b><br>
 * \li The handles returned by Sentinel LDK Run-time APIs and Sentinel
 *     LDK License Generation API cannot be used interchangeably.
 * \li The handles are not thread safe; multiple threads should not work on same handle simultaneously.
 *
 * \param init_param  [IN] Reserved for future use. Set this parameter to NULL.
 * \param handle      [OUT] Pointer to the resulting license generation handle.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_TOO_MANY_OPEN_HANDLES
 *    - \ref SNTL_LG_INTERNAL_ERROR
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_initialize
(
   const char        *init_param,
   sntl_lg_handle_t  *handle
);

/**
 * \brief  Start the license definition.
 *
 * <b>Usage Notes</b><br>
 * Use this function as the starting point to validate license definition or to generate licenses.
 * This function requires the Vendor Code. The Vendor Codes are stored in a VendorCodes directory in
 * your system.
 *
 * To generate a base certificate license, the fingerprint of the target machine is required. 
 * The fingerprint can be specified explicitly in the current_state parameter OR the fingerprint can be extracted 
 * from the C2V of a provisional certificate.
 *
 * For information on how to obtain the machine fingerprint OR 
 * how to get the C2V of a provisional certificate, refer to the sntl_get_info function in the Run-time API help file.
 *
 * \param handle             [IN] License generation handle.
 * \param start_param        [IN] Reserved for future use. Set this parameter to NULL.
 * \param vendor_code        [IN] Pointer to the Vendor Code.
 * \param license_type       [IN] Type of license.
 * \param license_definition [IN] Definition of the license to be introduced at the start.
 *                                 Pass a NULL if you do not want to introduce any
 *                                 license definition here.
 * \param current_state      [IN] The current state of the target license store. It
 *                                 can be one of the following: contents of a C2V file, resultant key state, fingerprint, or NULL.
 *                                 <br>
 *                                 The resultant key state is the value assigned to the 'resultant_state' parameter by the \ref sntl_lg_generate_license() function.
 *                                 <br>
 *                                 Fingerprint will be used for generating the licenses for SL-AdminMode or SL-UserMode keys.
 *                                 <br>
 *                                 The state can be set to NULL to validate an input license definition without
 *                                 generating a license or to get the list of target devices capable of consuming the license definition.
 *                                 <br>
 *                                 To generate a provisional license, 'current_state' parameter must be set to NULL.
 *                                 <br>
 *                                 To generate a generic update license, 'current_state' parameter must be set to NULL.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INVALID_VENDOR_CODE
 *    - \ref SNTL_LG_INVALID_KEY_CURRENT_STATE
 *    - \ref SNTL_LG_NOT_SUPPORTED_KEY_TYPE
 *    - \ref SNTL_LG_INVALID_LICENSE_DEFINITION
 *    - \ref SNTL_LG_INVALID_SCHEMA_VERSION 
 *    - \ref SNTL_LG_DAYS_TO_EXPIRATION_NOT_SUPPORTED
 *    - \ref SNTL_LG_EXPIRY_DATE_NOT_SUPPORTED
 *    - \ref SNTL_LG_UNLIMITED_CONCURRENCY_COUNT_NOT_SUPPORTED
 *    - \ref SNTL_LG_CONCURRENCY_NOT_SUPPORTED
 *    - \ref SNTL_LG_TOO_BIG_RO_MEM_DATA
 *    - \ref SNTL_LG_TOO_BIG_RW_MEM_DATA
 *    - \ref SNTL_LG_EXCEEDING_MAX_CONCURRENCY_COUNT_LIMIT
 *    - \ref SNTL_LG_EXCEEDING_MAX_EXECUTION_COUNT_LIMIT
 *    - \ref SNTL_LG_EXCEEDING_MAX_EXPIRY_DATE_LIMIT
 *    - \ref SNTL_LG_BELOW_MIN_EXPIRY_DATE_LIMIT
 *    - \ref SNTL_LG_MASTER_KEY_CONNECT_ERROR
 *    - \ref SNTL_LG_MASTER_KEY_IO_ERROR
 *    - \ref SNTL_LG_MASTER_KEY_ACCESS_ERROR
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *    - \ref SNTL_LG_CLONE_DETECTED
 *    - \ref SNTL_LG_OLD_FIRMWARE
 *    - \ref SNTL_LG_DYNAMIC_RESOURCE_NOT_SUPPORTED
 *    - \ref SNTL_LG_CLEAR_CLONE_NOT_ALLOWED
 *    - \ref SNTL_LG_LICENSE_DEFINITION_NOT_ALLOWED
 *          
 * \sa sntl_lg_initialize()
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_start
(
   sntl_lg_handle_t        handle,
   const char             *start_param,
   sntl_lg_vendor_code_t   vendor_code,
   sntl_lg_license_type_t  license_type,
   const char             *license_definition,
   const char             *current_state
);

/**
 * \brief  Apply license definition to the license state associated with the handle.
 *
 * <b>Usage Notes</b><br>
 *
 * Call this function after \ref sntl_lg_start(). You can call this function as many times as
 * needed in order to introduce changes in the license state through license definition parameters.
 *
 * This function validates the input license definition with respect to the
 * current state of the license associated with the handle. Only a valid license definition
 * will be applied to the license state associated with the handle.
 *
 * \param handle             [IN] License generation handle.
 * \param license_definition [IN] Definition of the license to be introduced.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INVALID_API_CALL_SEQUENCE
 *    - \ref SNTL_LG_INVALID_LICENSE_DEFINITION
 *    - \ref SNTL_LG_INVALID_SCHEMA_VERSION 
 *    - \ref SNTL_LG_DAYS_TO_EXPIRATION_NOT_SUPPORTED
 *    - \ref SNTL_LG_EXPIRY_DATE_NOT_SUPPORTED
 *    - \ref SNTL_LG_UNLIMITED_CONCURRENCY_COUNT_NOT_SUPPORTED
 *    - \ref SNTL_LG_CONCURRENCY_NOT_SUPPORTED
 *    - \ref SNTL_LG_TOO_BIG_RO_MEM_DATA
 *    - \ref SNTL_LG_TOO_BIG_RW_MEM_DATA
 *    - \ref SNTL_LG_EXCEEDING_MAX_CONCURRENCY_COUNT_LIMIT
 *    - \ref SNTL_LG_EXCEEDING_MAX_EXECUTION_COUNT_LIMIT
 *    - \ref SNTL_LG_EXCEEDING_MAX_EXPIRY_DATE_LIMIT
 *    - \ref SNTL_LG_BELOW_MIN_EXPIRY_DATE_LIMIT
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *    - \ref SNTL_LG_OLD_FIRMWARE
 *    - \ref SNTL_LG_DYNAMIC_RESOURCE_NOT_SUPPORTED
 *
 * \sa sntl_lg_initialize()
 * \sa sntl_lg_start()
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_apply_template
(
   sntl_lg_handle_t   handle,
   const char        *license_definition
);


/**
 * \brief  Decodes the input current state.
 *
 * <b>Usage Notes</b><br>
 * This function decodes the input current state and output the decoded information as xml buffer.
 * Input current state can be C2V or Resultant State (returned from \ref sntl_lg_generate_license() function) or fingerprint.
 * This function requires the Vendor Code. The Vendor Codes are stored in a VendorCodes directory in
 * your system.
 *
 * \param handle             [IN] License generation handle. 
 * \param vendor_code        [IN] Pointer to the Vendor Code. 
 * \param current_state      [IN] The current state. It can be contents of a C2V file or resultant key state or fingerprint.
 *                                 <br>
 *                                 The resultant key state is the value assigned to the 'resultant_state' parameter by the \ref sntl_lg_generate_license() function.
 *                                 <br>
 *
 * \param readable_state     [OUT] Pointer to the decoded information.
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INVALID_VENDOR_CODE
 *    - \ref SNTL_LG_INVALID_KEY_CURRENT_STATE
 *    - \ref SNTL_LG_NOT_SUPPORTED_KEY_TYPE
 *    - \ref SNTL_LG_MASTER_KEY_CONNECT_ERROR
 *    - \ref SNTL_LG_MASTER_KEY_IO_ERROR
 *    - \ref SNTL_LG_MASTER_KEY_ACCESS_ERROR
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *
 * \sa sntl_lg_initialize()
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_decode_current_state
(
   sntl_lg_handle_t        handle,
   sntl_lg_vendor_code_t   vendor_code,
   const char              *current_state,
   char                    **readable_state
);

/**
 * \brief  Generate the license.
 *
 * <b>Usage Notes</b><br>
 *
 * The resultant state is the expected final state (in XML format) on the end 
 * user machine after successful application of license. This state is the result 
 * of the current state passed at the time of \ref sntl_lg_start() together with 
 * the license definition that was applied up to this point using \ref sntl_lg_start() 
 * and \ref sntl_lg_apply_template().
 *
 * This function allocates memory for the information it retrieves. To
 * release allocated memory resources, use the \ref sntl_lg_free_memory() function.
 *
 * To reuse the handle again for license definition validation or license generation,
 * call the \ref sntl_lg_start() function with the same handle. The \ref sntl_lg_start() 
 * function frees all the resources acquired for the handle and allows license generation 
 * on the same handle without calling \ref sntl_lg_initialize() function.
 *
 * This API does not update the key. For information on how to update the key, refer 
 * to the sntl_update function in the Run-time API help file.
 *
 * \param handle           [IN] License generation handle.
 * \param generation_param [IN] Reserved for future use. Set this parameter to NULL.
 * \param license          [OUT] Pointer to the license that is retrieved.
 * \param resultant_state  [OUT] Pointer to the resultant state. If 'license_type' is specified
 *                               as \ref SNTL_LG_LICENSE_TYPE_PROVISIONAL or \ref SNTL_LG_LICENSE_TYPE_GENERIC_UPDATE
 *                               in \ref sntl_lg_start() function, then it will be returned as NULL. 
 *                               If the resultant state is not required, a value of NULL can be passed.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INVALID_API_CALL_SEQUENCE
 *    - \ref SNTL_LG_INVALID_LICENSE_DEFINITION
 *    - \ref SNTL_LG_KEY_FULL
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *    - \ref SNTL_LG_KEY_CURRENT_STATE_MISSING
 *    - \ref SNTL_LG_INSUFFICIENT_SEAT_POOL
 *    - \ref SNTL_LG_MASTER_FEATURE_NOT_FOUND
 *    - \ref SNTL_LG_MASTER_FEATURE_EXPIRED
 *    - \ref SNTL_LG_MASTER_NO_BATTERY_POWER
 *    - \ref SNTL_LG_NOTHING_TO_GENERATE
 *    - \ref SNTL_LG_DYNAMIC_RESOURCE_NOT_SUPPORTED
 *    - \ref SNTL_LG_LICENSE_DENIED_ON_VM
 *
 * \sa sntl_lg_initialize()
 * \sa sntl_lg_start()
 * \sa sntl_lg_apply_template()
 *
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_generate_license
(
   sntl_lg_handle_t       handle,
   const char            *generation_param,
   char                 **license,
   char                 **resultant_state
);

/**
 * \brief  Retrieve information associated with a handle.
 *
 * <b>Usage Notes</b><br>
 * This function retrieves information associated with a handle, such as
 * capable device information.
 *
 * <b>Capable device information</b>
 * <p>
 * Possible key types are as follows:
 *      \li HL-NetTime-250+
 *      \li HL-NetTime-50
 *      \li HL-NetTime-10
 *      \li HL-Net-250+
 *      \li HL-Net-50
 *      \li HL-Net-10
 *      \li HL-Time
 *      \li HL-Max
 *      \li HL-Max-Micro
 *      \li HL-Drive
 *      \li HL-Express-Card
 *      \li HL-Pro
 *      \li Sentinel-HL-NetTime-250+
 *      \li Sentinel-HL-NetTime-50
 *      \li Sentinel-HL-NetTime-10
 *      \li Sentinel-HL-Net-250+
 *      \li Sentinel-HL-Net-50
 *      \li Sentinel-HL-Net-10
 *      \li Sentinel-HL-Time
 *      \li Sentinel-HL-Max
 *      \li Sentinel-HL-Max-Micro
 *      \li Sentinel-HL-Drive
 *      \li Sentinel-HL-Express-Card
 *      \li Sentinel-HL-Max-Chip
 *      \li Sentinel-HL-Max-Board
 *      \li Sentinel-HL-Pro
 *      \li SL-AdminMode
 *      \li SL-UserMode
 * </p>
 * <p>
 * The following is a sample output format:
 \verbatim
 <?xml version="1.0" encoding="UTF-8"?>
 <sentinel_ldk_info>
   <capable_devices>
      <key_type>SL-AdminMode</key_type>
      <key_type configuration="hasphl">HL-NetTime-50</key_type>
      <key_type configuration="sentinelhl, hasphl">Sentinel HL-NetTime-50</key_type>
      <key_type configuration="sentinelhl, driverless">Sentinel-HL-Time</key_type>
      ...
      ...
      ...
   </capable_devices>
 </sentinel_ldk_info>
 \endverbatim
 * </p>
 *
 * This function allocates memory for the information it retrieves. To
 * release allocated memory resources, use the \ref sntl_lg_free_memory() function.
 *
 * \param handle     [IN]  License generation handle.
 * \param info_type  [IN]  Type of information required.
 * \param info       [OUT] Pointer to the information retrieved.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_OUT_OF_MEMORY
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_get_info
(
   sntl_lg_handle_t        handle,
   sntl_lg_info_type_t     info_type,
   char                    **info
);

/**
 * \brief Releases memory resources utilized by other License Generation APIs.
 *
 * <b>Usage Notes</b><br>
 * This function releases memory resources that were allocated to store retrieved data
 * from License Generation API calls. (This function does not release resources allocated by Sentinel LDK Run-time APIs.)
 *
 * \param memory_reference [INOUT] Pointer to the memory resources to be released.
 *
 * \sa sntl_lg_generate_license()
 *
 */
void SNTL_LG_CALLCONV sntl_lg_free_memory
(
   void  *memory_reference
);

/**
 * \brief  Clean up handle.
 *
 * <b>Usage Notes</b><br>
 * This function releases all the resources associated with a handle and
 * resets the handle value to \ref SNTL_LG_INVALID_HANDLE_VALUE after successful cleanup.
 *
 * \param handle [INOUT] License generation handle to be released.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *    - \ref SNTL_LG_LIB_INITIALIZATION_FAILED
 *    - \ref SNTL_LG_INVALID_HANDLE
 *    - \ref SNTL_LG_RESOURCE_LOCK_FAILURE
 *    - \ref SNTL_LG_INVALID_PARAMETER
 *    - \ref SNTL_LG_INTERNAL_ERROR
 *
 * \sa sntl_lg_initialize()
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_cleanup
(
   sntl_lg_handle_t  *handle
);

/**
 * \brief Retrieve version and build number of the LicGen library
 *
 * \param major_version [OUT] Pointer to retrieve the major version number.
 * \param minor_version [OUT] Pointer to retrieve the minor version number.
 * \param build_server  [OUT] Pointer to retrieve the build server id.
 * \param build_number  [OUT] Pointer to retrieve the build number.
 *
 * \return
 *    - \ref SNTL_LG_STATUS_OK
 *
 * \remark Any pointer can be NULL if its information is not required.
 *
 */
sntl_lg_status_t SNTL_LG_CALLCONV sntl_lg_get_version
(
   unsigned int *major_version,
   unsigned int *minor_version,
   unsigned int *build_server,
   unsigned int *build_number
);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SNTL_LICGEN_H__ */
