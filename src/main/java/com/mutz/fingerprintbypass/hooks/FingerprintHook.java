package com.mutz.fingerprintbypass.hooks;

import android.hardware.biometrics.BiometricManager;
import android.hardware.biometrics.BiometricPrompt;
import android.util.Log;

import java.lang.reflect.Method;

public class FingerprintHook {
    private static final String TAG = "FingerprintBypass";

    // Hook BiometricManager.isHardwareDetected
    public static void hookBiometricManager() throws Throwable {
        Class<?> biometricManagerClass = BiometricManager.class;
        Method isHardwareDetectedMethod = biometricManagerClass.getDeclaredMethod("isHardwareDetected", int.class);
        
        // Overriding the original method to always return true
        ReflectionHook.hookMethod(isHardwareDetectedMethod, new ReflectionHook.MethodHook() {
            @Override
            protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                Log.d(TAG, "BiometricManager.isHardwareDetected called, returning true");
                return true;
            }
        });
    }

    // Hook BiometricPrompt.authenticate
    public static void hookBiometricPrompt() throws Throwable {
        try {
            Class<?> biometricPromptClass = BiometricPrompt.class;
            Method authenticateMethod = null;
            
            // Try to find the authenticate method with different signatures
            for (Method method : biometricPromptClass.getDeclaredMethods()) {
                if (method.getName().equals("authenticate")) {
                    authenticateMethod = method;
                    break;
                }
            }
            
            if (authenticateMethod != null) {
                ReflectionHook.hookMethod(authenticateMethod, new ReflectionHook.MethodHook() {
                    @Override
                    protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                        Log.d(TAG, "BiometricPrompt.authenticate called, bypassing authentication");
                        // Here we would handle the authentication result
                        // but this would require more context about the callback object
                        // So we just let the original method proceed for now
                        return ReflectionHook.invokeOriginalMethod(receiver, args);
                    }
                });
            }
        } catch (Throwable t) {
            Log.e(TAG, "Error hooking BiometricPrompt.authenticate: " + t.getMessage());
            throw t;
        }
    }

    // Hook FingerprintServiceStubImpl.isHardwareDetected
    public static void hookFingerprintServiceStubImpl(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> fingerprintServiceStubImplClass = classLoader.loadClass(
                    "com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl");
            Method isHardwareDetectedMethod = fingerprintServiceStubImplClass.getDeclaredMethod("isHardwareDetected");
            
            ReflectionHook.hookMethod(isHardwareDetectedMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "FingerprintServiceStubImpl.isHardwareDetected called, returning true");
                    return true;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl");
            throw e;
        }
    }

    // Hook FingerprintServiceStubImpl.getErrorString
    public static void hookFingerprintServiceGetErrorString(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> fingerprintServiceStubImplClass = classLoader.loadClass(
                    "com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl");
            Method getErrorStringMethod = fingerprintServiceStubImplClass.getDeclaredMethod("getErrorString", int.class, int.class);
            
            ReflectionHook.hookMethod(getErrorStringMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "FingerprintServiceStubImpl.getErrorString called");
                    // Let the original method handle it
                    return ReflectionHook.invokeOriginalMethod(receiver, args);
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl");
            throw e;
        }
    }

    // Hook FingerprintAuthenticator.canAuthenticate
    public static void hookFingerprintAuthenticatorCanAuthenticate(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> fingerprintAuthenticatorClass = classLoader.loadClass(
                    "com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator");
            Method canAuthenticateMethod = fingerprintAuthenticatorClass.getDeclaredMethod("canAuthenticate", int.class, int.class, int.class);
            
            ReflectionHook.hookMethod(canAuthenticateMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "FingerprintAuthenticator.canAuthenticate called, returning true");
                    return true;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator");
            throw e;
        }
    }

    // Hook FingerprintAuthenticator.getAvailableSensorCount
    public static void hookFingerprintAuthenticatorSensorCount(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> fingerprintAuthenticatorClass = classLoader.loadClass(
                    "com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator");
            Method getAvailableSensorCountMethod = fingerprintAuthenticatorClass.getDeclaredMethod("getAvailableSensorCount");
            
            ReflectionHook.hookMethod(getAvailableSensorCountMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "FingerprintAuthenticator.getAvailableSensorCount called, returning 1");
                    return 1;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator");
            throw e;
        }
    }

    // Hook BiometricService.isHardwareDetected
    public static void hookBiometricService(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> biometricServiceClass = classLoader.loadClass(
                    "com.android.server.biometrics.sensors.BiometricService");
            Method isHardwareDetectedMethod = biometricServiceClass.getDeclaredMethod("isHardwareDetected", int.class);
            
            ReflectionHook.hookMethod(isHardwareDetectedMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "BiometricService.isHardwareDetected called, returning true");
                    return true;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.sensors.BiometricService");
            throw e;
        }
    }

    // Hook BiometricServiceBase.isHardwareDetected
    public static void hookBiometricServiceBase(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> biometricServiceBaseClass = classLoader.loadClass(
                    "com.android.server.biometrics.BiometricServiceBase");
            Method isHardwareDetectedMethod = biometricServiceBaseClass.getDeclaredMethod("isHardwareDetected");
            
            ReflectionHook.hookMethod(isHardwareDetectedMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "BiometricServiceBase.isHardwareDetected called, returning true");
                    return true;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.BiometricServiceBase");
            throw e;
        }
    }

    // Hook BiometricServiceWrapper.isHardwareDetected
    public static void hookBiometricServiceWrapper(ClassLoader classLoader) throws Throwable {
        try {
            Class<?> biometricServiceWrapperClass = classLoader.loadClass(
                    "com.android.server.biometrics.BiometricServiceWrapper");
            Method isHardwareDetectedMethod = biometricServiceWrapperClass.getDeclaredMethod("isHardwareDetected");
            
            ReflectionHook.hookMethod(isHardwareDetectedMethod, new ReflectionHook.MethodHook() {
                @Override
                protected Object onInvoke(Object receiver, Object... args) throws Throwable {
                    Log.d(TAG, "BiometricServiceWrapper.isHardwareDetected called, returning true");
                    return true;
                }
            });
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "Class not found: com.android.server.biometrics.BiometricServiceWrapper");
            throw e;
        }
    }
}
