#pragma once

#include <jni.h>
#include <android/log.h>
#include <string>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FingerprintBypass", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FingerprintBypass", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "FingerprintBypass", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FingerprintBypass", __VA_ARGS__)

namespace zygisk {
    void entry(JNIEnv *env, const char *module_path);
}
