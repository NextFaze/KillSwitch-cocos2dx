//
//  KillSwitch.m
//  XmasCracker
//
//  Created by Dan on 3/12/2014.
//
//

#import <Foundation/Foundation.h>

#include "../KillSwitch/KillSwitch.h"

int KillSwitch::getBuildNumber()
{
    int buildNumber = 0;
    
    NSString * nsBuildNumber = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleVersionKey];
    
    if(nsBuildNumber != nil)
    {
        buildNumber = [nsBuildNumber intValue];
    }
    
    return buildNumber;
}

std::string KillSwitch::getVersion()
{
    std::string version = "0";
    
    NSString * nsVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    
    if(nsVersion != nil)
    {
        version = [nsVersion UTF8String];
    }
    
    return version;
}

std::string KillSwitch::getAppName()
{
    std::string appName = "App";
    
    NSString * nsAppName = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleNameKey];
    
    if(nsAppName != nil)
    {
        appName = [nsAppName UTF8String];
    }
    
    return appName;
}

std::string KillSwitch::getAppBundleId()
{
    std::string bundleId = "";
    
    NSString * nsBundleId = [[NSBundle mainBundle] bundleIdentifier];
    
    if(nsBundleId != nil)
    {
        bundleId = [nsBundleId UTF8String];
    }
    
    return bundleId;
}