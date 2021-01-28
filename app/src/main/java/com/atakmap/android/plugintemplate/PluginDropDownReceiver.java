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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
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


        // Add Marker - Upload new Point
        final ImageButton addButton = templateView.findViewById(R.id.add_point);
        addButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setTitleText();

                // Create a Marker
                final Marker m = setupMarker();
                String uid = m.getUID();
                double latitude = m.getPoint().getLatitude();
                double longitude = m.getPoint().getLongitude();
                Log.i("Sal", "latitude: " + latitude + " | longitude: " + longitude);

                addMarkerToTheMap(m);
                markers.add(m);
                updateListViewMarkers(listViewMarkers, markers.size() == 1);

                // Send to API
                try {
                    JSONObject jsonObj = new JSONObject();
                    jsonObj.put("uid", uid);
                    jsonObj.put("latitude", latitude);
                    jsonObj.put("longitude", longitude);

                    // Send Marker to the API
                    PluginApi.insertMarker(jsonObj, new PluginApi.Callback() {
                        public void onResultReceived(JSONObject result) throws JSONException {
                            noErrorToast(result.getString("error"), "New Marker added.");
                        }
                    });

                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        });


        // Get Point - Download Point by UID
        final ImageButton downloadButton = templateView.findViewById(R.id.add_uid);
        downloadButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    checkConnection(new PluginApi.Callback() {
                        public void onResultReceived(JSONObject result) throws JSONException {

                            if (noErrorToast(result.getString("error"), "")) {

                                final AlertDialog.Builder builder = new AlertDialog.Builder(PluginTemplateLifecycle.mainActivity);
                                builder.setTitle("Point Name");

                                // Setup TextView
                                final EditText input = new EditText(context);
                                int maxLength = 6;
                                InputFilter[] fArray = new InputFilter[1];
                                fArray[0] = new InputFilter.LengthFilter(maxLength);
                                input.setFilters(fArray);
                                input.setInputType(InputType.TYPE_CLASS_TEXT);
                                builder.setView(input);

                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        final String uid = input.getText().toString();
                                        if (uid.length() != 6) {
                                            Toast.makeText(mainContext,
                                                    "Point Name does not have 6 characters.",
                                                    Toast.LENGTH_LONG).show();
                                        } else {
                                            try {
                                                PluginApi.getMarker(uid, new PluginApi.Callback() {
                                                    public void onResultReceived(JSONObject result) throws JSONException {

                                                        if(result.has("uid")) {

                                                            // Show only the selected marker at the map
                                                            cleanupMap();
                                                            String juid = result.getString("uid");
                                                            double latitude = result.getDouble("latitude");
                                                            double longitude = result.getDouble("longitude");
                                                            setMarker(juid, latitude, longitude);

                                                            // Remove not selected markers from the ListView
                                                            for (int i = markers.size() - 1; i >= 0; i--) {
                                                                Log.d("VINTAK", "markers.get(i).getUID(): " +
                                                                        markers.get(i).getUID() + " | juid: " + juid);
                                                                if (!markers.get(i).getUID().equals(juid)) {
                                                                    markers.remove(markers.get(i));
                                                                }
                                                            }
                                                            updateListViewMarkers(listViewMarkers, false);

                                                        } else {
                                                            String txt = "ID not found. (" + uid + ")";
                                                            Toast.makeText(context, txt, Toast.LENGTH_SHORT).show();
                                                        }

                                                    }
                                                });
                                            } catch (JSONException e) {
                                                e.printStackTrace();
                                            }
                                        }
                                    }
                                });
                                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        dialog.cancel();
                                    }
                                });

                                // Loading at main Thread
                                Handler mainHandler = new Handler(Looper.getMainLooper());
                                Runnable myRunnable = new Runnable() {
                                    @Override
                                    public void run() {
                                        builder.show();
                                    }
                                };
                                mainHandler.post(myRunnable);
                            }
                        }
                    });
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        });


        // List Markers - Download all Points
        final ImageButton listButton = templateView.findViewById(R.id.list_markers);
        listButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    downloadMarkersAndUpdateListView();
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            }
        });


        // Delete all Points
        clearButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    checkConnection(new PluginApi.Callback() {
                        public void onResultReceived(JSONObject result) throws JSONException {
                            if (noErrorToast(result.getString("error"), "")) {

                                final AlertDialog.Builder builder = new AlertDialog.Builder(PluginTemplateLifecycle.mainActivity);
                                builder.setTitle("Are you sure you want to DELETE all Points?");

                                builder.setPositiveButton("Yes, I am.", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {

                                        PluginApi.deleteMarkers(new PluginApi.Callback() {
                                            public void onResultReceived(JSONObject result) throws JSONException {
                                                if (noErrorToast(result.getString("error"), "")) {
                                                    getMapView().getRootGroup().findMapGroup(GROUP_NAME).clearItems();

                                                    Handler mainHandler = new Handler(Looper.getMainLooper());
                                                    Runnable myRunnable = new Runnable() {
                                                        @Override
                                                        public void run() {
                                                            markers = new ArrayList<Marker>();
                                                            updateListViewMarkers(listViewMarkers, true);
                                                        }
                                                    };
                                                    mainHandler.post(myRunnable);
                                                }
                                            }
                                        });
                                    }
                                });
                                builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        dialog.cancel();
                                    }
                                });

                                // Loading at main Thread
                                Handler mainHandler = new Handler(Looper.getMainLooper());
                                Runnable myRunnable = new Runnable() {
                                    @Override
                                    public void run() {
                                        builder.show();
                                    }
                                };
                                mainHandler.post(myRunnable);
                            }
                        }
                    });
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            }
        });


        // Settings
        final ImageButton settingsButton = templateView.findViewById(R.id.settings);

        settingsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final AlertDialog.Builder builder = new AlertDialog.Builder(mainContext);
                builder.setTitle("Server IP Address");

                final EditText input = new EditText(context);
                input.setInputType(InputType.TYPE_CLASS_TEXT);
                input.setText(PluginApi.API_URL);
                builder.setView(input);

                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        final String ip = input.getText().toString();
                        PluginApi.API_URL = ip;
                        Log.i("CarlFerr 2", "PluginApi.API_URL: " + PluginApi.API_URL);
                        try {
                            checkConnection(new PluginApi.Callback() {
                                public void onResultReceived(JSONObject result) throws JSONException {
                                    if (!noErrorToast(result.getString("error"), result.getString("success"))) {
                                        settingsButton.performClick();
                                    } else {
                                        noErrorToast("", "Connection OK");
                                    }
                                }
                            });
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                });
                builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                });

                // Loading at main Thread
                Handler mainHandler = new Handler(Looper.getMainLooper());
                Runnable myRunnable = new Runnable() {
                    @Override
                    public void run() {
                        builder.show();
                    }
                };
                mainHandler.post(myRunnable);
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
                        getMapView().getRootGroup().findMapGroup(GROUP_NAME).clearItems();

                        Marker m = markers.get(position);
                        setMarker(m.getUID(), m.getPoint().getLatitude(), m.getPoint().getLongitude());

                        Toast.makeText(mainContext, m.getUID(), Toast.LENGTH_SHORT).show();
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
