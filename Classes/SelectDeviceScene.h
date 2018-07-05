//
// Created by bandit on 2018/05/15.
//

#ifndef PROJ_ANDROID_STUDIO_SELECTDEVICESCENE_H
#define PROJ_ANDROID_STUDIO_SELECTDEVICESCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "IWifiReceiver.h"

class SelectDeviceScene : public cocos2d::Scene, public IWifiReceiver {
//定義
protected:
	enum SceneStatus {
		NONE,
		WAIT,
		SEARCH,
		CONNECTING,
		CONNECTED,
		ERR,
	};

	class PeerData {
	public:
		std::string name;
		std::string address;
	};

//コンストラクタ・デストラクタ
private:
    SelectDeviceScene();
    ~SelectDeviceScene();

//操作
public:
    static SelectDeviceScene * create();

private:
	bool init();

	void onReceive(int action, int reason)  override;

    void setSceneStatus(SceneStatus status);
	SceneStatus calcNextStatus();

	void dispThisDeviceInfo();
	void dispNetworkInfo();

    void clearDeviceList();
    void dispDeviceListView();
    bool parsePeerData(const std::string data);
	void peersChangedAction();

	void backButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void findButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void findButtonTimeout(float dt);
	void disconnectButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void cancelConnectButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type);

	void listviewCallback(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type);


//変数
private:
	std::vector<PeerData *> _deviceList;
	SceneStatus _status;
};


#endif //PROJ_ANDROID_STUDIO_SELECTDEVICESCENE_H
