/* 
 *
 * HASPJava: Sentinel HASP Java native interface
 * Copyright (C) 2022 Thales Group. All rights reserved.
 *
 */

#include <jni.h>
#include <string.h>
#include "hasp_api.h"

/*
 * Your Java namespace - change it to fit your package name, otherwise
 * the JVM is not able to load your functions!
 */
#define FUNCNAME(x)   Java_Aladdin_##x

/*
 * Class:     Hasp
 * Method:    hasp_login
 * Signature: (J[B[I)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(Hasp_Login)(JNIEnv *env, jclass obj, jlong feature_id, 
                jstring vendor_code, jintArray handle)
{
    const char *ptrByte_vendor_code = 
            (*env)->GetStringUTFChars(env, vendor_code, 0);
    jint *ptr32bit_handle = (*env)->GetIntArrayElements(env, handle, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_login((hasp_feature_t)feature_id, 
                    (hasp_vendor_code_t)ptrByte_vendor_code,
                    (hasp_handle_t*)ptr32bit_handle);

    (*env)->ReleaseStringUTFChars(env, vendor_code, ptrByte_vendor_code);
    (*env)->ReleaseIntArrayElements(env, handle, ptr32bit_handle, 0);

    return result;
}

#define SCOPE_VIEW_MAXSIZE (1024 * 128)

JNIEXPORT jint JNICALL
FUNCNAME(Hasp_LoginScope)(JNIEnv *env, jclass obj, jlong feature_id, 
                jstring scope, jstring vendor_code, jintArray handle)
{
    const char * ptrByte_vendor_code = 
            (*env)->GetStringUTFChars(env, vendor_code, 0);
    const char * ptrByte_scope = 
            (*env)->GetStringUTFChars(env, scope, 0);
    jint *ptr32bit_handle = (*env)->GetIntArrayElements(env, handle, 0);
    jbyteArray ptrDllScope = NULL;
    jbyte *ptrByte_DllScope = NULL;
    jint result = 0;

    (void)obj;

    ptrDllScope = (*env)->NewByteArray(env, SCOPE_VIEW_MAXSIZE);
    if( ptrDllScope == NULL ){
        (*env)->ReleaseStringUTFChars(env, vendor_code, ptrByte_vendor_code);
        (*env)->ReleaseStringUTFChars(env, scope, ptrByte_scope);
        (*env)->ReleaseIntArrayElements(env, handle, ptr32bit_handle, 0);
        return HASP_INSUF_MEM;
    }

    ptrByte_DllScope = (*env)->GetByteArrayElements(env, ptrDllScope,0);
    memset(ptrByte_DllScope, 0x0, SCOPE_VIEW_MAXSIZE);
    strcpy((char *)ptrByte_DllScope, (char *)ptrByte_scope);

    result = (jint)hasp_login_scope((hasp_feature_t)feature_id, 
                    (char*)ptrByte_scope, 
                    (hasp_vendor_code_t)ptrByte_vendor_code,
                    (hasp_handle_t*)ptr32bit_handle);

    (*env)->ReleaseByteArrayElements(env, ptrDllScope, ptrByte_DllScope, 0);
    (*env)->ReleaseStringUTFChars(env, vendor_code, ptrByte_vendor_code);
    (*env)->ReleaseStringUTFChars(env, scope, ptrByte_scope);
    (*env)->ReleaseIntArrayElements(env, handle, ptr32bit_handle, 0);

    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_logout
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_Logout)(JNIEnv *env, jclass obj, jint handle)
{
    (void)env;
    (void)obj;

    return (jint)hasp_logout((hasp_handle_t)handle);
}

/*
 * Class:     Hasp
 * Method:    hasp_encrypt
 * Signature: (I[BI)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_Encrypt)(JNIEnv *env, jclass obj, jint handle, 
                jbyteArray buffer, jint length)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_encrypt((hasp_handle_t)handle, 
                  (void *)ptrByte_Buffer, (hasp_size_t)length);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_decrypt
 * Signature: (I[BI)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_Decrypt)(JNIEnv *env, jclass obj, jint handle, 
                jbyteArray buffer, jint length)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_decrypt((hasp_handle_t)handle, 
                  (void *)ptrByte_Buffer, (hasp_size_t)length);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_read
 * Signature: (IIII[B)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(Hasp_Read)(JNIEnv *env, jclass obj, jint handle, jlong fileid, 
                jint offset, jint length, jbyteArray buffer)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_read((hasp_handle_t)handle, 
                    (hasp_fileid_t)fileid, (hasp_size_t)offset, 
                    (hasp_size_t) length, (void *)ptrByte_Buffer);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_write
 * Signature: (IIII[B)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_Write)(JNIEnv *env, jclass obj, jint handle, jlong fileid, 
                jint offset, jint length, jbyteArray buffer)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_write((hasp_handle_t)handle, 
                    (hasp_fileid_t)fileid, (hasp_size_t)offset, 
                    (hasp_size_t)length, (void *)ptrByte_Buffer);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_get_size
 * Signature: (II[I)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_GetSize)(JNIEnv *env, jclass obj, jint handle, 
                jlong fileid, jintArray size)
{
    jint *ptr32bit_Size = (*env)->GetIntArrayElements(env, size, 0);
    jint result = 0;

    (void)obj;

    result = hasp_get_size((hasp_handle_t)handle,
                    (hasp_fileid_t)fileid, (hasp_size_t *)ptr32bit_Size);

    (*env)->ReleaseIntArrayElements(env, size, ptr32bit_Size, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_get_rtc
 * Signature: (I[J)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(Hasp_GetRtc)(JNIEnv *env, jclass obj, jint handle, jlongArray time)
{
    unsigned long long *ptr64bit_time = (unsigned long long *)
            (*env)->GetLongArrayElements(env, time,0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_get_rtc((hasp_handle_t)handle,
                    (hasp_time_t*)ptr64bit_time);

    (*env)->ReleaseLongArrayElements(env, time, (jlong *)ptr64bit_time, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_encrypt
 * Signature: (I[BI)I
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_LegacyEncrypt)(JNIEnv *env, jclass obj, jint handle, 
                jbyteArray buffer, jint length)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_legacy_encrypt((hasp_handle_t)handle, 
                    (void *)ptrByte_Buffer, (hasp_size_t)length);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_decrypt
 * Signature: (I[BI)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(Hasp_LegacyDecrypt)(JNIEnv *env, jclass obj, jint handle, 
                jbyteArray buffer, jint length)
{
    jbyte *ptrByte_Buffer = (*env)->GetByteArrayElements(env, buffer, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_legacy_decrypt((hasp_handle_t)handle, 
                    (void *)ptrByte_Buffer,(hasp_size_t)length);

    (*env)->ReleaseByteArrayElements(env, buffer, ptrByte_Buffer, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_set_rtc
 * Signature: (IS  ???
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_LegacySetRtc)(JNIEnv *env, jclass obj, jint handle, 
                jlong new_time)
{
    (void)env;
    (void)obj;

    return (jint)hasp_legacy_set_rtc((hasp_handle_t)handle, 
                  (hasp_u64_t)new_time);
}

/*
 * Class:     Hasp
 * Method:    hasp_set_idletime
 * Signature: (IS  ???
 */
JNIEXPORT jint JNICALL
FUNCNAME(Hasp_LegacySetIdletime)(JNIEnv *env, jclass obj, jint handle, 
                jshort idle_time)
{
    (void)env;
    (void)obj;

    return (jint)hasp_legacy_set_idletime((hasp_handle_t)handle, 
                  (hasp_u16_t)idle_time);
}

/*
 * Class:     Hasp
 * Method:    hasp_get_info
 * Signature: (I[B[B[I)I
 */
JNIEXPORT jbyteArray JNICALL
FUNCNAME(Hasp_GetInfo)(JNIEnv *env, jclass obj, jstring scope,
                jstring format, jstring vendor_code, jintArray status)
{
    char *native_info=0;
    const char *ptrByte_format = (*env)->GetStringUTFChars(env, format, 0);
    const char *ptrByte_scope = (*env)->GetStringUTFChars(env, scope, 0);
    const char *ptrByte_vendor_code = 
          (*env)->GetStringUTFChars(env, vendor_code, 0 );
    jint *ptrInt_status = (*env)->GetIntArrayElements(env, status, 0);

    jbyte *ptrByte_info = 0;
    jbyteArray java_info = 0;

    int result = 0;

    (void)obj;

    result = hasp_get_info((const char *)ptrByte_scope,
                  (const char *)ptrByte_format, 
                  (hasp_vendor_code_t)ptrByte_vendor_code, &native_info);  

    ptrInt_status[0] = result;

    if (native_info) {
        java_info = (*env)->NewByteArray(env, (jsize)strlen(native_info));
        ptrByte_info = (*env)->GetByteArrayElements(env, java_info, 0);
        memcpy(ptrByte_info, native_info, strlen(native_info));
        hasp_free(native_info);
        (*env)->ReleaseByteArrayElements(env, java_info, ptrByte_info, 0);
    }

    (*env)->ReleaseStringUTFChars(env, scope, ptrByte_scope);
    (*env)->ReleaseStringUTFChars(env, format, ptrByte_format);
    (*env)->ReleaseStringUTFChars(env, vendor_code, ptrByte_vendor_code);
    (*env)->ReleaseIntArrayElements(env, status, ptrInt_status, 0);
  
    return java_info;  
}

/*
 * Class:     Hasp
 * Method:    hasp_get_sessioninfo
 * Signature: (I[B[B[I)I
 */
JNIEXPORT jbyteArray JNICALL
FUNCNAME(Hasp_GetSessioninfo)(JNIEnv *env, jclass obj, jint handle, 
                jstring format, jintArray status)
{
    char *native_info=0;
    const char *ptrByte_format = (*env)->GetStringUTFChars(env, format, 0);
    jint *ptrInt_status = (*env)->GetIntArrayElements(env, status, 0);
    jbyte *ptrByte_info = 0;
    jbyteArray java_info = 0;
    jint result = 0;

    (void)obj;

    result = hasp_get_sessioninfo((hasp_handle_t)handle,
                    (const char *)ptrByte_format, &native_info);  
    ptrInt_status[0] = result;

    if (native_info) {
        java_info = (*env)->NewByteArray(env, (jsize)strlen(native_info));
        ptrByte_info = (*env)->GetByteArrayElements(env, java_info, 0);
        memcpy(ptrByte_info, native_info, strlen(native_info));
        hasp_free(native_info);
        (*env)->ReleaseByteArrayElements(env, java_info, ptrByte_info, 0);
    }

    (*env)->ReleaseStringUTFChars(env, format, ptrByte_format);
    (*env)->ReleaseIntArrayElements(env, status, ptrInt_status, 0);
  
    return java_info;  
}

/*
 * Class:     Hasp
 * Method:    hasp_update
 * Signature: ([C)J
 */
JNIEXPORT jstring JNICALL
FUNCNAME(Hasp_Update)(JNIEnv *env, jclass obj, 
                jstring update_data, jintArray status)
{
    const char *ptr_update_data = 
            (*env)->GetStringUTFChars(env, update_data, 0);
    jint *ptrInt_status = (*env)->GetIntArrayElements(env, status, 0);
    char *native_ack_data = 0;
    jstring java_ack_data = NULL;

    hasp_status_t result = 0;

    (void)obj;

    result = hasp_update(ptr_update_data, &native_ack_data);

    ptrInt_status[0] = result;

    if (native_ack_data) {
        java_ack_data = (*env)->NewStringUTF(env, native_ack_data);
        hasp_free(native_ack_data);
    }

    (*env)->ReleaseIntArrayElements(env, status, ptrInt_status, 0);
    (*env)->ReleaseStringUTFChars(env, update_data, ptr_update_data);
  
    return java_ack_data;
}

/**
 * \brief Rehost a commuter license
 */
JNIEXPORT jbyteArray JNICALL 
FUNCNAME(Hasp_Transfer)(JNIEnv *env, jclass obj, jstring action, 
                jstring scope, jstring vendor_code, 
                jstring destination, jintArray status)
{
    char *native_info=0;
    const char *ptrByte_action =
            (*env)->GetStringUTFChars(env, action, 0);
    const char *ptrByte_scope =
            (*env)->GetStringUTFChars(env, scope, 0);
    const char *ptrByte_vendor_code =
            (*env)->GetStringUTFChars(env, vendor_code, 0);
    const char *ptrByte_destination =
            (*env)->GetStringUTFChars(env, destination, 0);
    jint *ptrInt_status = (*env)->GetIntArrayElements(env, status, 0);
    jint result = 0;
    jbyte *ptrByte_info = 0;
    jbyteArray java_info = 0;

    (void)obj;

    result = (jint)hasp_transfer(ptrByte_action, ptrByte_scope, 
                    (hasp_vendor_code_t)ptrByte_vendor_code, 
                    ptrByte_destination, &native_info);
    ptrInt_status[0] = result;

    if (native_info) {
        java_info = (*env)->NewByteArray(env, (jsize)strlen(native_info));
        ptrByte_info = (*env)->GetByteArrayElements(env, java_info, 0);
        memcpy(ptrByte_info, native_info, strlen(native_info));
        hasp_free(native_info);
        (*env)->ReleaseByteArrayElements(env, java_info, ptrByte_info, 0);
    }

    (*env)->ReleaseStringUTFChars(env, action, ptrByte_action);
    (*env)->ReleaseStringUTFChars(env, scope, ptrByte_scope);
    (*env)->ReleaseStringUTFChars(env, vendor_code, ptrByte_vendor_code);
    (*env)->ReleaseStringUTFChars(env, destination, ptrByte_destination);
    (*env)->ReleaseIntArrayElements(env, status, ptrInt_status, 0);

    return java_info;
}


/*
 * Class:     Hasp
 * Method:    hasp_datetime_to_hasptime
 * Signature: (IIIIII[J)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(HaspTime_DatetimeToHasptime)(JNIEnv *env, jclass obj, 
                jint day, jint month, jint year, jint hour, jint minute, 
                jint second, jlongArray time)
{
    unsigned long long *ptr64bit_time = (unsigned long long *)
            (*env)->GetLongArrayElements(env, time, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_datetime_to_hasptime((unsigned int)day, 
                    (unsigned int)month, (unsigned int)year, 
                    (unsigned int)hour, (unsigned int)minute, 
                    (unsigned int)second, (hasp_time_t *)ptr64bit_time);

    (*env)->ReleaseLongArrayElements(env, time, (jlong *)ptr64bit_time, 0);
    return result;
}

/*
 * Class:     Hasp
 * Method:    hasp_hasptime_to_datetime
 * Signature: (J[I[I[I[I[I[I)I
 */
JNIEXPORT jint JNICALL 
FUNCNAME(HaspTime_HasptimeToDatetime)(JNIEnv *env, jclass obj, 
                jlong time, jintArray day, jintArray month, jintArray year, 
                jintArray hour, jintArray minute, jintArray second)
{
    jint *ptrInt_day = (*env)->GetIntArrayElements(env, day, 0);
    jint *ptrInt_month = (*env)->GetIntArrayElements(env, month, 0);
    jint *ptrInt_year = (*env)->GetIntArrayElements(env, year, 0);
    jint *ptrInt_hour = (*env)->GetIntArrayElements(env, hour, 0);
    jint *ptrInt_minute = (*env)->GetIntArrayElements(env, minute, 0);
    jint *ptrInt_second = (*env)->GetIntArrayElements(env, second, 0);
    jint result = 0;

    (void)obj;

    result = (jint)hasp_hasptime_to_datetime((hasp_time_t)time, 
                    (unsigned int *)ptrInt_day, 
                    (unsigned int *)ptrInt_month, 
                    (unsigned int *)ptrInt_year, 
                    (unsigned int *)ptrInt_hour, 
                    (unsigned int *)ptrInt_minute,
                    (unsigned int *)ptrInt_second);

    (*env)->ReleaseIntArrayElements(env, day, ptrInt_day, 0);
    (*env)->ReleaseIntArrayElements(env, month, ptrInt_month, 0);
    (*env)->ReleaseIntArrayElements(env, year, ptrInt_year, 0);
    (*env)->ReleaseIntArrayElements(env, hour, ptrInt_hour, 0);
    (*env)->ReleaseIntArrayElements(env, minute, ptrInt_minute, 0);
    (*env)->ReleaseIntArrayElements(env, second, ptrInt_second, 0);

    return result;
}

/* 
 * Class:     HaspApiVersion
 * Method:    hasp_get_version
 */
JNIEXPORT jint JNICALL
FUNCNAME(HaspApiVersion_GetVersion)(JNIEnv *env, jclass obj, 
                jintArray major_version, jintArray minor_version, 
                jintArray build_server, jintArray build_number, 
                jbyteArray vendor_code)
{
    jint *ptrInt_major = (*env)->GetIntArrayElements(env, major_version, 0);
    jint *ptrInt_minor = (*env)->GetIntArrayElements(env, minor_version, 0);
    jint *ptrInt_buildSrv = (*env)->GetIntArrayElements(env, build_server, 0);
    jint *ptrInt_buildNo = (*env)->GetIntArrayElements(env, build_number, 0);
    jbyte *ptrByte_vendor_code = 
                    (*env)->GetByteArrayElements(env, vendor_code, 0);
    jint result = 0;
  
    (void)obj;

    result = (jint)hasp_get_version((unsigned int *)ptrInt_major, 
                    (unsigned int *)ptrInt_minor, 
                    (unsigned int *)ptrInt_buildSrv, 
                    (unsigned int *)ptrInt_buildNo, 
                    ptrByte_vendor_code);

    (*env)->ReleaseIntArrayElements(env, major_version, ptrInt_major, 0);
    (*env)->ReleaseIntArrayElements(env, minor_version, ptrInt_minor, 0);
    (*env)->ReleaseIntArrayElements(env, build_server, ptrInt_buildSrv, 0);
    (*env)->ReleaseIntArrayElements(env, build_number, ptrInt_buildNo, 0);
    (*env)->ReleaseByteArrayElements(env, vendor_code, ptrByte_vendor_code, 0);

    return result;
}

/* In Windows we must call the API DllMain to notify the API */
/* of the DLL handle. */
/* Specifically, it's used to load the External LM module from the same */
/* directory of the Java DLL */
#ifdef WIN32
#include <windows.h>

extern BOOL HASP_CALLCONV hasp_dllmain(HANDLE handle, DWORD reason, LPVOID reserved);

BOOL APIENTRY DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved)
{
    return hasp_dllmain(handle, reason, reserved);
}
#endif
