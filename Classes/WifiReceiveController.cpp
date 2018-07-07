//
// Created by twilight_bandit on 2018/05/24.
//

#include "cocos2d.h"
#include "WifiReceiveController.h"

USING_NS_CC;

IWifiReceiver * WifiReceiveController::receiver_ = nullptr;

WifiReceiveController::WifiReceiveController() {

}

WifiReceiveController::~WifiReceiveController() {

}

void WifiReceiveController::onReceive(int action, int reason) {
    if (receiver_ != nullptr) {
		//cocos2d-xのメインスレッドに戻す。
		//(エラーになるケースがあったので、ここで必ず実行することにした。)
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->performFunctionInCocosThread([=]() {
			receiver_->onReceive(action, reason);
		});
    }
}

void WifiReceiveController::setReceiver(IWifiReceiver * receiver) {
    receiver_ = receiver;
}
