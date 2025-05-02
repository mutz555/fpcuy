package com.mutz.fingerprintbypass;

public class ModuleInfo {
    public static final String MODULE_NAME = "Fingerprint Bypass by Mutz";
    public static final String MODULE_VERSION = "1.0";
    public static final String MODULE_AUTHOR = "Mutz";
    public static final String TARGET_DEVICE = "Infinix Hot 40 Pro";
    public static final String TARGET_SYSTEM = "HyperOS 2 Android 15";
    
    // Nama-nama kelas yang perlu di-hook berdasarkan log
    public static final String[] TARGET_CLASSES = {
        "com.android.server.biometrics.sensors.fingerprint.FingerprintAuthenticator",
        "com.android.server.biometrics.fingerprint.FingerprintServiceStubImpl",
        "com.android.server.biometrics.BiometricServiceBase",
        "com.android.server.biometrics.BiometricServiceWrapper",
        "com.android.server.biometrics.sensors.BiometricService",
        "android.hardware.biometrics.BiometricManager",
        "android.hardware.fingerprint.FingerprintManager"
    };
}