package com.mutz.fingerprintbypass.utils;

import android.util.Log;

/**
 * Logger utility for the Fingerprint Bypass module
 * @author Mutz
 */
public class Logger {
    private static final boolean DEBUG = true;
    
    /**
     * Log a message with a tag
     * @param tag The log tag
     * @param msg The message to log
     */
    public static void log(String tag, String msg) {
        if (DEBUG) {
            try {
                Log.i(tag, msg);
            } catch (Throwable t) {
                // Fallback to system out if Log is not available
                System.out.println(tag + ": " + msg);
            }
        }
    }
    
    /**
     * Log an error with a tag
     * @param tag The log tag
     * @param msg The error message
     * @param t The throwable that caused the error
     */
    public static void logError(String tag, String msg, Throwable t) {
        if (DEBUG) {
            try {
                Log.e(tag, msg, t);
            } catch (Throwable ex) {
                // Fallback to system out if Log is not available
                System.out.println(tag + " ERROR: " + msg);
                t.printStackTrace();
            }
        }
    }
}
