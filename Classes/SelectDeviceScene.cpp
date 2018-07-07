//
// Created by twilight_bandit on 2018/05/15.
//
#include "json/rapidjson.h"
#include "json/document.h"

#include "SelectDeviceScene.h"
#include "HelloWorldScene.h"
#include "WifiStatusJni.h"
#include "WifiReceiveController.h"

USING_NS_CC;

#define LABEL_MESSAGE 1
#define LABEL_ERROR 2
#define LABEL_MY_DEVICE_INFO 3
#define LABEL_MY_DEVICE_NAME 4
#define LABEL_MY_DEVICE_STATUS 5
#define LABEL_GROUP_OWNER 6
#define LABEL_GROUP_OWNER_ADDRESS 7

#define BUTTON_FIND 20
#define BUTTON_DISCONNECT 21
#define BUTTON_CANCELCONNECT 22
#define BUTTON_BACK 23

#define LISTVIEW_DEVICES 30

SelectDeviceScene::SelectDeviceScene()
	: Scene()
	, _status(SceneStatus::NONE)
{
}

SelectDeviceScene::~SelectDeviceScene() {
    clearDeviceList();
}

SelectDeviceScene * SelectDeviceScene::create()
{
	SelectDeviceScene * pRet = new SelectDeviceScene();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool SelectDeviceScene::init()
{
	if (!Scene::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithSystemFont("Select Device", "HiraKakuProN-W3", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    auto labelMessage = Label::createWithSystemFont("", "HiraKakuProN-W3", 24);
	labelMessage->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(labelMessage, 1, LABEL_MESSAGE);

	auto labelError = Label::createWithSystemFont("ERROR", "HiraKakuProN-W3", 24);
	labelError->setPosition(Vec2(origin.x + visibleSize.width / 2, 50));
	this->addChild(labelError, 1, LABEL_ERROR);

    auto labelMyDeviceInfo = Label::createWithSystemFont("自端末情報", "HiraKakuProN-W3", 12);
	labelMyDeviceInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
    labelMyDeviceInfo->setPosition(Vec2(10, visibleSize.height - 5));
    this->addChild(labelMyDeviceInfo, 1, LABEL_MY_DEVICE_INFO);

	auto labelMyDeviceName = Label::createWithSystemFont("", "HiraKakuProN-W3", 12);
	labelMyDeviceName->setAnchorPoint(Vec2(0.0f, 1.0f));
	labelMyDeviceName->setPosition(Vec2(10, visibleSize.height - 20));
	labelMyDeviceName->setString("");
	this->addChild(labelMyDeviceName, 1, LABEL_MY_DEVICE_NAME);

	auto labelMyDeviceStatus = Label::createWithSystemFont("", "HiraKakuProN-W3", 12);
	labelMyDeviceStatus->setAnchorPoint(Vec2(0.0f, 1.0f));
	labelMyDeviceStatus->setPosition(Vec2(10, visibleSize.height - 35));
	labelMyDeviceStatus->setString("");
	this->addChild(labelMyDeviceStatus, 1, LABEL_MY_DEVICE_STATUS);

	auto labelGroupOwner = Label::createWithSystemFont("", "HiraKakuProN-W3", 12);
	labelGroupOwner->setAnchorPoint(Vec2(0.0f, 1.0f));
	labelGroupOwner->setPosition(Vec2(10, visibleSize.height - 50));
	labelGroupOwner->setString("");
	this->addChild(labelGroupOwner, 1, LABEL_GROUP_OWNER);

	auto labelGroupOwnerAddress = Label::createWithSystemFont("", "HiraKakuProN-W3", 12);
	labelGroupOwnerAddress->setAnchorPoint(Vec2(0.0f, 1.0f));
	labelGroupOwnerAddress->setPosition(Vec2(10, visibleSize.height - 65));
	labelGroupOwnerAddress->setString("");
	this->addChild(labelGroupOwnerAddress, 1, LABEL_GROUP_OWNER_ADDRESS);

    auto findButton = ui::Button::create();
    findButton->setTouchEnabled(true);
    findButton->setTitleText("FIND");
    findButton->setTitleColor(Color3B::BLUE);
    findButton->setTitleFontSize(25);
    findButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    findButton->setPosition(Vec2(10, visibleSize.height - 85));
    findButton->addTouchEventListener(CC_CALLBACK_2(SelectDeviceScene::findButtonTouchEventListener, this));
    this->addChild(findButton, 1, BUTTON_FIND);

	auto disconnectButton = ui::Button::create();
	disconnectButton->setTouchEnabled(true);
	disconnectButton->setTitleText("DISCONNECT");
	disconnectButton->setTitleColor(Color3B::BLUE);
	disconnectButton->setTitleFontSize(25);
	disconnectButton->setAnchorPoint(Vec2(0.0f, 1.0f));
	disconnectButton->setPosition(Vec2(10, visibleSize.height - 105));
	disconnectButton->addTouchEventListener(CC_CALLBACK_2(SelectDeviceScene::disconnectButtonTouchEventListener, this));
	this->addChild(disconnectButton, 1, BUTTON_DISCONNECT);

	auto cancelConnectButton = ui::Button::create();
	cancelConnectButton->setTouchEnabled(true);
	cancelConnectButton->setTitleText("CANCEL");
	cancelConnectButton->setTitleColor(Color3B::BLUE);
	cancelConnectButton->setTitleFontSize(25);
	cancelConnectButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	cancelConnectButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 160));
	cancelConnectButton->addTouchEventListener(CC_CALLBACK_2(SelectDeviceScene::cancelConnectButtonTouchEventListener, this));
	this->addChild(cancelConnectButton, 1, BUTTON_CANCELCONNECT);

	auto backButton = ui::Button::create();
	backButton->setTouchEnabled(true);
	backButton->setTitleText("BACK");
	backButton->setTitleColor(Color3B::BLUE);
	backButton->setTitleFontSize(25);
	float x = origin.x + visibleSize.width - backButton->getContentSize().width / 2;
	float y = origin.y + backButton->getContentSize().height / 2;
	backButton->setPosition(Vec2(x, y));
	backButton->addTouchEventListener(CC_CALLBACK_2(SelectDeviceScene::backButtonTouchEventListener, this));
	this->addChild(backButton, 1, BUTTON_BACK);

	auto deviceListview = cocos2d::ui::ListView::create();
    deviceListview->setContentSize(cocos2d::Size(300, 150));
    deviceListview->setAnchorPoint(Vec2(0.0f, 1.0f));
    deviceListview->setPosition(Vec2(150, 250));
    deviceListview->setTouchEnabled(true);
    deviceListview->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(SelectDeviceScene::listviewCallback, this));
    addChild(deviceListview, 1, LISTVIEW_DEVICES);

	//画面statusの設定
	setSceneStatus(calcNextStatus());

	//自端末情報の表示
	dispThisDeviceInfo();

	//ネットワーク情報の表示
	dispNetworkInfo();

	//Device一覧表示の更新
	dispDeviceListView();

	//Broadcast受け取り開始
	WifiReceiveController::setReceiver(this);

    return true;
}

void SelectDeviceScene::onReceive(int action, int reason) {
	if (action == WIFI_P2P_STATE_CHANGED_ACTION) {
		setSceneStatus(calcNextStatus());
	}
	else if (action == WIFI_P2P_PEERS_CHANGED_ACTION) {
		peersChangedAction();
	}
	else if (action == WIFI_P2P_CONNECTION_CHANGED_ACTION) {
		setSceneStatus(calcNextStatus());
		dispThisDeviceInfo();
		dispNetworkInfo();
	}
	else if (action == WIFI_P2P_THIS_DEVICE_CHANGED_ACTION) {
		dispThisDeviceInfo();
		dispNetworkInfo();
	}
	else if (action == WIFI_CTL_DISCOVERPEERS_SUCCESS) {
		//成功時は処理なし
	}
	else if (action == WIFI_CTL_CONNECT_SUCCESS) {
		//成功時は処理なし
	}
	else if (action == WIFI_CTL_DISCONNECT_SUCCESS) {
		//成功時は処理なし
	}
	else if (action == WIFI_CTL_DISCOVERPEERS_FAILURE) {
		setSceneStatus(SceneStatus::ERR);
	}
	else if (action = WIFI_CTL_CONNECT_FAILURE) {
		setSceneStatus(SceneStatus::ERR);
	}
	else if (action = WIFI_CTL_DISCONNECT_FAILURE) {
		setSceneStatus(SceneStatus::ERR);
	}
}

void SelectDeviceScene::setSceneStatus(SceneStatus status) {
	auto labelMessage = this->getChildByTag<Label *>(LABEL_MESSAGE);
	auto labelError = this->getChildByTag<Label *>(LABEL_ERROR);
	auto labelMyDeviceInfo = this->getChildByTag<Label *>(LABEL_MY_DEVICE_INFO);
	auto labelMyDeviceName = this->getChildByTag<Label *>(LABEL_MY_DEVICE_NAME);
	auto labelMyDeviceStatus = this->getChildByTag<Label *>(LABEL_MY_DEVICE_STATUS);
	auto labelGroupOwner = this->getChildByTag<Label *>(LABEL_GROUP_OWNER);
	auto labelGroupOwnerAddress = this->getChildByTag<Label *>(LABEL_GROUP_OWNER_ADDRESS);
	auto findButton = this->getChildByTag<ui::Button *>(BUTTON_FIND);
	auto deviceListview = this->getChildByTag<ui::ListView *>(LISTVIEW_DEVICES);
	auto disconnectButton = this->getChildByTag<ui::Button *>(BUTTON_DISCONNECT);
	auto cancelConnectButton = this->getChildByTag<ui::Button *>(BUTTON_CANCELCONNECT);

	//エラーメッセージの設定
	if (status == SceneStatus::ERR) {
		labelError->setVisible(true);
		labelError->setString("エラー発生。再実行してください。");

		if (_status == SceneStatus::CONNECTED) {
			//切断処理でエラー(Statusはそのまま)
			_status == SceneStatus::CONNECTED;
		}
		else {
			//検索処理、接続処理でエラー(WAITに戻る。)
			_status = SceneStatus::WAIT;
		}
	}
	else {
		labelError->setVisible(false);
		labelError->setString("");
		_status = status;
	}


	//画面項目の設定
	if (_status == SceneStatus::NONE) {
		labelMessage->setVisible(true);
		labelMessage->setString("Wifiが無効です。");

		labelMyDeviceInfo->setVisible(false);
		labelMyDeviceName->setVisible(false);
		labelMyDeviceStatus->setVisible(false);
		labelGroupOwner->setVisible(false);
		labelGroupOwnerAddress->setVisible(false);
		findButton->setVisible(false);
		deviceListview->setVisible(false);
		disconnectButton->setVisible(false);
		cancelConnectButton->setVisible(false);
	}
	else if (_status == SceneStatus::WAIT) {
		labelMessage->setVisible(false);

		labelMyDeviceInfo->setVisible(true);
		labelMyDeviceName->setVisible(true);
		labelMyDeviceStatus->setVisible(true);
		labelGroupOwner->setVisible(true);
		labelGroupOwnerAddress->setVisible(true);
		findButton->setVisible(true);
		findButton->setEnabled(true);
		findButton->setTitleColor(Color3B::BLUE);

		deviceListview->setVisible(true);
		deviceListview->setEnabled(true);
		disconnectButton->setVisible(false);
		cancelConnectButton->setVisible(false);
	}
	else if (_status == SceneStatus::SEARCH) {
		labelMessage->setVisible(true);
		labelMessage->setString("検索中");

		labelMyDeviceInfo->setVisible(true);
		labelMyDeviceName->setVisible(true);
		labelMyDeviceStatus->setVisible(true);
		labelGroupOwner->setVisible(true);
		labelGroupOwnerAddress->setVisible(true);
		findButton->setVisible(true);
		findButton->setEnabled(false);
		findButton->setTitleColor(Color3B::GRAY);

		deviceListview->setVisible(false);
		disconnectButton->setVisible(false);
		cancelConnectButton->setVisible(false);
	}
	else if (_status == SceneStatus::CONNECTING) {
		labelMessage->setVisible(true);
		labelMessage->setString("接続中");

		labelMyDeviceInfo->setVisible(true);
		labelMyDeviceName->setVisible(true);
		labelMyDeviceStatus->setVisible(true);
		labelGroupOwner->setVisible(true);
		labelGroupOwnerAddress->setVisible(true);
		findButton->setVisible(true);
		findButton->setEnabled(false);
		findButton->setTitleColor(Color3B::GRAY);

		deviceListview->setVisible(true);
		deviceListview->setEnabled(false);
		disconnectButton->setVisible(false);
		cancelConnectButton->setVisible(true);
	}
	else if (_status == SceneStatus::CONNECTED) {
		labelMessage->setVisible(true);
		labelMessage->setString("接続完了");

		labelMyDeviceInfo->setVisible(true);
		labelMyDeviceName->setVisible(true);
		labelMyDeviceStatus->setVisible(true);
		labelGroupOwner->setVisible(true);
		labelGroupOwnerAddress->setVisible(true);
		findButton->setVisible(false);

		deviceListview->setVisible(true);
		deviceListview->setEnabled(false);
		disconnectButton->setVisible(true);
		cancelConnectButton->setVisible(false);
	}
}

SelectDeviceScene::SceneStatus SelectDeviceScene::calcNextStatus()
{
	SceneStatus ret = SceneStatus::NONE;

	auto isEnabled = IsWifiEnabled();
	auto isConnected = IsNWInfoEnabled();
	auto status = getMyDeviceStatus();

	if (isEnabled) {
		if (isConnected || status == "Connected") {
			ret = SceneStatus::CONNECTED;
		}
		else {
			if (_status == SceneStatus::CONNECTING) {
				ret = SceneStatus::CONNECTING;
			}
			else {
				ret = SceneStatus::WAIT;
			}
		}
	}
	else {
		ret = SceneStatus::NONE;
	}

	return ret;
}

void SelectDeviceScene::dispThisDeviceInfo() {
	auto labelMyDeviceName = this->getChildByTag<Label *>(LABEL_MY_DEVICE_NAME);
	labelMyDeviceName->setString(getMyDeviceName());

	auto labelMyDeviceStatus = this->getChildByTag<Label *>(LABEL_MY_DEVICE_STATUS);
	labelMyDeviceStatus->setString(getMyDeviceStatus());
}

void SelectDeviceScene::dispNetworkInfo()
{
	auto labelGroupOwner = this->getChildByTag<Label *>(LABEL_GROUP_OWNER);
	auto labelGroupOwnerAddress = this->getChildByTag<Label *>(LABEL_GROUP_OWNER_ADDRESS);

	if (IsNWInfoEnabled()) {
		if (IsNWInfoGroupOwner()) {
			labelGroupOwner->setString("OWNER is Mine");
		}
		else {
			labelGroupOwner->setString("OWNER is Other");
		}

		labelGroupOwnerAddress->setString("OWNER IP:" + getNWInfoGroupOwnerAddress());
	}
	else {
		labelGroupOwner->setString("");
		labelGroupOwnerAddress->setString("");
	}
}

void SelectDeviceScene::clearDeviceList() {
	for (auto wkData : _deviceList)
	{
		delete wkData;
		wkData = nullptr;
	}

	_deviceList.clear();
}

void SelectDeviceScene::dispDeviceListView() {
	auto listview = this->getChildByTag<ui::ListView *>(LISTVIEW_DEVICES);
	listview->removeAllChildrenWithCleanup(true);

	if (_deviceList.size() <= 0) {
		auto devName1 = cocos2d::ui::Text::create();
		devName1->setString("検索結果なし");
		devName1->setFontName("HiraKakuProN-W3");
		devName1->setFontSize(24);
		devName1->setColor(cocos2d::Color3B::WHITE);
		devName1->setTouchEnabled(false);
		listview->pushBackCustomItem(devName1);
	}
	else {
		for (int i = 0; i < _deviceList.size(); i++) {
			auto devName1 = cocos2d::ui::Text::create();
			devName1->setString(_deviceList[i]->name);
			devName1->setFontName("HiraKakuProN-W3");
			devName1->setFontSize(24);
			devName1->setColor(cocos2d::Color3B::WHITE);
			devName1->setTouchEnabled(true);
			devName1->setTag(i);
			listview->pushBackCustomItem(devName1);
		}
	}
}

bool SelectDeviceScene::parsePeerData(const std::string data) {
	rapidjson::Document document;
	document.Parse(data.c_str());

	if (document.HasParseError()) {
		//異常時は何もしない。
		return false;
	}

	//データ取得
	clearDeviceList();

	if (document.IsObject()) {
		//JSON配列取得

		if (document.HasMember("root") && document["root"].IsArray()) {
			const rapidjson::Value& arr = document["root"];

			std::string peerName = "";
			std::string peerAddress = "";

			// 要素取得
			for (int i = 0, max = arr.Size(); i < max; i++) {
				if (arr[i].HasMember("name")) {
					peerName = arr[i]["name"].GetString();
				}
				if (arr[i].HasMember("address")) {
					peerAddress = arr[i]["address"].GetString();
				}
				if (peerName != "" && peerAddress != "") {
					PeerData * wkData = new PeerData();
					wkData->name = peerName;
					wkData->address = peerAddress;
					_deviceList.push_back(wkData);
				}

				peerName = "";
				peerAddress = "";
			}
		}
	}

	return true;
}

void SelectDeviceScene::peersChangedAction()
{
	auto peers = GetPeersList();

	if (parsePeerData(peers)) {
		//Listviewの設定
		dispDeviceListView();
		setSceneStatus(calcNextStatus());
	}
	else {
		//エラー発生時は無視
	}
}

void SelectDeviceScene::backButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		auto helloWorldScene = HelloWorld::create();	//先に次のSceneを作成しておく。(onReceiveを動かしておく。)

		DisconnectFromWifiDirect();
		Director::getInstance()->replaceScene(helloWorldScene);
	}
}

void SelectDeviceScene::findButtonTouchEventListener(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type) {
	if (type == ui::Widget::TouchEventType::ENDED) {
		DiscoverPeers();
		setSceneStatus(SceneStatus::SEARCH);
		this->scheduleOnce(schedule_selector(SelectDeviceScene::findButtonTimeout), 5.0f);	//応答がなくても5秒したらStatus解除
	}
}

void SelectDeviceScene::findButtonTimeout(float dt)
{
	setSceneStatus(calcNextStatus());
}

void SelectDeviceScene::disconnectButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		DisconnectFromWifiDirect();
		setSceneStatus(SceneStatus::WAIT);
	}
}

void SelectDeviceScene::cancelConnectButtonTouchEventListener(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED) {
		DisconnectFromWifiDirect();
		setSceneStatus(SceneStatus::WAIT);
	}
}

void SelectDeviceScene::listviewCallback(cocos2d::Ref * sender, cocos2d::ui::ListView::EventType type)
{
	if (type == ui::ListView::EventType::ON_SELECTED_ITEM_END) {
		auto listview = static_cast<cocos2d::ui::ListView*>(sender);
		for (auto &item : listview->getItems()) {
			static_cast<cocos2d::ui::Text*>(item)->setColor(cocos2d::Color3B::WHITE);
		}

		auto currentSeletedItemIndex = (int)listview->getCurSelectedIndex();
		auto item = static_cast<cocos2d::ui::Text*>(listview->getItem(currentSeletedItemIndex));
		
		item->setColor(cocos2d::Color3B::RED);

		auto tag = item->getTag();
		auto device = _deviceList.at(tag);
		ConnectToWifiDirect(device->address);

		setSceneStatus(SceneStatus::CONNECTING);
	}
}
