/****************************************************************************
Copyright (c) 2014 NextFaze Pty Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.nextfaze;

import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.WindowManager;

import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;

public class KillSwitch {
	public static Context mContext = null;
	
	public static int getBuildNumber() {
		int build = 0;
		
		try {
			PackageInfo pInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0);

			build = pInfo.versionCode;		
		} catch (Exception e) {
			Log.e("KILL_SWITCH", "KillSwitch::getBuildVersion Exception:", e);
			e.printStackTrace();
		}
		
		return build;
	}
	
	public static String getBuildVersion() {
		String version = "";
		
		try {
			PackageInfo pInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0);

			version = pInfo.versionName;		
		} catch (Exception e) {
			Log.e("KILL_SWITCH", "KillSwitch::getBuildVersion Exception:", e);
			e.printStackTrace();
		}
		
		return version;
	}
	
	public static String getAppName() {
		String appName = "";
		
		try {
			PackageManager lPackageManager = mContext.getPackageManager();
			ApplicationInfo lApplicationInfo = null;
			lApplicationInfo = lPackageManager.getApplicationInfo(mContext.getApplicationInfo().packageName, 0);

			appName = (String) (lApplicationInfo != null ? lPackageManager.getApplicationLabel(lApplicationInfo) : "Unknown");		
		} catch (Exception e) {
			Log.e("KILL_SWITCH", "KillSwitch::getBuildVersion Exception:", e);
			e.printStackTrace();
		}
		
		return appName;
	}
}
