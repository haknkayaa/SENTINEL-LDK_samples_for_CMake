// Copyright (C) 2021 Thales Group. All rights reserved.

#include "hasp_api_cpp.h"
#include "errorprinter.h"
#include <iostream>

using namespace std;

ErrorPrinter::ErrorPrinter()
{
    errorMap.insert(StatusMap::value_type(HASP_STATUS_OK, "Operation completed successfully"));
    errorMap.insert(StatusMap::value_type(HASP_MEM_RANGE, "Invalid memory address"));
    errorMap.insert(StatusMap::value_type(HASP_INV_PROGNUM_OPT, "Unknown/invalid Feature ID option"));
    errorMap.insert(StatusMap::value_type(HASP_INSUF_MEM, "Memory allocation failed"));
    errorMap.insert(StatusMap::value_type(HASP_TMOF, "Too many open Features"));
    errorMap.insert(StatusMap::value_type(HASP_ACCESS_DENIED, "Feature access denied"));
    errorMap.insert(StatusMap::value_type(HASP_INCOMPAT_FEATURE, "Incompatible Feature"));
    errorMap.insert(StatusMap::value_type(HASP_HASP_NOT_FOUND, "HASP Key not found"));
    errorMap.insert(StatusMap::value_type(HASP_TOO_SHORT, "Encryption/decryption length too short"));
    errorMap.insert(StatusMap::value_type(HASP_INV_HND, "Invalid handle"));
    errorMap.insert(StatusMap::value_type(HASP_INV_FILEID, "Invalid file ID / memory descriptor"));
    errorMap.insert(StatusMap::value_type(HASP_OLD_DRIVER, "Driver or support daemon version too old"));
    errorMap.insert(StatusMap::value_type(HASP_NO_TIME, "Real time support not available"));
    errorMap.insert(StatusMap::value_type(HASP_SYS_ERR, "Generic error from host system call"));
    errorMap.insert(StatusMap::value_type(HASP_NO_DRIVER, "HASP driver not found"));
    errorMap.insert(StatusMap::value_type(HASP_INV_FORMAT, "Unrecognized info format"));
    errorMap.insert(StatusMap::value_type(HASP_REQ_NOT_SUPP, "Request not supported"));
    errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_OBJ, "Invalid update object"));
    errorMap.insert(StatusMap::value_type(HASP_KEYID_NOT_FOUND, "Key with specified ID was not found"));
    errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_DATA, "Update data consistency check failed"));
    errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_NOTSUPP, "Update not supported by this key"));
    errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_CNTR, "Update counter mismatch"));
    errorMap.insert(StatusMap::value_type(HASP_INV_VCODE, "Invalid Vendor Code"));
    errorMap.insert(StatusMap::value_type(HASP_ENC_NOT_SUPP, "Requested encryption algorithm not supported"));
    errorMap.insert(StatusMap::value_type(HASP_INV_TIME, "Invalid date/time"));
    errorMap.insert(StatusMap::value_type(HASP_NO_BATTERY_POWER, "Clock has no power"));
    errorMap.insert(StatusMap::value_type(HASP_NO_ACK_SPACE, "Update requested ack., but no area to return it"));
    errorMap.insert(StatusMap::value_type(HASP_TS_DETECTED, "Terminal services (remote terminal) detected"));
    errorMap.insert(StatusMap::value_type(HASP_FEATURE_TYPE_NOT_IMPL, "Feature type not implemented"));
    errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_ALG, "Unknown algorithm"));
    errorMap.insert(StatusMap::value_type(HASP_INV_SIG, "Signature check failed"));
    errorMap.insert(StatusMap::value_type(HASP_FEATURE_NOT_FOUND, "Feature not found" ));
    errorMap.insert(StatusMap::value_type(HASP_NO_LOG, "Trace log is not enabled" ));
    errorMap.insert(StatusMap::value_type(HASP_LOCAL_COMM_ERR, "Communication error between application and local LM" ));
    errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_VCODE, "Vendor Code not recognized by API)" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_SPEC, "Invalid XML spec" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_SCOPE, "Invalid XML scope" ));
    errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_KEYS, "Too many keys connected" ));
    errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_USERS, "Too many users" ));
    errorMap.insert(StatusMap::value_type(HASP_BROKEN_SESSION, "Broken session" ));
    errorMap.insert(StatusMap::value_type(HASP_REMOTE_COMM_ERR, "Communication error between local and remote License Manager" ));
    errorMap.insert(StatusMap::value_type(HASP_FEATURE_EXPIRED, "The feature is expired" ));
    errorMap.insert(StatusMap::value_type(HASP_OLD_LM, "HASP LM version is too old" ));
    errorMap.insert(StatusMap::value_type(HASP_DEVICE_ERR, "HASP SL secure storage I/O error or USB request error" ));
    errorMap.insert(StatusMap::value_type(HASP_UPDATE_BLOCKED, "Update installation not allowed" ));
    errorMap.insert(StatusMap::value_type(HASP_TIME_ERR, "System time has been tampered" ));
    errorMap.insert(StatusMap::value_type(HASP_SCHAN_ERR, "Secure channel communication error" ));
    errorMap.insert(StatusMap::value_type(HASP_STORAGE_CORRUPT, "Secure storage contains garbage" ));
    errorMap.insert(StatusMap::value_type(HASP_NO_VLIB, "Vendor lib cannot be found" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_VLIB, "Vendor lib cannot be loaded" ));
    errorMap.insert(StatusMap::value_type(HASP_SCOPE_RESULTS_EMPTY, "No feature matching scope found" ));
    errorMap.insert(StatusMap::value_type(HASP_VM_DETECTED, "Virtual machine detected" ));
    errorMap.insert(StatusMap::value_type(HASP_HARDWARE_MODIFIED, "HASP update incompatible with this hardware; HASP key is locked to other hardware" ));
    errorMap.insert(StatusMap::value_type(HASP_USER_DENIED, "Login denied because of user restrictions" ));
    errorMap.insert(StatusMap::value_type(HASP_UPDATE_TOO_OLD, "Update was already installed" ));
    errorMap.insert(StatusMap::value_type(HASP_UPDATE_TOO_NEW, "Another update must be installed first" ));
    errorMap.insert(StatusMap::value_type(HASP_OLD_VLIB, "Vendor lib is too old" ));
    errorMap.insert(StatusMap::value_type(HASP_UPLOAD_ERROR, "Upload via ACC failed, e.g. because of illegal format" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_RECIPIENT, "Invalid XML \"recipient\" parameter" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_DETACH_ACTION, "Invalid XML \"action\" parameter" ));
    errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_PRODUCTS, "Scope does not specify a unique Product" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_PRODUCT, "Invalid Product information" ));
    errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_RECIPIENT, "Unknown Recipient; update can only be applied to the Recipient specified in hasp_detach(), and not to this computer" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_DURATION, "Invalid duration specified" ));
    errorMap.insert(StatusMap::value_type(HASP_CLONE_DETECTED, "Cloned HASP SL secure storage detected" ));
	errorMap.insert(StatusMap::value_type(HASP_UPDATE_ALREADY_ADDED, "Specified V2C update already installed in the LLM" ));
	errorMap.insert(StatusMap::value_type(HASP_HASP_INACTIVE, "Specified Hasp Id is in Inactive state" ));
	errorMap.insert(StatusMap::value_type(HASP_NO_DETACHABLE_FEATURE, "No detachable feature exists" ));
	errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_HOSTS, "Scope does not specify a unique host" ));
	errorMap.insert(StatusMap::value_type(HASP_REHOST_NOT_ALLOWED, "Rehost is not allowed for any license" ));
	errorMap.insert(StatusMap::value_type(HASP_LICENSE_REHOSTED, "License is rehosted to other machine" ));
	errorMap.insert(StatusMap::value_type(HASP_REHOST_ALREADY_APPLIED, "Old rehost license try to apply" ));
	errorMap.insert(StatusMap::value_type(HASP_CANNOT_READ_FILE, "File not found or access denied" ));
	errorMap.insert(StatusMap::value_type(HASP_EXTENSION_NOT_ALLOWED, "Detach extension is not allowed to change the network property of the detached license, or to decrease the number of the detached network seats" ));
	errorMap.insert(StatusMap::value_type(HASP_DETACH_DISABLED, "Detach of license not allowed as product contains VM disabled feature and host machine is a virtual machine" ));
	errorMap.insert(StatusMap::value_type(HASP_REHOST_DISABLED, "Rehost of license not allowed as container contains VM disabled feature and host machine is a virtual machine" ));
	errorMap.insert(StatusMap::value_type(HASP_DETACHED_LICENSE_FOUND, "Format SL-AdminMode or migrate SL-Legacy to SL-AdminMode not allowed as container has detached license" ));
	errorMap.insert(StatusMap::value_type(HASP_RECIPIENT_OLD_LM, "Recipient of the requested operation is older than expected" ));
	errorMap.insert(StatusMap::value_type(HASP_SECURE_STORE_ID_MISMATCH, "Secure storage ID mismatch" ));
	errorMap.insert(StatusMap::value_type(HASP_DUPLICATE_HOSTNAME, "Duplicate Hostname found while key contains Hostname Fingerprinting" ));
	errorMap.insert(StatusMap::value_type(HASP_MISSING_LM, "The Sentinel License Manager is required for this operation" ));
	errorMap.insert(StatusMap::value_type(HASP_FEATURE_INSUFFICIENT_EXECUTION_COUNT, "You are attempting to consume multiple executions during log in to a Feature" ));
	errorMap.insert(StatusMap::value_type(HASP_INCOMPATIBLE_PLATFORM, "You are attempting to perform an operation not compatible with target platform" ));
	errorMap.insert(StatusMap::value_type(HASP_HASP_DISABLED, "The key is disabled due to suspected tampering" ));
	errorMap.insert(StatusMap::value_type(HASP_SHARING_VIOLATION, "The key is inaccessible due to sharing" ));
	errorMap.insert(StatusMap::value_type(HASP_KILLED_SESSION, "The session was killed due a network malfunction or manually from ACC" ));
	errorMap.insert(StatusMap::value_type(HASP_VS_DETECTED, "Program running on a virtual storage" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_REQUIRED, "An identity is required" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_UNAUTHENTICATED, "The identity is not authenticated" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_DISABLED, "The identity is disabled" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_DENIED, "The identity doesn't have enough permission for the operation" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_SHARING_VIOLATION, "A session for this identity from a different machine already exists" ));
	errorMap.insert(StatusMap::value_type(HASP_IDENTITY_TOO_MANY_MACHINES, "The maximum number of machines usable by the identity was reached" ));
    errorMap.insert(StatusMap::value_type(HASP_IDENTITY_SERVER_NOT_READY, "The server is not ready to authenticate"));
    errorMap.insert(StatusMap::value_type(HASP_UPDATE_OUT_OF_SYNC, "Trying to install a V2C file with an update counter that is out of sync with update counter in the Sentinel protection key."));
    errorMap.insert(StatusMap::value_type(HASP_REMOTE_SHARING_VIOLATION, "Multiple attempts to access the key from remote with a proxy"));
    errorMap.insert(StatusMap::value_type(HASP_CLOUD_SESSION_OCCUPIED_REMOTELY, "The session was released because the seat was requested from a different location"));
    errorMap.insert(StatusMap::value_type(HASP_CLOUD_MISSING_AUTHORIZATION, "Cloud licensing authorization is required to use this license"));
    errorMap.insert(StatusMap::value_type(HASP_INV_NETWORK_SEATS, "Invalid seat value in network detach. Seat count cannot be decreased when modifying a detach"));
    errorMap.insert(StatusMap::value_type(HASP_NETWORK_DETACH_DISABLED, "Network detach is disabled on products with only unlimited concurrency features"));
    errorMap.insert(StatusMap::value_type(HASP_CLOUD_NOT_SUPP, "The requested cloud functionality is not supported"));
    errorMap.insert(StatusMap::value_type(HASP_CLOUD_NOT_TRUSTED, "Only trusted licenses can be installed in the trusted license storage"));
    errorMap.insert(StatusMap::value_type(HASP_CLOUD_STORAGE_COMM_ERR, "Communication error with the license storage"));
    errorMap.insert(StatusMap::value_type(HASP_NO_API_DYLIB, "A required API dynamic library was not found" ));
    errorMap.insert(StatusMap::value_type(HASP_INV_API_DYLIB, "The found and assigned API dynamic library could not verified" ));
    errorMap.insert(StatusMap::value_type(HASP_INCOMPLETE_API_DYLIB, "Unable to find API function"));
    errorMap.insert(StatusMap::value_type(HASP_INVALID_OBJECT, "Object incorrectly initialized" ));
    errorMap.insert(StatusMap::value_type(HASP_INVALID_PARAMETER, "Invalid function parameter" ));
    errorMap.insert(StatusMap::value_type(HASP_ALREADY_LOGGED_IN, "Logging in twice to the same object" ));
    errorMap.insert(StatusMap::value_type(HASP_ALREADY_LOGGED_OUT, "Logging out twice from the same object" ));
    errorMap.insert(StatusMap::value_type(HASP_OPERATION_FAILED, "Incorrect use of system or platform"));
    errorMap.insert(StatusMap::value_type(HASP_NO_EXTBLOCK, "No classic memory extension block available" ));
	
	errorMap.insert(StatusMap::value_type(HASP_INV_PORT_TYPE, "Invalid port type" ));
	errorMap.insert(StatusMap::value_type(HASP_INV_PORT, "Invalid port value" ));
    errorMap.insert(StatusMap::value_type(HASP_NET_DLL_BROKEN, "Dot-Net DLL found broken"));
    errorMap.insert(StatusMap::value_type(HASP_NOT_IMPL, "Capability isn't available" ));
	
    errorMap.insert(StatusMap::value_type(HASP_INT_ERR, "Internal API error" ));
	
    errorMap.insert(StatusMap::value_type(HASP_FIRST_HELPER, "Reserved for Sentinel helper libraries" ));
	
    errorMap.insert(StatusMap::value_type(HASP_FIRST_HASP_ACT, "Reserved for Sentinel Activation API" ));
}

void ErrorPrinter::printError(haspStatus status)
{
    StatusMap::const_iterator error = errorMap.find(status);
    if(error == errorMap.end())
        cout<<"unknown error: "<<status<<endl;
    else
        cout<<error->second<<endl;
}

const char* ErrorPrinter::getError(haspStatus status)
{
    StatusMap::const_iterator error = errorMap.find(status);
    if(error == errorMap.end())
        return "Unknown error";
    else
        return error->second;
}
