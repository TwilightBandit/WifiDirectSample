package org.cocos2dx.cpp;

import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

import static com.loopj.android.http.AsyncHttpClient.log;

/**
 * Created by twilight_bandit on 2018/05/24.
 */

public class WifiController implements WifiP2pManager.PeerListListener, WifiP2pManager.ConnectionInfoListener  {
    public static final int WIFI_P2P_STATE_CHANGED_ACTION = 1;
    public static final int WIFI_P2P_PEERS_CHANGED_ACTION = 2;
    public static final int WIFI_P2P_CONNECTION_CHANGED_ACTION = 3;
    public static final int WIFI_P2P_THIS_DEVICE_CHANGED_ACTION = 4;

    public static final int WIFI_CTL_DISCOVERPEERS_SUCCESS = 10;
    public static final int WIFI_CTL_CONNECT_SUCCESS = 11;
    public static final int WIFI_CTL_DISCONNECT_SUCCESS = 12;

    public static final int WIFI_CTL_DISCOVERPEERS_FAILURE = 101;
    public static final int WIFI_CTL_CONNECT_FAILURE = 102;
    public static final int WIFI_CTL_DISCONNECT_FAILURE = 103;

    private static native void callCocosReceiver(int action, int reason);

    private static WifiP2pManager manager = null;       //Wifi Direct対応デバイス(P2Pデバイス)の情報、接続状態を管理するクラス
    private static WifiP2pManager.Channel channel = null;

    private static ArrayList<WifiP2pDevice> peers = new ArrayList<WifiP2pDevice>();
    private static WifiP2pInfo info = null;
    private static WifiP2pDevice device = null;

    private static boolean isEnabled = false;

    public static final Object lockPeers = new Object();
    public static final Object lockInfo = new Object();

    WifiController(WifiP2pManager manager, WifiP2pManager.Channel channel) {
        this.manager = manager;
        this.channel = channel;
    }

    public static void callCocos(int action, int reason) {
        callCocosReceiver(action, reason);
    }

    @Override
    public void onPeersAvailable(WifiP2pDeviceList wifiP2pDeviceList) {
        synchronized (lockPeers) {
            peers.clear();
            peers.addAll(wifiP2pDeviceList.getDeviceList());
        }

        callCocos(WIFI_P2P_PEERS_CHANGED_ACTION, 0);
    }

    @Override
    public void onConnectionInfoAvailable(WifiP2pInfo wifiP2pInfo) {
        synchronized (lockInfo) {
            info = wifiP2pInfo;
        }

        callCocos(WIFI_P2P_CONNECTION_CHANGED_ACTION, 0);
    }

    public void connectionLost() {
        info = null;
        callCocos(WIFI_P2P_CONNECTION_CHANGED_ACTION, 0);
    }

    public static boolean IsWifiEnabled() {
        return isEnabled;
    }

    public static void setIsEnabled(boolean value) {
        isEnabled = value;

        callCocos(WIFI_P2P_STATE_CHANGED_ACTION, 0);
    }

    public void setWifiP2pDevice(WifiP2pDevice value) {
        device = value;

        callCocos(WIFI_P2P_THIS_DEVICE_CHANGED_ACTION, 0);
    }

    public static String getMyDeviceName() {
        if (device != null) {
            return device.deviceName;
        }

        return "";
    }

    public static String getMyDeviceStatus() {
        if (device != null) {
            switch (device.status) {
                case WifiP2pDevice.AVAILABLE:
                    return "Available";     //利用可能
                case WifiP2pDevice.INVITED:
                    return "Invited";       //招待
                case WifiP2pDevice.CONNECTED:
                    return "Connected";     //接続済み
                case WifiP2pDevice.FAILED:
                    return "Failed";        //失敗
                case WifiP2pDevice.UNAVAILABLE:
                    return "Unavailable";   //無効
                default:
                    return "Unknown";
            }
        }

        return "";
    }

    public static void DiscoverPeers() {
        //他デバイスの検索
        manager.discoverPeers(channel, new WifiP2pManager.ActionListener() {
            @Override
            public void onSuccess() {
                callCocos(WIFI_CTL_DISCOVERPEERS_SUCCESS, 0);
            }

            @Override
            public void onFailure(int reasonCode) {
                callCocos(WIFI_CTL_DISCOVERPEERS_FAILURE, reasonCode);
            }
        });

        return;
    }

    public static String GetPeersList() {
        JSONArray jsonArray = new JSONArray();
        JSONObject jsonRoot = new JSONObject();

        synchronized (lockPeers) {
            try {
                for (WifiP2pDevice wkPeer : peers) {
                    JSONObject kv = new JSONObject();
                    kv.put("name", wkPeer.deviceName);
                    kv.put("address", wkPeer.deviceAddress);
                    jsonArray.put(kv);
                }
                jsonRoot.put("root", jsonArray);
            } catch (JSONException e) {
                //何らかのエラーが発生した場合の処理(ここでは無視する。)
            }
        }

        return  jsonRoot.toString();
    }

    public static void Connect(String address) {
        WifiP2pConfig config = new WifiP2pConfig();
        config.deviceAddress = address;
        config.wps.setup = WpsInfo.PBC;

        manager.connect(channel, config, new WifiP2pManager.ActionListener() {

            @Override
            public void onSuccess() {
                callCocos(WIFI_CTL_CONNECT_SUCCESS, 0);
            }

            @Override
            public void onFailure(int reason) {
                callCocos(WIFI_CTL_CONNECT_FAILURE, reason);
            }
        });
    }

    public static void Disconnect() {
        log.d("WIFIDIRECT","Disconnect start");
        if (device == null || device.status == WifiP2pDevice.CONNECTED) {
            log.d("WIFIDIRECT","Disconnect normal");
            //通常の切断処理
            manager.removeGroup(channel, new WifiP2pManager.ActionListener() {
                @Override
                public void onSuccess() {
                    callCocos(WIFI_CTL_DISCONNECT_SUCCESS, 0);
                }

                @Override
                public void onFailure(int reason) {
                    callCocos(WIFI_CTL_DISCONNECT_FAILURE, reason);
                }
            });
        }
        else if (device.status == WifiP2pDevice.AVAILABLE
                || device.status == WifiP2pDevice.INVITED) {
            log.d("WIFIDIRECT","Disconnect nego or cancel");
            //ネゴシエーション中ノキャンセル
            manager.cancelConnect(channel, new WifiP2pManager.ActionListener() {
                @Override
                public void onSuccess() {
                    callCocos(WIFI_CTL_DISCONNECT_SUCCESS, 0);
                }

                @Override
                public void onFailure(int reason) {
                    callCocos(WIFI_CTL_DISCONNECT_FAILURE, reason);
                }
            });
        }
        else {
            log.d("WIFIDIRECT","Disconnect other");
            //deviceの状態が上記以外の場合には何もしない。(でよいと思う。)
            callCocos(WIFI_CTL_DISCONNECT_SUCCESS, 0);
        }
    }

    public static boolean IsNWInfoEnabled() {
        synchronized (lockInfo) {
            if (info != null) {
                return true;
            }
            else {
                return false;
            }
        }
    }

    public static boolean IsNWInfoGroupOwner() {
        synchronized (lockInfo) {
            if (info != null) {
                return info.isGroupOwner;
            }
            else {
                return false;
            }
        }
    }

    public static String getNWInfoGroupOwnerAddress() {
        synchronized (lockInfo) {
            if (info != null) {
                return info.groupOwnerAddress.getHostAddress();
            }
            else {
                return "";
            }
        }
    }
}
