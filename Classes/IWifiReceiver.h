//
// Created by bandit on 2018/05/29.
//

#ifndef PROJ_ANDROID_STUDIO_IWIFIRECEIVER_H
#define PROJ_ANDROID_STUDIO_IWIFIRECEIVER_H

class IWifiReceiver  {
public:
    const int WIFI_P2P_STATE_CHANGED_ACTION = 1;
    const int WIFI_P2P_PEERS_CHANGED_ACTION = 2;
    const int WIFI_P2P_CONNECTION_CHANGED_ACTION = 3;
    const int WIFI_P2P_THIS_DEVICE_CHANGED_ACTION = 4;

    const int WIFI_CTL_DISCOVERPEERS_SUCCESS = 10;
	const int WIFI_CTL_CONNECT_SUCCESS = 11;
	const int WIFI_CTL_DISCONNECT_SUCCESS = 12;

    const int WIFI_CTL_DISCOVERPEERS_FAILURE = 101;
	const int WIFI_CTL_CONNECT_FAILURE = 102;
	const int WIFI_CTL_DISCONNECT_FAILURE = 103;

public:
    virtual void onReceive(int action, int reason) = 0;
};

#endif //PROJ_ANDROID_STUDIO_IWIFIRECEIVER_H
