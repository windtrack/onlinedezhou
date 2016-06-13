package com.oo58.game.texaspoker;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;


public class NetCheckReceiver {
	
	Context mContext = null;
	private ConnectivityManager mConnectivityManager; 
	   
	 private NetworkInfo netInfo; 
	private BroadcastReceiver myNetReceiver = new BroadcastReceiver() { 
		   
		 @Override 
		 public void onReceive(Context context, Intent intent) { 
		     
		  String action = intent.getAction(); 
		        if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) { 
		              
		            mConnectivityManager = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE); 
		            netInfo = mConnectivityManager.getActiveNetworkInfo();   
		            int status = 1;
		            if(netInfo != null && netInfo.isAvailable()) { 
		   
		                 /////////////ÍøÂçÁ¬½Ó 
		                String name = netInfo.getTypeName(); 
		                   
		                if(netInfo.getType()==ConnectivityManager.TYPE_WIFI){ 
		                 /////WiFiÍøÂç 
		   
		                }else if(netInfo.getType()==ConnectivityManager.TYPE_ETHERNET){ 
		                /////ÓÐÏßÍøÂç 
		   
		                }else if(netInfo.getType()==ConnectivityManager.TYPE_MOBILE){ 
		               /////////3gÍøÂç 
		   
		                } 
		                
		                status = 1;
		              } else { 
		            	  ////////ÍøÂç¶Ï¿ª 
		            	  status = 2;
		            } 
		            
		            AppActivity.nativeNetworkChanged(status);
		        } 
		   
		   }  
	};  
	
	public void initAndRegListener(Context ctx)
	{
		mContext = ctx;
		
		IntentFilter mFilter = new IntentFilter(); 
	    mFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION); 
	    mContext.registerReceiver(myNetReceiver, mFilter); 
	}
	
	public void unregListener()
	{
		if(myNetReceiver!=null){ 
			mContext.unregisterReceiver(myNetReceiver); 
        }
	}
}
