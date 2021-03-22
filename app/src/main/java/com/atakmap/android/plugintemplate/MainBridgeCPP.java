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


import android.util.Log;


public class MainBridgeCPP {



    public void load() {
        Qtoken.run();
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



