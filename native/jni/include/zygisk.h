#pragma once

#include <jni.h>

#define ZYGISK_API_VERSION 2

// Hook function pointer types for biometric services
extern bool (*original_isHardwareDetected)(void*);
extern bool (*original_canAuthenticate)(void*, int);
extern int (*original_getAvailableSensorCount)(void*);

namespace zygisk {

// Manages the state associated with a single client
class ModuleBase {
public:
    // Override this if you want to handle the case when zygisk is enabled in Magisk Manager
    // but the module is disabled. By default this will call dlclose() on the module itself.
    virtual void onUnload() {}

    // When a new process is created, zygisk will call this method before calling nativeForkAndSpecialize
    // in zygote.
    virtual void onLoad() {}

    // Called where the JNI functions pre/post append nativeForkAndSpecialize would be called.
    // These are called when the process is still in zygote.
    virtual void preAppSpecialize(void *args) {}
    virtual void postAppSpecialize(void *args) {}

    // Called where the JNI function forkSystemServer would be called.
    // These are called when the process is still in zygote.
    virtual void preServerSpecialize(void *args) {}
    virtual void postServerSpecialize(void *args) {}

    // These are only for internal use
    void *_impl = nullptr;
};

class AppSpecializeArgs {
public:
    jint &uid;               /* Reference to 'uid' in the args */
    jint &debuggable;        /* Reference to 'debuggable' in the args */
    jstring &nice_name;      /* Reference to 'nice_name' in the args */
    jstring &app_data_dir;   /* Reference to 'app_data_dir' in the args */
    jboolean &mount_external;/* Reference to 'mount_external' in the args */

    AppSpecializeArgs() = delete;   /* Prevent stack allocating this object */
};

class ServerSpecializeArgs {
public:

    ServerSpecializeArgs() = delete;   /* Prevent stack allocating this object */
};

// Register the module class
class ModuleSetupImpl;
class ModuleSetup {
public:
    ModuleSetupImpl *impl = nullptr;
    void addModule(ModuleBase *module);
    static ModuleSetup* getInstance(void *impl) {
        ModuleSetup *self = new ModuleSetup();
        self->impl = static_cast<ModuleSetupImpl*>(impl);
        return self;
    }
};

} // namespace zygisk

// Magisk will find and call the zygisk_companion_entry entry point
// This function runs in magiskd context
// Return value: the raw fd of the socket that should be sent to the module
extern "C" __attribute__((visibility("default")))
int zygisk_companion_entry(int client_id);

// This macro serves as the entry point for the module in Zygisk context
#define ZYGISK_NAVIGATOR_HANDLER(module_class) \
extern "C" __attribute__((visibility("default"))) \
zygisk::ModuleBase *zygisk_module_entry() { \
    return new module_class(); \
}

// This macro serves as the entry point for the module in Zygisk context
#define ZYGISK_SETUP(impl) \
extern "C" __attribute__((visibility("default"))) \
void zygisk_module_setup(void *impl) { \
    auto setup = zygisk::ModuleSetup::getInstance(impl); \
    do_zygisk_setup(setup); \
}