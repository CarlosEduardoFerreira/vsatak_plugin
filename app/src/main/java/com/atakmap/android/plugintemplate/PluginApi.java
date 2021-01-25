package com.atakmap.android.plugintemplate;

import android.util.Log;
import android.util.Patterns;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;


public class PluginApi {

    final static String HTTP = "http://";
    static String API_URL = "192.168.1.15";

    final static String SERVER_PORT = ":9000";

    // Definition of the callback interface
    public interface Callback {
        void onResultReceived(JSONObject result) throws JSONException;
    }

    private static void send(final String endpoint, final String method,
                             final JSONObject params, final Callback callback) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {

                    final URL url = new URL(HTTP + API_URL + SERVER_PORT + endpoint);

                    JSONObject jsonError = new JSONObject();
                    if(API_URL.equals("")) {
                        jsonError.put("error", "Please configure the Server's IP address.");
                        callback.onResultReceived(jsonError);
                        return;

                    } else if(!Patterns.IP_ADDRESS.matcher(API_URL).matches()) {
                        jsonError.put("error", "Invalid Server's IP Address.");
                        callback.onResultReceived(jsonError);
                        return;
                    }

                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.setRequestMethod(method);
                    conn.setRequestProperty("Content-Type", "application/json;charset=UTF-8");
                    conn.setRequestProperty("Accept","application/json");

                    if(method.equals("POST") || method.equals("DELETE")) {
                        conn.setDoOutput(true);
                        conn.setDoInput(true);

                        if(params != null) {
                            Log.i("JSON", params.toString());
                            DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                            os.writeBytes(params.toString());
                            os.flush();
                            os.close();
                        }
                    }

                    conn.connect();

                    StringBuilder result = new StringBuilder();

                    InputStream is = conn.getInputStream();
                    InputStreamReader inputStreamReader = new InputStreamReader(is);
                    int data = inputStreamReader.read();
                    while(data != -1){
                        char current = (char) data;
                        result.append(current);
                        data = inputStreamReader.read();
                    }
                    is.close();
                    conn.disconnect();

                    JSONObject jsonObj = new JSONObject(result.toString());

                    callback.onResultReceived(jsonObj);

                } catch (Exception e) {
                    try {
                        JSONObject jsonError = new JSONObject();
                        jsonError.put("error", "Connection failed.");

                        callback.onResultReceived(jsonError);

                    } catch (JSONException jsonException) {
                        jsonException.printStackTrace();
                    }

                    e.printStackTrace();
                }
            }
        });

        thread.start();
    }


    public static void insertMarker(JSONObject jsonObj, final Callback callback) {
        send("/upload_point", "POST", jsonObj, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void listMarkers(final Callback callback) throws JSONException {
        send("/list_files", "GET", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void getMarker(String uid, final Callback callbackList) throws JSONException {
        JSONObject jsonParam = new JSONObject();
        jsonParam.put("uid", uid);

        send("/download_data", "POST", jsonParam, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callbackList.onResultReceived(result);
            }
        });
    }


    public static void deleteMarkers(final Callback callback) {
        send("/delete_file", "DELETE", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void checkApiConnection(final Callback callback) {
        send("/ping", "GET", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


}
