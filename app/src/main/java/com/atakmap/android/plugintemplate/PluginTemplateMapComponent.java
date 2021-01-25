package com.atakmap.android.plugintemplate;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;

import com.atakmap.android.ipc.AtakBroadcast.DocumentedIntentFilter;

import com.atakmap.android.maps.MapView;
import com.atakmap.android.dropdown.DropDownMapComponent;

import com.atakmap.android.maps.Marker;
import com.atakmap.android.plugintemplate.plugin.PluginTemplateLifecycle;
import com.atakmap.coremap.log.Log;
import com.atakmap.android.plugintemplate.plugin.R;
import com.atakmap.coremap.maps.coords.GeoPoint;

import org.json.JSONException;

import static com.atakmap.android.maps.MapView._mapView;

public class PluginTemplateMapComponent extends DropDownMapComponent {

    private static final String TAG = "PluginTemplateMapComponent";

    private Context pluginContext;
    private static MapView mapView = null;

    private PluginDropDownReceiver ddr;

    public final static MapView getMapView() {
        return _mapView;
    }

    public void onCreate(final Context context, Intent intent,
                         final MapView view) {

        context.setTheme(R.style.ATAKPluginTheme);
        super.onCreate(context, intent, view);
        pluginContext = context;
        mapView = view;

        try {
            ddr = new PluginDropDownReceiver(
                    view, context);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Log.d(TAG, "registering the plugin filter");
        DocumentedIntentFilter ddFilter = new DocumentedIntentFilter();
        ddFilter.addAction(PluginDropDownReceiver.SHOW_PLUGIN);
        registerDropDownReceiver(ddr, ddFilter);
    }


    public static void setMarker2() {
        GeoPoint gp = new GeoPoint(28.534716261864375, -81.53335078544796);
        long id = 2345;
        Marker m = new Marker(id, gp, "2345");
        mapView.setSelfMarker(m);
    }


    public static void showAlert() {
        AlertDialog alertDialog = new AlertDialog.Builder(PluginTemplateLifecycle.mainActivity).create();
        alertDialog.setTitle("Alert");
        alertDialog.setMessage("Alert message to be shown");
        alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
        alertDialog.show();
    }

    @Override
    protected void onDestroyImpl(Context context, MapView view) {
        super.onDestroyImpl(context, view);
    }

}
