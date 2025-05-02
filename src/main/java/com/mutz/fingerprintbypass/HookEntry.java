package com.mutz.fingerprintbypass;

import android.content.Context;
import android.util.Log;

import com.mutz.fingerprintbypass.hooks.FingerprintHook;

public class HookEntry {
    private static final String TAG = "FingerprintBypass";

    public static void initZygote() {
        Log.i(TAG, "initZygote — hooking system methods");
        
        try {
            // Hook BiometricManager.isHardwareDetected
            FingerprintHook.hookBiometricManager();
            Log.i(TAG, "hooked android.hardware.biometrics.BiometricManager.isHardwareDetected");
        } catch (Throwable t) {
            Log.i(TAG, "failed hook BiometricManager → " + t.toString());
        }
        
        try {
            // Hook BiometricPrompt.authenticate
            FingerprintHook.hookBiometricPrompt();
            Log.i(TAG, "hooked android.hardware.biometrics.BiometricPrompt.authenticate");
        } catch (Throwable t) {
            Log.i(TAG, "failed hook BiometricPrompt → " + t.toString());
        }
    }

    public static void handleLoadPackage(String packageName, ClassLoader classLoader) {
        Log.i(TAG, "Loaded pkg → " + packageName);
        
        // Specific hooks for system settings
        if ("com.android.settings".equals(packageName)) {
            Log.i(TAG, "Applying hooks in com.android.settings");
            
            try {
                // Hook FingerprintServiceStubImpl.isHardwareDetected
                FingerprintHook.hookFingerprintServiceStubImpl(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl.isHardwareDetected");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl.isHardwareDetected → " + t.toString());
            }
            
            try {
                // Hook FingerprintServiceStubImpl.getErrorString
                FingerprintHook.hookFingerprintServiceGetErrorString(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl.getErrorString");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl.getErrorString → " + t.toString());
            }
            
            try {
                // Hook FingerprintAuthenticator.canAuthenticate
                FingerprintHook.hookFingerprintAuthenticatorCanAuthenticate(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator.canAuthenticate");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator.canAuthenticate → " + t.toString());
            }
            
            try {
                // Hook FingerprintAuthenticator.getAvailableSensorCount
                FingerprintHook.hookFingerprintAuthenticatorSensorCount(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator.getAvailableSensorCount");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator.getAvailableSensorCount → " + t.toString());
            }
            
            try {
                // Hook BiometricService.isHardwareDetected
                FingerprintHook.hookBiometricService(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.sensors.BiometricService.isHardwareDetected");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.sensors.BiometricService.isHardwareDetected → " + t.toString());
            }
            
            try {
                // Hook BiometricServiceBase.isHardwareDetected
                FingerprintHook.hookBiometricServiceBase(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.BiometricServiceBase.isHardwareDetected");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.BiometricServiceBase.isHardwareDetected → " + t.toString());
            }
            
            try {
                // Hook BiometricServiceWrapper.isHardwareDetected
                FingerprintHook.hookBiometricServiceWrapper(classLoader);
                Log.i(TAG, "hooked com.android.server.biometrics.BiometricServiceWrapper.isHardwareDetected");
            } catch (Throwable t) {
                Log.i(TAG, "failed hook com.android.server.biometrics.BiometricServiceWrapper.isHardwareDetected → " + t.toString());
            }
        }
    }
}
