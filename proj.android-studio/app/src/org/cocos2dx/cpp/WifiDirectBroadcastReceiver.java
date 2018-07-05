package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/**
 * Created by bandit on 2018/05/22.
 */

public class WifiDirectBroadcastReceiver extends BroadcastReceiver {
    private WifiP2pManager manager;
    private WifiP2pManager.Channel channel;
    private WifiController controller;

    public WifiDirectBroadcastReceiver(WifiP2pManager manager, WifiP2pManager.Channel channel, WifiController controller) {
        super();
        this.manager = manager;
        this.channel = channel;
        this.controller = controller;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
            //Wifi Directの有効/無効状態通知
            //本体デバイスの機能そのものの利用可能・不可や、機能制限、Wifi Direct有効化を促すのに利用します。
            int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);
            if (state == WifiP2pManager.WIFI_P2P_STATE_ENABLED) {
                //Wifi Direct有効
                controller.setIsEnabled(true);
            }
            else {
                //Wifi Direct無効
                controller.setIsEnabled(false);
            }
        }
        else if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {
            //デバイス情報の変更通知(通信可能なデバイスの発見、ロストなど)
            //Wifi Direct通信ができるデバイスに変更があった時に呼ばれます。
            //デバイス一覧を更新したり、通信相手を切り替えるトリガとして利用します。

            if (manager != null) {
                manager.requestPeers(channel, controller);  //新たなデバイス一覧を取得
            }
        }
        else if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)) {
            //IPアドレスなどコネクション情報。通信状態の変更通知。

            if (manager == null) {
                return;
            }

            NetworkInfo networkInfo = (NetworkInfo) intent.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);
            if (networkInfo.isConnected()) {
                //接続された
                manager.requestConnectionInfo(channel, controller);
            }
            else {
                //切断された
                controller.connectionLost();
            }

        }
        else if (WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION.equals(action)) {
            //自分自身のデバイス状態の変更通知。(相手デバイスではない。)

            //自端末の情報を更新する
            controller.setWifiP2pDevice((WifiP2pDevice)intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_DEVICE));
        }
    }
}
