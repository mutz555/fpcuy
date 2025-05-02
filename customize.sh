#!/system/bin/sh
SKIPUNZIP=1

# Extract files
ui_print "- Extracting module files"
unzip -o "$ZIPFILE" -x 'META-INF/*' -d $MODPATH >&2

# Set executable permissions
set_perm_recursive $MODPATH 0 0 0755 0644

# Check architecture and install appropriate libraries
ui_print "- Checking device architecture"

# Create zygisk directory if it doesn't exist
mkdir -p $MODPATH/zygisk

# Install for both architectures if available
if [ -f $MODPATH/zygisk/arm64-v8a.so ] && [ -f $MODPATH/zygisk/armeabi-v7a.so ]; then
  ui_print "- Installing for both arm64 and arm architectures"
else 
  if [ "$ARCH" = "arm" ]; then
    ui_print "- Architecture: arm"
    cp -f $MODPATH/zygisk/armeabi-v7a.so $MODPATH/zygisk/armeabi-v7a.so 2>/dev/null || :
  elif [ "$ARCH" = "arm64" ]; then
    ui_print "- Architecture: arm64"
    cp -f $MODPATH/zygisk/arm64-v8a.so $MODPATH/zygisk/arm64-v8a.so 2>/dev/null || :
  else
    ui_print "! Unsupported architecture: $ARCH"
    abort "! Unsupported architecture"
  fi
fi

# Set proper permissions for Zygisk libraries
set_perm $MODPATH/zygisk/arm64-v8a.so 0 0 0644
set_perm $MODPATH/zygisk/armeabi-v7a.so 0 0 0644

# Clean up unneeded files
rm -rf $MODPATH/native 2>/dev/null

ui_print "- Done"
ui_print "- Fingerprint Bypass by Mutz successfully installed"
ui_print "- Modul ini untuk memperbaiki fingerprint service di Infinix Hot 40 Pro dengan HyperOS 2 Android 15"
ui_print "- Reboot your device to apply changes"
