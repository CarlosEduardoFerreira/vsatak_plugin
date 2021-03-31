/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.atakmap.android.plugintemplate;


import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class MainBridgeCPP {


//~/dev/qtoken-cpp on  remove-sodium
//❯ l /opt/VIN/
//    total 36K
//    drwxr-xr-x  5 root root 4.0K Jan 27 11:35 .
//    drwxr-xr-x 19 root root 4.0K Mar  7 12:00 ..
//    drwxrwxrwx  2 root root 4.0K Jan 29 17:35 keys
//    drwxrwxrwx  2 root root  20K Mar 23 01:24 outputs
//    drwxr-xr-x  4 root root 4.0K Jan 27 11:35 receipts






    public void run() throws InterruptedException, IOException {

//        File sdcard = Environment.getExternalStorageDirectory();

        String sdcard = System.getenv("EXTERNAL_STORAGE");

        File vinFolder = new File(sdcard + "/VIN");
        if(!vinFolder.exists()) {
            vinFolder.mkdirs();
        }

        File vinFolderKeys = new File(sdcard + "/VIN/keys");
        if(!vinFolderKeys.exists()) {
            vinFolderKeys.mkdirs();
        }

        File vinFolderOutputs = new File(sdcard + "/VIN/outputs");
        if(!vinFolderOutputs.exists()) {
            vinFolderOutputs.mkdirs();
        }

        File vinFolderReceipts = new File(sdcard + "/VIN/receipts");
        if(!vinFolderReceipts.exists()) {
            vinFolderReceipts.mkdirs();
        }

        File vinFolderLogs = new File(sdcard + "/VIN/logs");
        if(!vinFolderLogs.exists()) {
            vinFolderLogs.mkdirs();
        }

        File vinFolderReceived = new File(sdcard + "/VIN/receipts/received");
        if(!vinFolderReceived.exists()) {
            vinFolderReceived.mkdirs();
        }

        File vinFolderSent = new File(sdcard + "/VIN/receipts/sent");
        if(!vinFolderSent.exists()) {
            vinFolderSent.mkdirs();
        }

        Thread.sleep(1000);


        //File defaults = new File(vinFolder, "defaults.cfg");
        //FileOutputStream fos = new FileOutputStream(defaults);

        //File cfg = new File("defaults.cfg");
        //File dst = new File (vinFolder.toString() + "/defaults.cfg");

//        Log.d("###", "cfg: " + cfg + " | dst: " + dst);

        //copy(cfg, dst);


        Thread.sleep(1000);


        Qtoken.run();
    }

    public void put() {
        Qtoken.put();
    }

    public void share() {
        Qtoken.share();
    }

    public void spread() {
        Qtoken.spread();
    }

    public void gather() {
        Qtoken.gather();
    }

    void copy(File src, File dst) throws IOException {
        try (InputStream in = new FileInputStream(src)) {
            try (OutputStream out = new FileOutputStream(dst)) {
                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
            }
        }
    }

//    static {
//
//        System.loadLibrary("main-bridge");
//
////        System.loadLibrary("gnustl_shared");
////        System.loadLibrary("c++_shared");
////
////        System.loadLibrary("config++");
////        System.loadLibrary("stdc++");
////        System.loadLibrary("gcc_s"); // depend libc.so.6
//
////        System.loadLibrary("crypto");
////        System.loadLibrary("ssl");
////
////        System.loadLibrary("boost_context"); // depend of libc++_shared.so
////        System.loadLibrary("boost_fiber");
////        System.loadLibrary("boost_filesystem");
////        System.loadLibrary("boost_regex");
////        System.loadLibrary("boost_system");
////
////        System.loadLibrary("aff3ct-2.3.5");
////
////        System.loadLibrary("kademlia");
////
////        System.loadLibrary("PocoFundation");
////        System.loadLibrary("PocoJSON");
////        System.loadLibrary("PocoNet");
////        System.loadLibrary("PocoUtil");
////        System.loadLibrary("PocoXML");
////
////        System.loadLibrary("qtoken");
//
//        Log.d("###VINTAK", "MainBridge Loaded");
//    }
//
//    public final native String c();
//
//    public final native Object doShare();
//
//    public final native void run();
}



