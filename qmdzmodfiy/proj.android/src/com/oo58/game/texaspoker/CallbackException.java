package com.oo58.game.texaspoker;

import android.app.AlarmManager;
import android.app.Application;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

public class CallbackException  extends Application implements Thread.UncaughtExceptionHandler{

	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		Thread.setDefaultUncaughtExceptionHandler(this);
	}
	
	@Override
	public void uncaughtException(Thread arg0, Throwable arg1) {
		// TODO Auto-generated method stub
		Intent intent = getBaseContext().getPackageManager()  
	            .getLaunchIntentForPackage(getBaseContext().getPackageName());  
	    		

        
        PendingIntent restartIntent = PendingIntent.getActivity(    
                getApplicationContext(), 0, intent,    
                Intent.FLAG_ACTIVITY_NEW_TASK);                                                 
        //退出程序                                          
        AlarmManager mgr = (AlarmManager)getSystemService(Context.ALARM_SERVICE);    
        mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 1000,    
                restartIntent); // 1秒钟后重启应用   
        
        System.exit(0);
	}
	
}
