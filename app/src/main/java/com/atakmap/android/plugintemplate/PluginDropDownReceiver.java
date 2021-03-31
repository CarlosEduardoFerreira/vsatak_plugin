package com.atakmap.android.plugintemplate;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;
import android.text.InputFilter;
import android.text.InputType;
import android.util.Patterns;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Toast;

import com.atak.plugins.impl.PluginLayoutInflater;
import com.atakmap.android.maps.MapView;
import com.atakmap.android.maps.Marker;
import com.atakmap.android.plugintemplate.plugin.PluginTemplateLifecycle;
import com.atakmap.android.plugintemplate.plugin.R;
import com.atakmap.android.dropdown.DropDown.OnStateListener;
import com.atakmap.android.dropdown.DropDownReceiver;

import com.atakmap.coremap.log.Log;
import com.atakmap.coremap.maps.coords.GeoPoint;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import java.util.UUID;

import static android.content.Context.MODE_PRIVATE;


public class PluginDropDownReceiver extends DropDownReceiver implements
        OnStateListener {

    public static final String TAG = PluginDropDownReceiver.class.getSimpleName();

    public static final String SHOW_PLUGIN = "com.atakmap.android.plugintemplate.SHOW_PLUGIN";
    public final View templateView;

    Context mainContext = PluginTemplateLifecycle.mainActivity;

    // ArrayList containing all markers
    ArrayList<Marker> markers = new ArrayList<Marker>();

    // UIDs used to populate the ListView
    private List<String> markersUids() {
        List<String> uids = new ArrayList<String>();
        for (Marker marker : markers) {
            uids.add(marker.getUID());
        }
        return uids;
    }


    public final String GROUP_NAME = "Sal";


    /**************************** CONSTRUCTOR *****************************/

    public PluginDropDownReceiver(final MapView mapView,
                                  final Context context) throws JSONException {
        super(mapView);

        templateView = PluginLayoutInflater.inflate(context, R.layout.main_layout, null);
        final ImageButton clearButton = templateView.findViewById(R.id.clear_points);
        final ListView listViewMarkers = templateView.findViewById(R.id.list_view_markers);

        SharedPreferences prefs = mainContext.getSharedPreferences("server", MODE_PRIVATE);
        PluginApi.API_URL = prefs.getString("ip", "");

        getMapView().getRootGroup().addGroup(GROUP_NAME);


        downloadMarkersAndUpdateListView();


        // RUN
        final ImageButton addButton = templateView.findViewById(R.id.add_point);
        addButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread( new Runnable() { @Override public void run() {
                    MainBridgeCPP bridge = new MainBridgeCPP();
                    Log.d("###QTOKEN", "PluginDropDownReceiver run");
                    try {
                        bridge.run();
                    } catch (InterruptedException | IOException e) {
                        e.printStackTrace();
                    }
                } } ).start();
            }
        });


        // PUT
        final ImageButton downloadButton = templateView.findViewById(R.id.add_uid);
        downloadButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread( new Runnable() { @Override public void run() {
                    MainBridgeCPP bridge = new MainBridgeCPP();
                    Log.d("###QTOKEN", "PluginDropDownReceiver put");
                    bridge.put();
                } } ).start();
            }
        });


        // SHARE
        final ImageButton listButton = templateView.findViewById(R.id.list_markers);
        listButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread( new Runnable() { @Override public void run() {
                    MainBridgeCPP bridge = new MainBridgeCPP();
                    Log.d("###QTOKEN", "PluginDropDownReceiver share");
                    bridge.share();
                } } ).start();
            }
        });


        // SPREAD
        clearButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread( new Runnable() { @Override public void run() {
                    MainBridgeCPP bridge = new MainBridgeCPP();
                    Log.d("###QTOKEN", "PluginDropDownReceiver Spread");
                    bridge.spread();
                } } ).start();
            }
        });


        // GATHER
        final ImageButton settingsButton = templateView.findViewById(R.id.settings);
        settingsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread( new Runnable() { @Override public void run() {
                    MainBridgeCPP bridge = new MainBridgeCPP();
                    Log.d("###QTOKEN", "PluginDropDownReceiver Gather");
                    bridge.gather();
                } } ).start();
            }
        });

    }


    private void updateListViewMarkers(final ListView listViewMarkers, final boolean check) {
        // Loading at main thread
        Handler mainHandler = new Handler(Looper.getMainLooper());
        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {

                if (check) {
                    final LinearLayout layoutStart = templateView.findViewById(R.id.layout_start);
                    final LinearLayout layoutList = templateView.findViewById(R.id.layout_list_view);

                    if (markers.size() == 0) {
                        fadeOut(layoutList);
                        fadeIn(layoutStart);
                    } else {
                        fadeOut(layoutStart);
                        fadeIn(layoutList);
                    }
                }

                ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(
                        PluginTemplateMapComponent.getMapView().getContext(),
                        android.R.layout.simple_list_item_1, markersUids());
                listViewMarkers.setAdapter(arrayAdapter);

                listViewMarkers.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                        /*getMapView().getRootGroup().findMapGroup(GROUP_NAME).clearItems();

                        Marker m = markers.get(position);
                        setMarker(m.getUID(), m.getPoint().getLatitude(), m.getPoint().getLongitude());
                        */
                        // call cpp here for testing only

                        new Thread( new Runnable() { @Override public void run() {
                            MainBridgeCPP bridge = new MainBridgeCPP();
                                Log.d("###QTOKEN", "PluginDropDownReceiver put");
                                    bridge.put();
                        } } ).start();

                        //String txt = (String) bridge.doShare();
                        //String txt = bridge.stringFromCPP();

                        //Toast.makeText(mainContext, txt, Toast.LENGTH_SHORT).show();
                    }
                });
            }
        };
        mainHandler.post(myRunnable);
    }


    private void fadeIn(LinearLayout layout) {
        AlphaAnimation anim = new AlphaAnimation(0.0f, 1.0f);
        anim.setDuration(1000);
        layout.startAnimation(anim);
        layout.setVisibility(View.VISIBLE);
    }

    private void fadeOut(LinearLayout layout) {
        AlphaAnimation anim = new AlphaAnimation(1.0f, 0.0f);
        anim.setDuration(300);
        layout.startAnimation(anim);
        layout.setVisibility(View.GONE);
    }

    /**
     * Check ip format and connection to the Server's API
     */
    private void checkConnection(final PluginApi.Callback callback) throws JSONException {
        final JSONObject jsonError = new JSONObject();
        jsonError.put("success", "");
        jsonError.put("error", "");

        if (PluginApi.API_URL.equals("")) {
            jsonError.put("error", "Please configure the Server's ip address.");
            callback.onResultReceived(jsonError);

        } else if (!Patterns.IP_ADDRESS.matcher(PluginApi.API_URL).matches()) {
            jsonError.put("error", "Invalid Server's IP Address.");
            callback.onResultReceived(jsonError);

        } else {
            PluginApi.checkApiConnection(new PluginApi.Callback() {

                public void onResultReceived(JSONObject result) throws JSONException {
                    String error = result.getString("error");

                    if (error.equals("")) {
                        jsonError.put("success", "Connection OK");

                        SharedPreferences prefs = mainContext.getSharedPreferences("server", MODE_PRIVATE);
                        SharedPreferences.Editor edit = prefs.edit();
                        edit.putString("ip", PluginApi.API_URL).apply();
                    }

                    callback.onResultReceived(jsonError);
                }
            });

        }
    }

    /**
     * downloadMarkersAndUpdateListView()
     * Download Markers from the API and
     * update the ListView "listViewMarkers"
     *
     * no parameters
     * no return
     */
    private void downloadMarkersAndUpdateListView() throws JSONException {

        final ListView listViewMarkers = templateView.findViewById(R.id.list_view_markers);

        PluginApi.listMarkers(new PluginApi.Callback() {
            public void onResultReceived(JSONObject result) throws JSONException {

                markers = new ArrayList<Marker>();
                cleanupMap();

                Iterator<String> iter = result.keys();
                while (iter.hasNext()) {
                    String uid = iter.next();
                    try {
                        JSONObject obj = result.getJSONObject(uid);
                        double latitude = obj.getDouble("latitude");
                        double longitude = obj.getDouble("longitude");

                        GeoPoint gp = new GeoPoint(latitude, longitude);
                        Marker m = new Marker(gp, uid);
                        markers.add(m);

                    } catch (JSONException e) {
                        noErrorToast("The Plugin cannot load this data.", "");
                    }
                }

                updateListViewMarkers(listViewMarkers, true);

                updateMapWithMarkers();
            }
        });
    }


    /**
     * noErrorToast() check the variable error,
     * show Toast and return a boolean
     *
     * @param error   = information/text from the API
     * @param success = local message to show to the user
     * @return = False if the API returns error, otherwise True
     */
    private static boolean noErrorToast(String error, String success) {
        final String msg = error.equals("") ? success : error;

        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Context context = PluginTemplateMapComponent.getMapView().getContext();
                if (!msg.equals(""))
                    Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
            }
        });

        return error.equals("");
    }


    private void updateMapWithMarkers() {
        for (Marker m : markers) {
            String uid = m.getUID();
            double latitude = m.getPoint().getLatitude();
            double longitude = m.getPoint().getLongitude();

            setMarker(uid, latitude, longitude);
        }
    }


    public Marker setupMarker() {
        String uid = UUID.randomUUID().toString().substring(0, 6);

        double latitude = getRandomValue(30, 60, 1);
        double longitude = getRandomValue(70, 120, -1); // negative

        return setupMarker(uid, latitude, longitude);
    }


    public Marker setupMarker(String uid, double latitude, double longitude) {
        GeoPoint gp = new GeoPoint(latitude, longitude);

        Marker m = new Marker(gp, uid);

        Log.i(TAG, "setMarker: " + m.getUID() + " | latitude: " + latitude + " | longitude: " + longitude);

        //Icon i = new Icon("@android:drawable/marker_red.png");
        //m.setIcon(i);
        m.setType("b");
        m.setMetaString("entry", "user");
        m.setMetaString("callsign", "VSATAK");
        m.setTitle("VSATAK");/**/

        return m;
    }


    private void addMarkerToTheMap(Marker m) {
        getMapView().getRootGroup().findMapGroup(GROUP_NAME).addItem(m);

        //markers.add(m);

        //final ListView listViewMarkers = templateView.findViewById(R.id.list_view_markers);
        //updateListViewMarkers(listViewMarkers, true);
    }


    public Marker setMarker() {
        Marker m = setupMarker();
        addMarkerToTheMap(m);

        return m;
    }

    public Marker setMarker(String uid, double latitude, double longitude) {

        Marker m = setupMarker(uid, latitude, longitude);
        addMarkerToTheMap(m);

        return m;


    }


    private void cleanupMap() {
        getMapView().getRootGroup().findMapGroup(GROUP_NAME).clearItems();
    }


    void setTitleText() {
        //((TextView)templateView.findViewById(R.id.plugin_title)).setText("Markers: " + getNextInt());
    }


    public static Double getRandomValue(final int lower, final int higher, final int multiplier) {

        int decimalPlaces = 15;

        final Random random = new Random();

        if (lower < 0 || higher <= lower || decimalPlaces < 0) {
            throw new IllegalArgumentException("Wrong parameter!");
        }

        final double dbl = ((random == null ? new Random() : random).nextDouble()
                * (higher - lower))
                + lower;
        return Double.parseDouble(String.format("%." + decimalPlaces + "f", dbl)) * multiplier;

    }


    public void disposeImpl() {
    }

    @Override
    public void onReceive(Context context, Intent intent) {

        final String action = intent.getAction();
        if (action == null)
            return;

        if (action.equals(SHOW_PLUGIN)) {
            Log.d(TAG, "showing plugin drop down");
            showDropDown(templateView, 0.325D, FULL_HEIGHT, FULL_WIDTH,
                    HALF_HEIGHT, false);
        }
    }

    @Override
    public void onDropDownSelectionRemoved() {
    }

    @Override
    public void onDropDownVisible(boolean v) {
    }

    @Override
    public void onDropDownSizeChanged(double width, double height) {
    }

    @Override
    public void onDropDownClose() {
    }

}
