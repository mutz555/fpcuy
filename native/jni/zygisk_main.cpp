class FingerprintBypassModule : public zygisk::ModuleBase {
public:
    void onLoad(zygisk::Api const* api, JNIEnv* env_) override {
        LOGI("FingerprintBypass module loaded");
        env = env_; // Simpan env untuk digunakan nanti
    }

    void preAppSpecialize(zygisk::AppSpecializeArgs* args) override {
        if (!env || !args->nice_name) return;

        const char* process_name = env->GetStringUTFChars(args->nice_name, nullptr);
        bool is_system_server = strcmp(process_name, "system_server") == 0;

        const char* app_data_dir = nullptr;
        bool is_settings = false;

        if (args->app_data_dir) {
            app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
            is_settings = strstr(app_data_dir, "com.android.settings") != nullptr;
        }

        if (is_system_server || is_settings) {
            LOGI("Hooking process: %s", process_name);
            hook_needed = true;
        }

        env->ReleaseStringUTFChars(args->nice_name, process_name);
        if (app_data_dir) {
            env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
        }
    }

    void postAppSpecialize(zygisk::AppSpecializeArgs* args) override {
        if (hook_needed) {
            LOGI("Injecting fingerprint hook into app");
            fingerprint_bypass_main(env);
            hook_needed = false;
        }
    }

    void preServerSpecialize(zygisk::ServerSpecializeArgs* args) override {
        LOGI("System server pre-specialize");
        hook_needed = true;
    }

    void postServerSpecialize(zygisk::ServerSpecializeArgs* args) override {
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

// Ini dia ayang, macro yang paling penting!
REGISTER_ZYGISK_MODULE(FingerprintBypassModule)