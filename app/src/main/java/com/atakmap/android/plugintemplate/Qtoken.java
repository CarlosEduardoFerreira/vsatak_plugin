//
// Created by Carlos Ferreira on 3/10/21.
//

package com.atakmap.android.plugintemplate;



public class Qtoken  {

    static {
        System.loadLibrary("gflags_nothreads");

        System.loadLibrary("crypto");
        System.loadLibrary("ssl");

        System.loadLibrary("qtoken");

        System.loadLibrary("main-bridge");
    }

    public static native void run();

    public static native void put();

    public static native void share();

    public static native void spread();

    public static native void gather();

}
