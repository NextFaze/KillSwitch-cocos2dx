#KillSwitch-cocos2dx

Remote configuration and control for iOS and Android apps built with Cocos2D-X.

## How to include into project

First clone/download/add as a submodule to your current project.

### iOS

* Create new Group called 'KillSwitch'
* Add the following:
	* Resources folder
	* KillSwitch.cpp
	* KillSwitch.h
	* KillSwitch-ios.mm
	* ModalLayer.cpp
	* ModalLayer.h
* Under the project's Build Phases, ensure that the KillSwitch resources have been added to the 'Copy Bundle Resources' section
	* MessageButton_pressed.png
	* GeosansLight.ttf
	* MessageButton_disabled.png
	* MessageButton.png
	* MessageBox.png 

### Android
In your main AppActivity.java file
    
	import org.cocos2dx.lib.Cocos2dxActivity;
	import android.os.Bundle;
	
    // Import the KillSwitch class
	import com.nextfaze.KillSwitch;
    
    ...
    
    // In onCreate set the context
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        KillSwitch.mContext = getApplicationContext();
        
        ...
    }

Change the APP_STL define from gnustl_static in Application.mk

    APP_STL := c++_static

	APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
    
Add the required C++ files to Android.mk

	LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   
                   ...
                   
                   ../../KillSwitch/KillSwitch.cpp \
                   ../../KillSwitch/KillSwitch-android.cpp \
                   ../../KillSwitch/ModalLayer.cpp

	LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
	
						...
	
						$(LOCAL_PATH)/../../KillSwitch 

Add the project reference to the KillSwitch JAR in project.properties

    android.library.reference.1=../cocos2d/cocos/platform/android/java
    android.library.reference.2=../KillSwitch/proj.android

Add the JAR to build.xml

    <target name="-pre-compile">
    	<!-- Redefine project.all.jars.path before -compile executes -->
     	<path id="project.all.jars.path">
            <fileset file="../cocos2d/cocos/platform/android/java/bin/classes.jar" />
			<fileset file="../KillSwitch/proj.android/bin/classes.jar" />
      	</path>   
	</target>

Add the KillSwitch resources folder to the projects build-cfg.json file

	{
		"ndk_module_path" :[
			"../cocos2d",
			"../cocos2d/cocos",
			"../cocos2d/external"
		],
		"copy_resources": [
			{
				"from": "../Resources",
				"to": ""
			},
			{
				"from": "../KillSwitch/Resources",
				"to": ""
			}
		]
	}

## Usage

### Sample JSON Config file

	{
	"maintenanceMode": false,
	"maintenanceMessage": "App is currently in maintenance, please come back later",
	"appUpdateLink": "https://example.com/getApp",
	"appVersionCurrent": "1",
	"appVersionMin": "1"
	}
