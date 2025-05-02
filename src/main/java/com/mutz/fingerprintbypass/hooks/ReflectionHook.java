package com.mutz.fingerprintbypass.hooks;

import java.lang.reflect.Method;

public class ReflectionHook {
    public abstract static class MethodHook {
        protected abstract Object onInvoke(Object receiver, Object... args) throws Throwable;
    }

    public static void hookMethod(Method method, MethodHook callback) {
        // Zygisk implementation will directly hook native methods
        // This is just a stub for compilation
    }

    public static Object invokeOriginalMethod(Object receiver, Object... args) throws Throwable {
        // Zygisk implementation will call original methods
        // This is just a stub for compilation
        return null;
    }
}
