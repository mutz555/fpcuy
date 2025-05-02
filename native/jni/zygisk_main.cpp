#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <android/log.h>

// Include Zygisk API
#include "include/zygisk.h"

// Zygisk logging
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FingerprintBypassZygisk", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "FingerprintBypassZygisk", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FingerprintBypassZygisk", __VA_ARGS__)

// Forward declaration from module.cpp
extern "C" void fingerprint_bypass_main(JNIEnv* env);

// Zygisk module class implementation
class FingerprintBypassModule : public zygisk::ModuleBase {
public:
    // Called when the module is loaded into the zygote process
    void onLoad() override {
        LOGI("FingerprintBypass module loaded into zygote process");
    }

    // Called when a new process is forked from zygote
    void preAppSpecialize(void *args) override {
        auto app_spec = static_cast<zygisk::AppSpecializeArgs*>(args);
        
        // Only hook system_server and settings apps
        if (app_spec->nice_name) {
            const char* process_name = env->GetStringUTFChars(app_spec->nice_name, nullptr);
            bool is_system_server = strcmp(process_name, "system_server") == 0;
            
            // For settings app check
            const char* app_data_dir = nullptr;
            bool is_settings = false;
            
            if (app_spec->app_data_dir) {
                app_data_dir = env->GetStringUTFChars(app_spec->app_data_dir, nullptr);
                is_settings = strstr(app_data_dir, "com.android.settings") != nullptr;
            }
            
            if (is_system_server || is_settings) {
                LOGI("Hooking fingerprint in process: %s", process_name);
                hook_needed = true;
            }
            
            // Clean up JNI references
            env->ReleaseStringUTFChars(app_spec->nice_name, process_name);
            if (app_data_dir) {
                env->ReleaseStringUTFChars(app_spec->app_data_dir, app_data_dir);
            }
        }
    }

    // Called after process specialization
    void postAppSpecialize(void *args) override {
        if (hook_needed) {
            LOGI("Injecting fingerprint hooks into app process");
            fingerprint_bypass_main(env);
            hook_needed = false;
        }
    }

    // Called before system server specialization
    void preServerSpecialize(void *args) override {
        LOGI("System server starting, preparing fingerprint hooks");
        hook_needed = true;
    }

    // Called after system server specialization
    void postServerSpecialize(void *args) override {
        LOGI("System server started, injecting fingerprint hooks");
        fingerprint_bypass_main(env);
    }

private:
    bool hook_needed = false;
    JNIEnv* env = nullptr;
};

// Register our module with Zygisk
ZYGISK_NAVIGATOR_HANDLER(FingerprintBypassModule)

