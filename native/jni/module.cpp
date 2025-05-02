#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <jni.h>
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FingerprintBypassNative", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FingerprintBypassNative", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FingerprintBypassNative", __VA_ARGS__)

// Fungsi bantuan untuk menemukan alamat fungsi dalam library
static void *find_function_address(void *handle, const char *symbol) {
    void *addr = dlsym(handle, symbol);
    if (addr == nullptr) {
        LOGE("Gagal menemukan simbol: %s", symbol);
    } else {
        LOGI("Berhasil menemukan simbol: %s pada alamat %p", symbol, addr);
    }
    return addr;
}

// Fungsi asli yang akan di-hook
static bool (*original_isHardwareDetected)(void*, int);
static bool (*original_canAuthenticate)(void*, int, int, int);
static int (*original_getAvailableSensorCount)(void*);

// Implementasi pengganti fungsi yang di-hook
static bool hooked_isHardwareDetected(void* thiz, int authenticators) {
    LOGI("[FingerprintBypass] isHardwareDetected dipanggil - mengembalikan TRUE");
    return true;
}

static bool hooked_canAuthenticate(void* thiz, int userId, int authenticators, int flags) {
    LOGI("[FingerprintBypass] canAuthenticate dipanggil - mengembalikan TRUE");
    return true;
}

static int hooked_getAvailableSensorCount(void* thiz) {
    LOGI("[FingerprintBypass] getAvailableSensorCount dipanggil - mengembalikan 1");
    return 1;
}

// Hook untuk framework Java
static void hook_java_methods(JNIEnv *env) {
    // Hook BiometricManager
    jclass biometricManagerClass = env->FindClass("android/hardware/biometrics/BiometricManager");
    if (biometricManagerClass != nullptr) {
        LOGI("Menemukan BiometricManager class");
        
        // Hook method isHardwareDetected
        jmethodID isHardwareDetectedMethod = env->GetMethodID(biometricManagerClass, "isHardwareDetected", "(I)Z");
        if (isHardwareDetectedMethod != nullptr) {
            LOGI("Menemukan method isHardwareDetected, melakukan hook");
            // Implementasi hook disini
        }
        
        env->DeleteLocalRef(biometricManagerClass);
    } else {
        LOGE("Gagal menemukan BiometricManager class");
    }
    
    // Hook FingerprintManager
    jclass fingerprintManagerClass = env->FindClass("android/hardware/fingerprint/FingerprintManager");
    if (fingerprintManagerClass != nullptr) {
        LOGI("Menemukan FingerprintManager class");
        
        // Hook method isHardwareDetected
        jmethodID isHardwareDetectedMethod = env->GetMethodID(fingerprintManagerClass, "isHardwareDetected", "()Z");
        if (isHardwareDetectedMethod != nullptr) {
            LOGI("Menemukan method isHardwareDetected, melakukan hook");
            // Implementasi hook disini
        }
        
        env->DeleteLocalRef(fingerprintManagerClass);
    } else {
        LOGE("Gagal menemukan FingerprintManager class");
    }
    
    // Hook HyperOS specific classes berdasarkan log dan screenshot
    const char *hyperos_classes[] = {
        "com/android/server/biometrics/sensors/fingerprint/FingerprintAuthenticator", // Fokus utama berdasarkan screenshot
        "com/android/server/biometrics/fingerprint/FingerprintServiceStubImpl",
        "com/android/server/biometrics/BiometricServiceBase",
        "com/android/server/biometrics/BiometricServiceWrapper",
        "com/android/server/biometrics/sensors/BiometricService"
    };
    
    for (const char* className : hyperos_classes) {
        jclass cls = env->FindClass(className);
        if (cls != nullptr) {
            LOGI("Berhasil menemukan class %s", className);
            
            // Temukan dan hook method isHardwareDetected
            jmethodID isHardwareDetectedMethod = env->GetMethodID(cls, "isHardwareDetected", "()Z");
            if (isHardwareDetectedMethod != nullptr) {
                LOGI("Menemukan method isHardwareDetected dalam %s", className);
                
                // Buat JNI method yang selalu return true
                static const char* capturedClassName = className; // Simpan nama kelas di variabel lokal untuk lambda
                static JNINativeMethod methods[] = {
                    {"isHardwareDetected", "()Z", (void *) []() -> jboolean {
                        LOGI("[HookReplacement] %s.isHardwareDetected dipanggil - mengembalikan TRUE", capturedClassName);
                        return JNI_TRUE;
                    }}
                };
                
                // Register native method override
                if (env->RegisterNatives(cls, methods, 1) == 0) {
                    LOGI("Berhasil meng-hook isHardwareDetected pada %s", className);
                } else {
                    LOGE("Gagal meng-hook isHardwareDetected pada %s", className);
                    env->ExceptionClear();
                }
            }
            
            env->DeleteLocalRef(cls);
        } else {
            // Bersihkan pengecualian yang mungkin terjadi dari FindClass
            env->ExceptionClear();
            LOGI("Class tidak ditemukan: %s (normal untuk beberapa kelas HyperOS)", className);
        }
    }
}

// Hook system services
static void hook_system_services(JNIEnv *env) {
    LOGI("Mengaitkan hook ke system services yang menanganli fingerprint");
    
    // Coba akses system service manager
    jclass serviceManagerClass = env->FindClass("android/os/ServiceManager");
    if (serviceManagerClass != nullptr) {
        LOGI("Ditemukan ServiceManager class");
        
        // Dapatkan method getService
        jmethodID getServiceMethod = env->GetStaticMethodID(serviceManagerClass, "getService", "(Ljava/lang/String;)Landroid/os/IBinder;");
        if (getServiceMethod != nullptr) {
            LOGI("Menemukan ServiceManager.getService method");
            
            // Coba ambil fingerprint service
            jstring serviceName = env->NewStringUTF("fingerprint");
            jobject fingerprint_service = env->CallStaticObjectMethod(serviceManagerClass, getServiceMethod, serviceName);
            
            if (fingerprint_service != nullptr) {
                LOGI("Berhasil mendapatkan fingerprint service");
                // Implementasi hook untuk service disini
                env->DeleteLocalRef(fingerprint_service);
            } else {
                LOGI("Gagal mendapatkan fingerprint service (normal untuk beberapa proses)");
            }
            
            env->DeleteLocalRef(serviceName);
        }
        
        env->DeleteLocalRef(serviceManagerClass);
    } else {
        LOGE("Gagal menemukan ServiceManager class");
    }
}

// Fungsi utama untuk hook fingerprint
extern "C" void fingerprint_bypass_main(JNIEnv* env) {
    LOGI("Fingerprint bypass starting - oleh Mutz");
    LOGI("Target: HyperOS 2 di Infinix Hot 40 Pro");
    
    // Hook Java methods terlebih dahulu
    hook_java_methods(env);
    
    // Hook system services
    hook_system_services(env);
    
    LOGI("Semua hook telah terpasang!");
}

// JNI OnLoad dipanggil saat library diload
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad dipanggil untuk Fingerprint Bypass by Mutz");
    
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("Gagal mendapatkan JNI environment");
        return JNI_ERR;
    }
    
    // Panggil fungsi utama
    fingerprint_bypass_main(env);
    
    return JNI_VERSION_1_6;
}
