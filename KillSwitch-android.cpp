//
//  KillSwitch-android.cpp
//  XmasCracker
//
//  Created by Dan on 3/12/2014.
//
//

#include "KillSwitch.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

int KillSwitch::getBuildNumber()
{
    int buildNumber = 0;
    
    JniMethodInfo t;
 
	if (JniHelper::getStaticMethodInfo(t,"com/nextfaze/KillSwitch","getBuildNumber","()I")) 
	{
		buildNumber = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
    
    return buildNumber;
}

std::string KillSwitch::getVersion()
{
    std::string version = "0";
    
    JniMethodInfo t;
 
	if (JniHelper::getStaticMethodInfo(t,"com/nextfaze/KillSwitch","getVersion","()Ljava/lang/String;")) 
	{
		jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		
		
        version = JniHelper::jstring2string(s);	
	}
    
    return version;
}

std::string KillSwitch::getAppName()
{
    std::string appName = "App";
    
    JniMethodInfo t;
 
	if (JniHelper::getStaticMethodInfo(t,"com/nextfaze/KillSwitch","getAppName","()Ljava/lang/String;")) 
	{
		jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		
		
        appName = JniHelper::jstring2string(s);	
	}
    
    return appName;
}

std::string KillSwitch::getAppBundleId()
{
    std::string bundleId = "";
    
    JniMethodInfo t;
 
	if (JniHelper::getStaticMethodInfo(t,"com/nextfaze/KillSwitch","getAppBundleId","()Ljava/lang/String;")) 
	{
		jstring s = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		
		
		bundleId = JniHelper::jstring2string(s);	
	}
    
    return bundleId;
}