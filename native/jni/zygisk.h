#pragma once

#include <jni.h>
#include <cstdint>
#include <functional>

namespace zygisk {

// Arguments passed to preAppSpecialize/postAppSpecialize
struct AppSpecializeArgs {
    JNIEnv* env;
    jobject nice_name;      // Java String
    jobject app_data_dir;   // Java String
};

// Arguments passed to preServerSpecialize/postServerSpecialize
struct ServerSpecializeArgs {
    JNIEnv* env;
    // add fields as needed
};

// API pointer (reserved for future use)
struct Api {};

// Base class for Zygisk modules
class ModuleBase {
public:
    virtual void onLoad(Api const* api, JNIEnv* env) {}
    virtual void preAppSpecialize(AppSpecializeArgs* args) {}
    virtual void postAppSpecialize(AppSpecializeArgs* args) {}
    virtual void preServerSpecialize(ServerSpecializeArgs* args) {}
    virtual void postServerSpecialize(ServerSpecializeArgs* args) {}
    virtual void onUnload() {}
    virtual ~ModuleBase() = default;
};

// Macro to register your module and export the zygisk_module symbol
#define REGISTER_ZYGISK_MODULE(moduleClass)                 \
  extern "C" ModuleBase* __zygisk_create_module() {       \
    return new moduleClass();                              \
  }                                                         \
  extern "C" intptr_t zygisk_module = (intptr_t)&__zygisk_create_module;

} // namespace zygisk