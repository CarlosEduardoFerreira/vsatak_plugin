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
    static String API_URL = "";

    final static String SERVER_PORT = ":5000";

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

                    Log.i("CarlFerr 4", "PluginApi.API_URL: " + PluginApi.API_URL);

                    JSONObject jsonError = new JSONObject();
                    if(API_URL.equals("")) {
                        jsonError.put("error", "Please configure the Server's ip address.");
                        callback.onResultReceived(jsonError);
                        return;

                    } else if(!Patterns.IP_ADDRESS.matcher(API_URL).matches()) {
                        jsonError.put("error", "Invalid Server's IP Address.");
                        callback.onResultReceived(jsonError);
                        return;
                    }

                    Log.i("CarlFerr", "url: " + url + " | method: " + method + " | params: " + params);

                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.setRequestMethod(method);
                    conn.setRequestProperty("Content-Type", "application/json;charset=UTF-8");
                    conn.setRequestProperty("Accept","application/json");

                    if(method.equals("POST")) {
                        conn.setDoOutput(true);
                        conn.setDoInput(true);

                        if(params != null) {
                            Log.i("JSON", params.toString());
                            DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                            //os.writeBytes(URLEncoder.encode(jsonParam.toString(), "UTF-8"));
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

                    //BufferedReader in = new BufferedReader (new InputStreamReader(conn.getInputStream()));

                    Log.i("CarlFerr", "conn.conn.getErrorStream(): " + conn.getErrorStream());

                    Log.i("CarlFerr", "conn.getResponseCode(): " + conn.getResponseCode());

                    Log.i("CarlFerr", "conn.getResponseMessage(): " + conn.getResponseMessage());

                    Log.i("CarlFerr", "conn.getInputStream(): " + conn.getInputStream());

                    Log.i("CarlFerr", "result: " + result);

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
        send("/insert_marker", "POST", jsonObj, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void listMarkers(final Callback callback) throws JSONException {
        send("/list_markers", "GET", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void getMarker(String uid, final Callback callbackList) throws JSONException {
        JSONObject jsonParam = new JSONObject();
        jsonParam.put("uid", uid);

        send("/get_marker", "POST", jsonParam, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callbackList.onResultReceived(result);
            }
        });
    }


    public static void deleteMarkers(final Callback callback) {
        send("/delete_markers", "GET", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


    public static void checkApiConnection(final Callback callback) {
        send("/check_connection", "GET", null, new Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {
                callback.onResultReceived(result);
            }
        });
    }


}
