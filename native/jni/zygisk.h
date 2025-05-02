#pragma once

#include <jni.h>

namespace zygisk {

    // Zygisk API version
    constexpr int ZYGISK_API_VERSION = 1;

    // Zygisk includes these methods to the app
    struct Api {
        // Get the Java application context
        jobject getApplicationContext();
    };

    // Arguments for app process specialization
    struct AppSpecializeArgs {
        // Standard zygisk args
        jint uid;
        jint gid;
        jintArray gids;
        jint runtime_flags;
        jobjectArray mount_external;
        jstring se_info;
        jstring nice_name;
        jstring instruction_set;
        jstring app_data_dir;
    };

    // Arguments for system server process specialization
    struct ServerSpecializeArgs {
        jint uid;
        jint gid;
        jintArray gids;
        jint runtime_flags;
        jlong permitted_capabilities;
        jlong effective_capabilities;
    };

    // The base class for a Zygisk module
    class ModuleBase {
    public:
        // Called when the module is loaded
        virtual void onLoad(Api *api, JNIEnv *env) {}
        
        // Called before app process specialization
        virtual void preAppSpecialize(AppSpecializeArgs *args) {}
        
        // Called after app process specialization
        virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
        
        // Called before system server process specialization
        virtual void preServerSpecialize(ServerSpecializeArgs *args) {}
        
        // Called after system server process specialization
        virtual void postServerSpecialize(const ServerSpecializeArgs *args) {}
        
        virtual ~ModuleBase() {}
    };

    // Register your Zygisk module
    #define REGISTER_ZYGISK_MODULE(clazz) \
    extern "C" __attribute__((visibility("default"))) \
    zygisk::ModuleBase *zygisk_module_entry() { \
        return new clazz(); \
    }

} // namespace zygisk
