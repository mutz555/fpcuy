package com.mutz.fingerprintbypass.hooks;

import com.mutz.fingerprintbypass.utils.Logger;

import java.lang.reflect.Method;

/**
 * Hooks for BiometricManager class
 * @author Mutz
 */
public class BiometricManagerHook {
    private static final String TAG = "FingerprintBypass";
    
    /**
     * Initialize BiometricManager hooks
     */
    public static void init() throws Throwable {
        Class<?> biometricManagerClass = Class.forName("android.hardware.biometrics.BiometricManager");
        
        // Hook isHardwareDetected method
        Method isHardwareDetected = biometricManagerClass.getDeclaredMethod("isHardwareDetected");
        hookIsHardwareDetected(isHardwareDetected);
        
        // Hook canAuthenticate method
        try {
            Method canAuthenticate = biometricManagerClass.getDeclaredMethod("canAuthenticate");
            hookCanAuthenticate(canAuthenticate);
        } catch (NoSuchMethodException e) {
            // Try the version with authenticators parameter (Android 11+)
            try {
                Method canAuthenticate = biometricManagerClass.getDeclaredMethod("canAuthenticate", int.class);
                hookCanAuthenticateWithInt(canAuthenticate);
            } catch (NoSuchMethodException ex) {
                Logger.log(TAG, "No suitable canAuthenticate method found");
            }
        }
        
        // Hook BiometricPrompt if available
        try {
            Class<?> biometricPromptClass = Class.forName("android.hardware.biometrics.BiometricPrompt");
            Method authenticate = biometricPromptClass.getDeclaredMethod("authenticate");
            hookAuthenticate(authenticate);
            Logger.log(TAG, "hooked BiometricPrompt.authenticate");
        } catch (ClassNotFoundException | NoSuchMethodException e) {
            Logger.log(TAG, "failed hook BiometricPrompt → " + e.getMessage());
        }
    }
    
    /**
     * Hook the isHardwareDetected method to always return true
     */
    private static void hookIsHardwareDetected(final Method method) throws Throwable {
        FingerprintHook.hookMethod(method);
    }
    
    /**
     * Hook the canAuthenticate method to always return success (0)
     */
    private static void hookCanAuthenticate(final Method method) throws Throwable {
        FingerprintHook.hookMethodToReturnInt(method, 0); // BIOMETRIC_SUCCESS = 0
    }
    
    /**
     * Hook the canAuthenticate(int authenticators) method to always return success
     */
    private static void hookCanAuthenticateWithInt(final Method method) throws Throwable {
        FingerprintHook.hookMethodToReturnInt(method, 0); // BIOMETRIC_SUCCESS = 0
    }
    
    /**
     * Hook the authenticate method to bypass actual authentication
     */
    private static void hookAuthenticate(final Method method) throws Throwable {
        // This is more complex and would require simulating a successful callback
        // For now, we're just ensuring hardware is detected
        Logger.log(TAG, "BiometricPrompt.authenticate hook registered");
    }
}
