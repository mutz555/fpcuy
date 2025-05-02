# Fingerprint Bypass by Mutz

Modul Zygisk untuk membypass fingerprint service pada Infinix Hot 40 Pro dengan HyperOS 2 Android 15, fokus pada FingerprintAuthenticator.isHardwareDetected()

## Deskripsi

Modul ini dibuat untuk mengatasi masalah pada sensor fingerprint di Infinix Hot 40 Pro yang menjalankan HyperOS 2 pada Android 15. Modul bekerja dengan mengganti fungsi-fungsi berikut agar selalu mengembalikan nilai positif:

- `FingerprintAuthenticator.isHardwareDetected()` → selalu `true`
- `FingerprintServiceStubImpl.isHardwareDetected()` → selalu `true`
- `BiometricManager.isHardwareDetected()` → selalu `true`
- `FingerprintManager.isHardwareDetected()` → selalu `true`

## Instalasi

1. Pastikan Anda memiliki Magisk v24+ atau KernelSU yang terinstall
2. Aktifkan fitur Zygisk di Magisk
3. Download file zip dari release
4. Flash melalui recovery atau Magisk Manager
5. Restart perangkat

## Target Device

- Infinix Hot 40 Pro
- HyperOS 2
- Android 15

## Cara Kerja

Modul ini menggunakan Zygisk API untuk melakukan hook pada level system service. Modul akan menginjeksi kode native ke dalam proses system_server dan aplikasi Settings, sehingga dapat mengganti perilaku fingerprint service.

## Troubleshooting

Jika modul tidak bekerja:

1. Pastikan Zygisk diaktifkan di Magisk Manager
2. Pastikan module diaktifkan di Magisk Manager
3. Cobalah untuk mengaktifkan fitur "Enforce DenyList" di Magisk Manager
4. Cek logs dengan perintah: `logcat | grep FingerprintBypass`

## Pengembangan

### Menggunakan GitHub Actions (Cara Paling Mudah)

1. Fork repository ini
2. Aktifkan GitHub Actions di repository Anda
3. Edit source code sesuai kebutuhan
4. Push perubahan ke repository
5. GitHub Actions akan otomatis build modul dan menyediakannya sebagai artifact
6. Anda juga bisa membuat tag untuk release otomatis

#### Menggunakan Manual Build

1. Clone repository
2. Edit source code di `native/jni/`
3. Build dengan Android NDK: `cd native/jni && ndk-build`
4. Salin file .so ke folder zygisk
5. Zip semua file untuk membuat flashable zip

## Struktur Direktori

```
├── META-INF/                    # Magisk module installer files
├── native/
│   └── jni/                    # Native source code
│       ├── include/            # Zygisk API header
│       ├── module.cpp          # Main hooking logic
│       ├── zygisk_main.cpp     # Zygisk entry point
│       ├── Android.mk          # NDK build config
│       └── Application.mk      # NDK application config
├── zygisk/                     # Output directory for native libraries
├── module.prop                 # Module info
├── customize.sh                # Installation script
└── uninstall.sh               # Uninstallation script
```

## Build & Release

Konfigurasi GitHub Actions sudah dibuat di file `.github/workflows/build.yml`. Workflow ini akan melakukan:

1. Build library native untuk arm64-v8a dan armeabi-v7a
2. Paket library ke dalam struktur modul Zygisk
3. Zip semua file untuk membuat modul Magisk yang bisa di-flash
4. Upload artifact build untuk testing dan deployment
5. Membuat release otomatis jika push dengan tag

Untuk membuat release, cukup buat tag dan push:
```bash
git tag v1.0
git push --tags
```

## Lisensi

Free to use, diharapkan memberikan credit kepada "Mutz" jika melakukan modifikasi.