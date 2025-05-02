#include <android/log.h>
#include "zygisk.h"

// Logging macros
#define LOG_TAG "FingerprintBypassZygisk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Forward declaration of your hook function
extern "C" void fingerprint_bypass_main(JNIEnv* env);

using namespace zygisk;

class FingerprintBypassModule : public ModuleBase {
public:
    void onLoad(Api const* api, JNIEnv* env_) override {
        LOGI("FingerprintBypass module loaded");
        env = env_;
    }

    void preAppSpecialize(AppSpecializeArgs* args) override {
        if (!env || !args->nice_name) return;
        const char* name = env->GetStringUTFChars((jstring)args->nice_name, nullptr);
        bool hook = strcmp(name, "system_server") == 0 ||
                    (args->app_data_dir && strstr(env->GetStringUTFChars((jstring)args->app_data_dir, nullptr), "com.android.settings"));
        if (hook) {
            LOGI("Hooking fingerprint in process: %s", name);
            hook_needed = true;
        }
        env->ReleaseStringUTFChars((jstring)args->nice_name, name);
    }

    void postAppSpecialize(AppSpecializeArgs* args) override {
        if (hook_needed) {
            LOGI("Injecting fingerprint hook into app");
            fingerprint_bypass_main(env);
            hook_needed = false;
        }
    }

    void preServerSpecialize(ServerSpecializeArgs* args) override {
        LOGI("System server pre-specialize");
        hook_needed = true;
    }

    void postServerSpecialize(ServerSpecializeArgs* args) override {
        if (hook_needed) {
            LOGI("Injecting fingerprint hook into system_server");
            fingerprint_bypass_main(env);
            hook_needed = false;
        }
    }

private:
    bool hook_needed = false;
    JNIEnv* env = nullptr;
};

// Register the module for Zygisk
REGISTER_ZYGISK_MODULE(FingerprintBypassModule)