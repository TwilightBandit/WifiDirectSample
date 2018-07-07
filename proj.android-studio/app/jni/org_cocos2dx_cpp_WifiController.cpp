//
// Created by twilight_bandit on 2018/05/24.
//
#ifndef PROJ_ANDROID_STUDIO_CPP_WIFICONTROLLER_H
#define PROJ_ANDROID_STUDIO_CPP_WIFICONTROLLER_H

#include <jni.h>
#include "WifiReceiveController.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_WifiController_callCocosReceiver(JNIEnv* env, jobject obj, jint action, jint reason) {
    WifiReceiveController::onReceive(action, reason);
}

#ifdef __cplusplus
}
#endif

#endif  //PROJ_ANDROID_STUDIO_CPP_WIFICONTROLLER_H
