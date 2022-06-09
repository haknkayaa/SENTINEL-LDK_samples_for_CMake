/*
 *  
 *
 *  Copyright (C) 2022 Thales Group. All rights reserved.
 *  Use is subject to license terms.
 */

package Aladdin;

public class HaspStatus
{
    /**
     * return codes
     */

    /**
     * Request successfully completed
     */
    public static final int HASP_STATUS_OK              = 0;

    /**
     * Request exceeds memory range of a Sentinel file
     */
    public static final int HASP_MEM_RANGE              = 1;

    /**
     * Legacy HASP HL Run-time API: Unknown/Invalid Feature ID option
     */
    public static final int HASP_INV_PROGNUM_OPT        = 2;

    /**
     * System is out of memory
     */
    public static final int HASP_INSUF_MEM              = 3;

    /**
     * Too many open Features/login sessions
     */
    public static final int HASP_TMOF                   = 4;

    /**
     * Access to Feature, Sentinel protection key or functionality denied
     */
    public static final int HASP_ACCESS_DENIED          = 5;

    /**
     * Legacy decryption function cannot work on Feature
     */
    public static final int HASP_INCOMPAT_FEATURE       = 6;

    /**
     * Sentinel protection key not available
     */
    public static final int HASP_HASP_NOT_FOUND         = 7;

    /**
     * Deprecated - use HASP_HASP_NOT_FOUND
     */
    public static final int HASP_CONTAINER_NOT_FOUND    = 7;

    /**
     * Encrypted/decrypted data length too short to execute function call
     */
    public static final int HASP_TOO_SHORT              = 8;

    /**
     * Invalid login handle passed to function
     */
    public static final int HASP_INV_HND                = 9;

    /**
     * Specified File ID not recognized by API 
     */
    public static final int HASP_INV_FILEID             = 10;

    /**
     * Installed driver or daemon too old to execute function
     */
    public static final int HASP_OLD_DRIVER             = 11;

    /**
     * Real-time clock (rtc) not available
     */
    public static final int HASP_NO_TIME                = 12;

    /**
     * Generic error from host system call
     */
    public static final int HASP_SYS_ERR                = 13;

    /**
     * Required driver not installed 
     */
    public static final int HASP_NO_DRIVER              = 14;

    /**
     * Invalid XML format
     */
    public static final int HASP_INV_FORMAT             = 15;

    /**
     * Unable to execute function in this context; the requested
     * functionality is not implemented 
     */
    public static final int HASP_REQ_NOT_SUPP           = 16;

    /**
     * Binary data passed to function does not contain valid update 
     */
    public static final int HASP_INV_UPDATE_OBJ         = 17;

    /**
     * Sentinel protection key not found
     */
    public static final int HASP_KEYID_NOT_FOUND        = 18;

    /**
     * Required XML tags not found; Contents in binary data are missing
     * or invalid 
     */
    public static final int HASP_INV_UPDATE_DATA        = 19;

    /**
     * Update request not supported by Sentinel protection key
     */
    public static final int HASP_INV_UPDATE_NOTSUPP     = 20;

    /**
     * Update counter set incorrectly
     */
    public static final int HASP_INV_UPDATE_CNTR        = 21;

    /**
     * Invalid Vendor Code passed
     */
    public static final int HASP_INV_VCODE              = 22;

    /**
     * Sentinel protection key does not support encryption type
     */
    public static final int HASP_ENC_NOT_SUPP           = 23;

    /**
     * Passed time value outside supported value range
     */
    public static final int HASP_INV_TIME               = 24;

    /**
     * Real-time clock battery out of power
     */
    public static final int HASP_NO_BATTERY_POWER       = 25;

    /**
     * Acknowledge data requested by update, but ack_data parameter
     * is NULL 
     */
    public static final int HASP_NO_ACK_SPACE           = 26;

    /**
     * Program running on a terminal server
     */
    public static final int HASP_TS_DETECTED            = 27;

    /**
     * Requested Feature type not implemented
     */
    public static final int HASP_FEATURE_TYPE_NOT_IMPL  = 28;

    /**
     * Unknown algorithm used in H2R/V2C file
     */
    public static final int HASP_UNKNOWN_ALG            = 29;

    /**
     * Signature verification operation failed
     */
    public static final int HASP_INV_SIG                = 30;

    /**
     * Requested Feature not available
     */
    public static final int HASP_FEATURE_NOT_FOUND      = 31;

    /**
     * Access log not enabled
     */
    public static final int HASP_NO_LOG                 = 32;

    /**
     * Communication error between API and local Sentinel License Manager
     */
    public static final int HASP_LOCAL_COMM_ERR         = 33;

    /**
     * Vendor Code not recognized by API
     */
    public static final int HASP_UNKNOWN_VCODE          = 34;

    /**
     * Invalid XML specification
     */
    public static final int HASP_INV_SPEC               = 35;

    /**
     * Invalid XML scope
     */
    public static final int HASP_INV_SCOPE              = 36;

    /**
     * Too many Sentinel HASP protection keys currently connected
     */
    public static final int HASP_TOO_MANY_KEYS          = 37;

    /**
     * Too many concurrent user sessions currently connected
     */
    public static final int HASP_TOO_MANY_USERS         = 38;

    /**
     * Session has been interrupted
     */
    public static final int HASP_BROKEN_SESSION         = 39;

    /**
     * Communication error between local and remote HASP License Manager
     */
    public static final int HASP_REMOTE_COMM_ERR        = 40;

    /**
     * Feature expired
     */
    public static final int HASP_FEATURE_EXPIRED        = 41;

    /**
     * HASP License Manager version too old
     */
    public static final int HASP_OLD_LM                 = 42;

    /**
     * Input/Output error occurred in secure storage area of HASP SL key OR
     * a USB error occurred when communicating with a HASP HL key
     */
    public static final int HASP_DEVICE_ERR             = 43;

    /**
     * Update installation not permitted; This update was already applied
     */
    public static final int HASP_UPDATE_BLOCKED         = 44;

    /**
     * System time has been tampered with
     */
    public static final int HASP_TIME_ERR               = 45;

    /**
     * Communication error occurred in secure channel
     */
    public static final int HASP_SCHAN_ERR              = 46;

    /**
     * Corrupt data exists in secure storage area of HASP SL protection key
     */
    public static final int HASP_STORAGE_CORRUPT        = 47;

    /**
     * Unable to find Vendor library
     */
    public static final int HASP_NO_VLIB                = 48;

    /**
     * Unable to load Vendor library
     */
    public static final int HASP_INV_VLIB               = 49;

    /**
     * Unable to locate any Features matching scope
     */
    public static final int HASP_SCOPE_RESULTS_EMPTY    = 50;

    /**
     * Program running on a virtual machine
     */
    public static final int HASP_VM_DETECTED            = 51;

    /**
     * HASP SL key incompatible with machine hardware; HASP SL key is locked
     * to different hardware. OR:
     * In the case of a V2C file, conflict between HASP SL key data and machine
     * hardware data; HASP SL key locked to different hardware
     */
    public static final int HASP_HARDWARE_MODIFIED      = 52;

    /**
     * Login denied because of user restrictions
     */
    public static final int HASP_USER_DENIED            = 53;

    /**
     * Trying to install a V2C file with an update counter that is out of
     * sequence with the update counter on Sentinel HASP protection key. The
     * update counter value in the V2C file is lower than the value in Sentinel
     * HASP protection key.
     */
    public static final int HASP_UPDATE_TOO_OLD         = 54;

    /**
     * Trying to install a V2C file with an update counter that is out of
     * sequence with update counter in Sentinel HASP protection key. The first
     * value in the V2C file is greater than the value in Sentinel HASP
     * protection key.
     */
    public static final int HASP_UPDATE_TOO_NEW         = 55;

    /**
     * Vendor library version too old
     */
    public static final int HASP_OLD_VLIB               = 56;

    /**
     * Upload via ACC failed, e.g. because of illegal format
     */
    public static final int HASP_UPLOAD_ERROR           = 57;

    /**
     * Invalid XML "recipient" parameter
     */
    public static final int HASP_INV_RECIPIENT          = 58;

    /**
     * Invalid XML "action" parameter
     */
    public static final int HASP_INV_DETACH_ACTION      = 59;

    /**
     * Scope does not specify a unique Product
     */
    public static final int HASP_TOO_MANY_PRODUCTS      = 60;

    /**
     * Invalid Product information
     */
    public static final int HASP_INV_PRODUCT            = 61;

    /**
     * Unknown Recipient; update can only be applied to the
     * Recipient specified in detach(), and not to this computer
     */
    public static final int HASP_UNKNOWN_RECIPIENT      = 62;

    /**
     * Invalid duration
     */
    public static final int HASP_INV_DURATION           = 63;

    /**
     * Cloned secure storage area detected
     */
    public static final int HASP_CLONE_DETECTED         = 64;

    /**
     * Specified V2C update already installed in the LLM
     */
    public static final int HASP_UPDATE_ALREADY_ADDED   = 65;

    /**
     * Specified Hasp Id is in Inactive state
     */
    public static final int HASP_HASP_INACTIVE          = 66;

    /**
     * No detachable feature exists
     */
    public static final int HASP_NO_DETACHABLE_FEATURE  = 67;

    /**
     * No detachable feature exists (typo; kept for compatibility)
     */
    public static final int HASP_NO_DEATCHABLE_FEATURE  = 67;

    /**
     * scope does not specify a unique Host
     */
    public static final int HASP_TOO_MANY_HOSTS         = 68;

    /**
     * Rehost is not allowed for any license
     */
    public static final int HASP_REHOST_NOT_ALLOWED     = 69;

    /**
     * License is rehosted to other machine
     */
    public static final int HASP_LICENSE_REHOSTED       = 70;

    /**
     * Old rehost license try to apply
     */
    public static final int HASP_REHOST_ALREADY_APPLIED = 71;

    /**
     * File not found or access denied
     */
    public static final int HASP_CANNOT_READ_FILE       = 72;

    /**
     * Extension of license not allowed as number of detached
     * licenses is greater than current concurrency count
     */
    public static final int HASP_EXTENSION_NOT_ALLOWED  = 73;

    /**
     * Detach of license not allowed as product
     * contains VM disabled feature and host machine is a virtual machine
     */
    public static final int HASP_DETACH_DISABLED        = 74;

    /**
     * Rehost of license not allowed as container
     * contains VM disabled feature and host machine is a virtual machine
     */
    public static final int HASP_REHOST_DISABLED        = 75;

    /**
     * Format SL-AdminMode or migrate SL-Legacy to SL-AdminMode not allowed
     * as container has detached license
     */
    public static final int HASP_DETACHED_LICENSE_FOUND = 76;

    /**
     * Recipient of the requested operation is older than expected
     */
    public static final int HASP_RECIPIENT_OLD_LM       = 77;

    /**
     * Secure storage ID mismatch
     */
    public static final int HASP_SECURE_STORE_ID_MISMATCH = 78;

    /**
     * Duplicate Hostname found while key contains Hostname Fingerprinting
     */
    public static final int HASP_DUPLICATE_HOSTNAME     = 79;

    /**
     * The Sentinel License Manager is required for this operation
     */
    public static final int HASP_MISSING_LM             = 80;

    /**
     * Attempting to consume multiple executions during log in to a Feature.
     * However, the license for the Feature does not contain enough remaining executions
     */
    public static final int HASP_FEATURE_INSUFFICIENT_EXECUTION_COUNT = 81;

    /**
     * Attempting to perform an operation not compatible with target platform
     */
    public static final int HASP_INCOMPATIBLE_PLATFORM  = 82;

    /**
     * The key is disabled due to suspected tampering
     */
    public static final int HASP_HASP_DISABLED          = 83;

    /**
     * The key is inaccessible due to sharing
     */ 
    public static final int HASP_SHARING_VIOLATION      = 84;

    /**
     * The session was killed due a network malfunction or manually from ACC
     */
    public static final int HASP_KILLED_SESSION         = 85;

    /**
     * Program running on a virtual storage
     */
    public static final int HASP_VS_DETECTED            = 86;

    /**
     * An identity is required
     */
    public static final int HASP_IDENTITY_REQUIRED      = 87;

    /**
     * The identity is not authenticated
     */
    public static final int HASP_IDENTITY_UNAUTHENTICATED = 88;

    /**
     * The identity is disabled
     */
    public static final int HASP_IDENTITY_DISABLED      = 89;

    /**
     * The identity doesn't have enough permission for the operation
     */
    public static final int HASP_IDENTITY_DENIED        = 90;

    /**
     * A session for this identity from a different machine already exists
     */
    public static final int HASP_IDENTITY_SHARING_VIOLATION      = 91;

    /**
     * The maximum number of machines usable by the identity was reached
     */
    public static final int HASP_IDENTITY_TOO_MANY_MACHINES      = 92;

    /**
     * The server is not ready to authenticate
     */
    public static final int HASP_IDENTITY_SERVER_NOT_READY       = 93;

    /**
     * Trying to install a V2C file with an update counter that is out of
     * sync with update counter in the Sentinel protection key
     */
    public static final int HASP_UPDATE_OUT_OF_SYNC              = 94;

    /**
     * Multiple attemps to access the key from remote with a proxy
     */
    public static final int HASP_REMOTE_SHARING_VIOLATION        = 95;

    /**
     * The session was released because the seat was requested from a different location
     */
    public static final int HASP_CLOUD_SESSION_OCCUPIED_REMOTELY = 96;

    /**
     * Cloud licensing authorization is required to use this license
     */
    public static final int HASP_CLOUD_MISSING_AUTHORIZATION     = 97;

    /**
     * Invalid seat value in network detach. Seat count cannot be decreased when modifying a detach
     */
    public static final int HASP_INV_NETWORK_SEATS               = 98;

    /**
     * Network detach is disabled on products with only unlimited concurrency features
     */
    public static final int HASP_NETWORK_DETACH_DISABLED         = 99;

    /**
     * The requested cloud functionality is not supported
     */
    public static final int HASP_CLOUD_NOT_SUPP                  = 100;

    /**
     * Only trusted licenses can be installed in the trusted license storage
     */
    public static final int HASP_CLOUD_NOT_TRUSTED               = 101;

    /**
     * Communication error with the license storage
     */
    public static final int HASP_CLOUD_STORAGE_COMM_ERR          = 102;

    /**
     * The identity is expired
     */
    public static final int HASP_IDENTITY_EXPIRED                = 103;

    /**
     * Invalid option value
     */
    public static final int HASP_INV_OPTION                      = 104;

    /**
     * Busy error with the license storage
     */
    public static final int HASP_CLOUD_STORAGE_BUSY              = 105;

    /**
     * API dispatcher: API for this Vendor Code was not found
     */
    public static final int HASP_NO_API_DYLIB           = 400;

    /**
     * API dispatcher: Unable to load API; DLL possibly corrupt?
     */
    public static final int HASP_INV_API_DYLIB          = 401;

    /**
     * API dispatcher: Unable to find API function; DLL possibly old?
     */
    public static final int HASP_INCOMPLETE_API_DYLIB   = 402;

    /**
     * C++ API: Object incorrectly initialized
     */
    public static final int HASP_INVALID_OBJECT         = 500;

    /**
     * Invalid function parameter
     */
    public static final int HASP_INV_PARAM              = 501;

    /**
     * C++ API: Logging in twice to the same object
     */
    public static final int HASP_ALREADY_LOGGED_IN      = 502;

    /**
     * C++ API: Logging out twice of the same object
     */
    public static final int HASP_ALREADY_LOGGED_OUT     = 503;

    /**
     * .NET API: Incorrect use of system or platform
     */
    public static final int HASP_OPERATION_FAILED       = 525;

    /*
     * Internal use: no classic memory extension block available
     */
    public static final int HASP_NO_EXTBLOCK            = 600;

    /*
     * Internal use: invalid port type
     */
    public static final int HASP_INV_PORT_TYPE          = 650;

    /*
     * Internal use: invalid port value
     */
    public static final int HASP_INV_PORT               = 651;

    /*
     * Dot-Net DLL found broken
     */
    public static final int HASP_NET_DLL_BROKEN         = 652;

    /**
     * Requested function not implemented
     */
    public static final int HASP_NOT_IMPL               = 698;

    /**
     * Internal error occurred in API
     */
    public static final int HASP_INT_ERR                = 699;

    /*
     * Reserved for Sentinel helper libraries
     */
    public static final int HASP_FIRST_HELPER           = 2001;

    /*
     * Reserved for Sentinel Activation API
     */
    public static final int HASP_FIRST_HASP_ACT         = 3001;

    public static final int HASP_NEXT_FREE_VALUES       = 7001;

    public static String runtime_library_default     = "HASPJava";
    public static String runtime_library_x64_windows = "HASPJava_x64";
    public static String runtime_library_x64_linux   = "HASPJava_x86_64";
    public static String runtime_library_armhf_linux = "HASPJava_armhf";
    public static String runtime_library_arm64_linux = "HASPJava_arm64";

    public static void Init()
    {
        String operatingSystem = System.getProperty("os.name");
        String architecture = System.getProperty("os.arch");

        try
        {
            if (operatingSystem.indexOf("Windows") != -1)
            {
                if (architecture.equals("x86_64") || architecture.equals("amd64"))
                {
                    System.loadLibrary(runtime_library_x64_windows);
                }
                else
                {
                    System.loadLibrary(runtime_library_default);
                }
            }
            else if (operatingSystem.indexOf("Linux") != -1)
            {
                    if (architecture.equals("x86_64") || architecture.equals("amd64"))
                    {
                        System.loadLibrary(runtime_library_x64_linux);
                    }
                    else if (architecture.equals("arm"))
                    {
                        System.loadLibrary(runtime_library_armhf_linux);
                    }
                    else if (architecture.equals("aarch64"))
                    {
                        System.loadLibrary(runtime_library_arm64_linux);
                    }
                    else
                    {
                        System.loadLibrary(runtime_library_default);
                    }
            }
            else
            {
                System.loadLibrary(runtime_library_default);
            }
        }
        catch (UnsatisfiedLinkError e)
        {
            if (e.getMessage().indexOf("already loaded in another classloader") == -1)
            {
                throw e;
            }
        }
    }
}
