//
// Created by twilight_bandit on 2018/05/24.
//

#ifndef PROJ_ANDROID_STUDIO_WIFIRECEIVECONTROLLER_H
#define PROJ_ANDROID_STUDIO_WIFIRECEIVECONTROLLER_H

#include "IWifiReceiver.h"

class WifiReceiveController {
//コンストラクタ
public:
    WifiReceiveController();
    ~WifiReceiveController();

//操作
public:
    static void setReceiver(IWifiReceiver * receiver);
    static void onReceive(int action, int reason);

//変数
private:
    static IWifiReceiver * receiver_;
};


#endif //PROJ_ANDROID_STUDIO_WIFIRECEIVECONTROLLER_H
