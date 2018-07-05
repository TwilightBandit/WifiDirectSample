//
// Created by bandit on 2018/05/26.
//

#ifndef PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H
#define PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H

#ifdef __cplusplus
extern "C" {
#endif

	//Wifi���
	bool IsWifiEnabled();

	//�l�b�g���[�N���̎擾
	bool IsNWInfoEnabled();
	bool IsNWInfoGroupOwner();
	std::string getNWInfoGroupOwnerAddress();

	//���[�����̎擾
    std::string getMyDeviceName();
    std::string getMyDeviceStatus();

	//�[�����X�g�̎擾
	std::string GetPeersList();
	
	//����
	void DiscoverPeers();	//���[���̌���

	void ConnectToWifiDirect(std::string address);	//�ڑ�
	void DisconnectFromWifiDirect();	//�ؒf

#ifdef __cplusplus
}
#endif

#endif //PROJ_ANDROID_STUDIO_WIFISTATUSJNI_H
