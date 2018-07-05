//
// Created by bandit on 2018/05/26.
//

#ifndef PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H
#define PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H

#ifdef __cplusplus
extern "C" {
#endif

	//Wifi状態
	bool IsWifiEnabled();

	//ネットワーク情報の取得
	bool IsNWInfoEnabled();
	bool IsNWInfoGroupOwner();
	std::string getNWInfoGroupOwnerAddress();

	//自端末情報の取得
    std::string getMyDeviceName();
    std::string getMyDeviceStatus();

	//端末リストの取得
	std::string GetPeersList();
	
	//操作
	void DiscoverPeers();	//他端末の検索

	void ConnectToWifiDirect(std::string address);	//接続
	void DisconnectFromWifiDirect();	//切断

#ifdef __cplusplus
}
#endif

#endif //PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H
