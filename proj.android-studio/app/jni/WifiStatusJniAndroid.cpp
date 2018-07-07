//
// Created by twilight_bandit on 2018/05/26.
//
//#include <jni.h>
#include "jni/JniHelper.h"
#include "WifiStatusJni.h"

#define  CLASS_NAME "org/cocos2dx/cpp/WifiController"

#ifdef __cplusplus
extern "C" {
#endif

bool IsWifiEnabled() {
    bool ret = false;

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "IsWifiEnabled", "()Z")) {
        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        ret = (result != JNI_FALSE);

        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

bool IsNWInfoEnabled() {
    bool ret = false;

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "IsNWInfoEnabled", "()Z")) {
        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        ret = (result != JNI_FALSE);

        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

bool IsNWInfoGroupOwner() {
    bool ret = false;

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "IsNWInfoGroupOwner", "()Z")) {
        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
        ret = (result != JNI_FALSE);

        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

std::string getNWInfoGroupOwnerAddress() {
    std::string ret = "";

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getNWInfoGroupOwnerAddress", "()Ljava/lang/String;")) {
        jobject objResult = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        ret = cocos2d::JniHelper::jstring2string((jstring)objResult);   // jstringをstd::stringに変換

        t.env->DeleteLocalRef(objResult);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

std::string getMyDeviceName() {
    std::string ret = "";

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getMyDeviceName", "()Ljava/lang/String;")) {
        jobject objResult = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        ret = cocos2d::JniHelper::jstring2string((jstring)objResult);   // jstringをstd::stringに変換

        t.env->DeleteLocalRef(objResult);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

std::string getMyDeviceStatus() {
    std::string ret = "";

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getMyDeviceStatus", "()Ljava/lang/String;")) {
        jobject objResult = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        ret = cocos2d::JniHelper::jstring2string((jstring)objResult);   // jstringをstd::stringに変換

        t.env->DeleteLocalRef(objResult);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

void DiscoverPeers() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "DiscoverPeers", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);

        t.env->DeleteLocalRef(t.classID);
    }
}

std::string GetPeersList() {
    std::string ret = "";

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "GetPeersList", "()Ljava/lang/String;")) {
        jobject objResult = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        ret = cocos2d::JniHelper::jstring2string((jstring)objResult);   // jstringをstd::stringに変換

        t.env->DeleteLocalRef(objResult);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

void ConnectToWifiDirect(std::string address) {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "Connect", "(Ljava/lang/String;)V")) {
        jstring adr = t.env->NewStringUTF(address.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, adr);

        t.env->DeleteLocalRef(adr);
        t.env->DeleteLocalRef(t.classID);
    }
}

void DisconnectFromWifiDirect() {
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "Disconnect", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);

        t.env->DeleteLocalRef(t.classID);
    }
}

#ifdef __cplusplus
}
#endif
