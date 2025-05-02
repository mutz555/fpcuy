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
