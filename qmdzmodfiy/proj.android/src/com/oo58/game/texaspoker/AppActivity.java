/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

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
package com.oo58.game.texaspoker;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.alexd.jsonrpc.JSONRPCException;
import org.apache.http.impl.client.DefaultHttpClient;
import org.cocos2dx.lib.*;
import org.json.JSONException;
import org.json.JSONObject;


import com.oo58.game.UpdateUtils;
import com.oo58.game.texaspoker.sina.SinaLoginLogout;
import com.oo58.game.texaspoker.wxapi.WXPayEntryActivity;
import com.oo58.game.texaspoker.wxapi.WXPaying;
import com.tencent.android.tpush.XGIOperateCallback;
import com.tencent.android.tpush.XGPushClickedResult;
import com.tencent.android.tpush.XGPushConfig;
import com.tencent.android.tpush.XGPushManager;
import com.tencent.android.tpush.common.Constants;
import com.tencent.android.tpush.service.XGPushService;
import com.tencent.stat.StatConfig;
import com.tencent.stat.StatService;
import com.tencent.tauth.Tencent;
import com.umeng.analytics.AnalyticsConfig;
import com.umeng.analytics.MobclickAgent;
import com.umeng.mobclickcpp.MobClickCppHelper;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;
import android.content.pm.Signature;
import android.media.MediaRecorder;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.os.Vibrator;
import android.util.JsonReader;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {
	public static Context mContext;
	public static Context allContext;
	public static Tencent mTencent;
	private PowerManager.WakeLock mWakeLock;
	// http://discuss.cocos2d-x.org/t/how-to-set-full-screen-on-android-4-4/10278/4
	private Cocos2dxGLSurfaceView glSurfaceView;
	
	public static Activity mAct = null;
	
	NetCheckReceiver mNetChecker = new NetCheckReceiver();
	
	WXPaying mWXPay = new WXPaying();
	
	//SoundRecorder m_soundRecorder ;

	
	
	private MsgReceiver updateListViewReceiver;
	
	
	
	
	
	public static final int FLAG_HOMEKEY_DISPATCHED = 0x80000000;
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		   this.getWindow().setFlags(FLAG_HOMEKEY_DISPATCHED, FLAG_HOMEKEY_DISPATCHED);//关键代码
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		mContext = AppActivity.this;
		
		try {
			ApplicationInfo appInfo = this.getPackageManager()
                    .getApplicationInfo(getPackageName(),
            PackageManager.GET_META_DATA);
				String msg=appInfo.metaData.getString("data_Name");
			
//			int channelid = Integer.parseInt(msg) ;
//			
//			System.out.println(channelid);
			
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
			
	
		
		
		
		
		
		mAct = this;
		allContext = this.getApplicationContext();
		mTencent = Tencent.createInstance("1104823392", getApplicationContext());
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		mWakeLock = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK,
				"MyLock");
		mWakeLock.acquire();
		
		MobclickAgent.setDebugMode(false);
		MobclickAgent.updateOnlineConfig(this);
		AnalyticsConfig.enableEncrypt(true);
		
		checkUpdate();
		
		PackageManager pm2 = getPackageManager();  
		homeInfo= pm2.resolveActivity(new Intent(Intent.ACTION_MAIN)  .addCategory(Intent.CATEGORY_HOME), 0);  
		
		mNetChecker.initAndRegListener(mContext);
		
		mWXPay.init(this);
		
		MobClickCppHelper.init(this);
		
		
		
		
		
		
		
		
		
		
//		// 开启logcat输出，方便debug，发布时请关闭
//		 XGPushConfig.enableDebug(this, true);
//		// 如果需要知道注册是否成功，请使用registerPush(getApplicationContext(), XGIOperateCallback)带callback版本
//		// 如果需要绑定账号，请使用registerPush(getApplicationContext(),account)版本
//		// 具体可参考详细的开发指南
//		// 传递的参数为ApplicationContext
//		Context context = getApplicationContext();
//		XGPushManager.registerPush(context);    
//		 
//		// 2.36（不包括）之前的版本需要调用以下2行代码
//		Intent service = new Intent(context, XGPushService.class);
//		context.startService(service);
		 
		 
		// 其它常用的API：
		// 绑定账号（别名）注册：registerPush(context,account)或registerPush(context,account, XGIOperateCallback)，其中account为APP账号，可以为任意字符串（qq、openid或任意第三方），业务方一定要注意终端与后台保持一致。
		// 取消绑定账号（别名）：registerPush(context,"*")，即account="*"为取消绑定，解绑后，该针对该账号的推送将失效
		// 反注册（不再接收消息）：unregisterPush(context)
		// 设置标签：setTag(context, tagName)
		// 删除标签：deleteTag(context, tagName)
		
		updateListViewReceiver = new MsgReceiver();
		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction("com.oo58.game.texaspoker.activity.UPDATE_LISTVIEW");
		registerReceiver(updateListViewReceiver, intentFilter);
		
		XGPushManager.registerPush(getApplicationContext(),
				new XGIOperateCallback() {
					@Override
					public void onSuccess(Object data, int flag) {
//						Log.w(Constants.LogTag,
//								"+++ register push sucess. token:" + data);

					}

					@Override
					public void onFail(Object data, int errCode, String msg) {
//						Log.w(Constants.LogTag,
//								"+++ register push fail. token:" + data
//										+ ", errCode:" + errCode + ",msg:"
//										+ msg);

					}
				});
		
		
		
		
		
		
		
		
		
		
		
		
		//java传josn给C++的demo
/*		JSONObject jsonObj = new JSONObject();  
        try {
			jsonObj.put("Int_att",25);
	        jsonObj.put("String_att","str");//添加string型属性  
	        jsonObj.put("Double_att",12.25);//添加double型属性  
	        jsonObj.put("Boolean_att",true);//添加boolean型属性  
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}//添加int型属性  

        PushJson(jsonObj.toString()) ;*/
		
	}
	
	
	@SuppressLint("HandlerLeak") 
	private static Handler mVideoHandler = new Handler() {
		public void handleMessage(final Message msg)
		{
			mAct.runOnUiThread(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					switch (msg.what) {
					case 2009:
					{
						AppActivity appAct = (AppActivity)mAct;

//						appAct.showTemplate(YouTuiViewType.WHITE_GRID);
					}
						break ;
					case 1001:
					{
						//Toast.makeText(mAct, (String)msg.obj, Toast.LENGTH_LONG).show();
						AppActivity appAct = (AppActivity)mAct;
						//appAct.innerPlayVideo((String)msg.obj, msg.arg1, msg.arg2);
						//appAct.innerPlayVideo("rtmp://video.0058.com/RoomVideoChat/123456", 100, 100);
						appAct.innerPlayVideo((String)msg.obj, msg.arg1, msg.arg2);
					}
						break;
					case 1002:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.innerStopVideo();
					}
						break;
					case 1003:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.innerSetVideoVisible(msg.arg1 == 1 ? true : false);
					}
						break;
					case 1004:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.innerBringVideoToFront();
					}
						break;
					case 1005:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.innerVideoMute(msg.arg1);
					}
						break;
					case 1006:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.innerSetVideoButtonsVisible(msg.arg1==1 ? true : false);
					}
						break;
					case 2001:
					{
						AppActivity appAct = (AppActivity)mAct;
						appAct.mWXPay.order((String)msg.obj);
					}
						break;
					}
				}
				
			});
			super.handleMessage(msg);
		}
	};
	
	public void checkUpdate()
	{
		UpdateUtils uu = UpdateUtils.getInstance();
		uu.init(this, "6");
		// 检查版本更新
		uu.checkVersion();
	}
	static {
        MobClickCppHelper.loadLibrary();
    }
	
	ResolveInfo homeInfo ;
	@Override
	public void onPause() {
		super.onPause();
		if (mWakeLock != null) {
			mWakeLock.release();
			mWakeLock = null;
			
		}
		
		MobclickAgent.onPause(this);
		XGPushManager.onActivityStoped(this);
	}

	@Override
	public void onResume() {
		super.onResume();
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		//mWakeLock = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK,
		//		"<span><span class='string'>MyLock</span></span>");
		mWakeLock = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK| PowerManager.ON_AFTER_RELEASE, "ATAAW");
		mWakeLock.acquire();
		
		
	
		MobclickAgent.onResume(this);
		
		XGPushClickedResult click = XGPushManager.onActivityStarted(this);
		Log.d("TPush", "onResumeXGPushClickedResult:" + click);
		if (click != null) { // 判断是否来自信鸽的打开方式
//			Toast.makeText(this, "通知被点击:" + click.toString(),
//					Toast.LENGTH_SHORT).show();
		}
	}
	
	@Override
	protected void onDestroy() {
		// 释放资源
		super.onDestroy();
		if (mWakeLock != null) {
			mWakeLock.release();
			mWakeLock = null;
		}
		
		if (mNetChecker != null) {
			mNetChecker.unregListener();
		}
	}
	
	public static Object rtnActivity() {
		return mContext;
	}
	
	public Cocos2dxGLSurfaceView onCreateView() {
		glSurfaceView = new Cocos2dxGLSurfaceView(this);
		this.hideSystemUI();
		// TestCpp should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		 if (hasFocus)
		 {
		 this.hideSystemUI();
		 }
	}

	private void hideSystemUI() {
		// Set the IMMERSIVE flag.
		// Set the content to appear under the system bars so that the content
		// doesn't resize when the system bars hide and show.
		glSurfaceView
				.setSystemUiVisibility(//Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
		 Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE
		 | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
		 | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
		 | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
		 | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
		 | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	}
	
	public static void QQ_Logon() {
		Intent intent_pay = new Intent();
		intent_pay.setClass(mContext, LoginActivity.class);
		mContext.startActivity(intent_pay);
	}

	public static void WX_Logon() {
		Intent intent_pay = new Intent();
		intent_pay.setClass(mContext, WEIXINLogin.class);
		mContext.startActivity(intent_pay);
	}
	
	//sina登录
	SinaLoginLogout mSinaLoginLogout ;
	public static void sina_Logon() {
		AppActivity appAct = (AppActivity)mAct;
		if(appAct.mSinaLoginLogout==null){
			appAct.mSinaLoginLogout = new SinaLoginLogout(appAct) ;
		}
		appAct.mSinaLoginLogout.doLogin() ;
	}
	public static void callSinaLogout(){
		
		AppActivity appAct = (AppActivity)mAct;
		if(appAct.mSinaLoginLogout!=null){
			appAct.mSinaLoginLogout.doLogout() ;
		}
	
	}
	public static void Mobile_pay(String order_id, String money, String name, int type) {
		Log.i("lvjian", "order_id--------"+order_id);
		Log.i("lvjian", "money--------"+money);
		Log.i("lvjian", "name---------" + name);
		if(type==0){
			Intent intent_pay = new Intent();
			intent_pay.putExtra("order_id", order_id);
			intent_pay.putExtra("money", money);
			intent_pay.setClass(mContext, UniPaying.class);
			mContext.startActivity(intent_pay);
		} else if (type == 1) {
			Intent intent_pay = new Intent();
			intent_pay.putExtra("order_id", order_id);
			intent_pay.putExtra("money", money);
			intent_pay.putExtra("name", name);
			intent_pay.setClass(mContext, AliPaying.class);
			mContext.startActivity(intent_pay);
		} else if (type == 2) { // WeiXin Pay

			Message msg = mVideoHandler.obtainMessage();
			msg.what = 2001;
			msg.obj = order_id;
			mVideoHandler.sendMessage(msg);
		}
		
	}
	public static String getVersionName() {
		return UpdateUtils.versionName;
	}
	
	public static void Vibrate(final int times)
	{
		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					 Vibrator vibrator = (Vibrator) mContext.getSystemService(VIBRATOR_SERVICE);
			         vibrator.vibrate(times);
				} catch (Exception ex) 
				{
					ex.printStackTrace();
				}
			}
			
		});

	}
	
	public static void callCustomService(final String number)
	{
		final Activity ctx = mAct;
		ctx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					Intent intent = new Intent(Intent.ACTION_CALL,Uri.parse("tel:"+number));  
					ctx.startActivity(intent);
				}
				catch (Exception ex)
				{
					ex.printStackTrace();
				}
			}
		});
	}
	
	private static ConnectivityManager connMgr = null;
	public static boolean checkNetworkState()
	{

		if(null == connMgr){  

            connMgr = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);  
        }  
		NetworkInfo net = connMgr.getActiveNetworkInfo();  
        if (net != null && net.isConnected()) {  

            return true;  
        }  
        return false;
	}
	
	public static void logi(String tag, String text)
	{
		Log.i(tag, text);
	}
	
	public  static String getLocalMacAddress() {

		 WifiManager wifi = (WifiManager) mContext.getSystemService (Context.WIFI_SERVICE);
	     WifiInfo info = wifi.getConnectionInfo();
	     return info.getMacAddress();
	     	
	}
	
	public static void playVideo(final String path, final int x, final int y)
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1001;
		msg.arg1 = x;
		msg.arg2 = y;
		msg.obj = path;
		mVideoHandler.sendMessage(msg);		
	}
	
	public static void setVideoVisible(boolean visible)
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1003;
		msg.arg1 = visible ? 1 : 0;
		mVideoHandler.sendMessage(msg);
	}
	
	public static void stopVideo()
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1002;
		mVideoHandler.sendMessage(msg);
	}
	
	public static void bringVideoToFront()
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1004;
		mVideoHandler.sendMessage(msg);
	}
	
	public static void setVideoMute(boolean mute)
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1005;
		msg.arg1 = mute ? 0: 50;
		mVideoHandler.sendMessage(msg);
	}
	
	public static void setVideoToolsVisible(boolean visible)
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1006;
		msg.arg1 = visible ? 1: 0;
		mVideoHandler.sendMessage(msg);
	}
	
	public static void restartApp()
	{
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 1007;
		mVideoHandler.sendMessage(msg);
	}
	
	public void innerRestart()
	{
		final Intent intent = getPackageManager().getLaunchIntentForPackage(getPackageName());  
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);  
        startActivity(intent);
        
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    super.onActivityResult(requestCode, resultCode, data);
	    if(mSinaLoginLogout!=null){
	    	   if (mSinaLoginLogout.mSsoHandler != null) {
	   	    	mSinaLoginLogout.mSsoHandler.authorizeCallBack(requestCode, resultCode, data);
	   	    }
	    }
	    	
	}
	
	public static void callShow3rdLogin(){
		
		
		
	}
	public static void callShow3rdLogout(){
		
		
		
	}
	public static void call3rdExit(){
		
		
	} 
	
	
	
	public static String curBillingPrice;
	public static String curBillingName;
	public static String curBillingOrderid;
	public static String curBillingProid;
	public static String curBillingUid;
	public static String curBillingUname;
	
	public static void callBilling(String jsonstr) {

		try {
			JSONObject jb = new JSONObject(jsonstr);//转换为JSONObject  

			String uid = jb.getString("uid");
			String uname = jb.getString("uname");
			String strTradeID = jb.getString("strTradeID");
			String itemName = jb.getString("itemName");
			String itemprice = jb.getDouble("itemprice")*100+"";
			String itemid = jb.getString("itemid");
			
			curBillingOrderid = strTradeID;
			curBillingPrice = itemprice;
			curBillingName = itemName;
			curBillingProid = itemid;
			curBillingUid = uid;
			curBillingUname = uname;

		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		AppActivity appAct = (AppActivity) mAct;
//		appAct.doSdkPay(appAct.mQihooUserInfo, true);
	}
	
	
	
	
	public static final int REQUSET = 1; 
	public static String mOpenID ;
	public static String mOpenKey ;
	/**
	 * 拍照
	 */
	public static void callTakePhoto(){
		AppActivity appAct = (AppActivity)mAct;
		Intent intent = new Intent() ;
		intent.setClass(appAct, PhotoManagerActivity.class) ;
		
		intent.putExtra("fun", "takephoto") ;
		appAct.startActivityForResult(intent, REQUSET);  
	}
	/**
	 * 打开相册
	 */
	public static void callPhotoalbum(){

		AppActivity appAct = (AppActivity)mAct;
		Intent intent = new Intent() ;
		intent.setClass(appAct, PhotoManagerActivity.class) ;
		
		intent.putExtra("fun", "photoalbum") ;
		appAct.startActivityForResult(intent, REQUSET);  
		
	}
	/**
	 * 调用录音机
	 */
/*	public static void callSoundRecorder(){
		AppActivity appAct = (AppActivity)mAct;
		if(appAct.m_soundRecorder==null){
			appAct.m_soundRecorder = new SoundRecorder() ;
		}
		appAct.m_soundRecorder.start(); 
	}
	public static void callStopRecorder(){
		AppActivity appAct = (AppActivity)mAct;
		if(appAct.m_soundRecorder==null){
			appAct.m_soundRecorder = new SoundRecorder() ;
		}
		appAct.m_soundRecorder.stop();
	}*/
	
	
	public static void callShare(){
		
		Message msg = mVideoHandler.obtainMessage();
		msg.what = 2009;
		mVideoHandler.sendMessage(msg);
		
		

	}
	
	
	public static void showLog(String str){
		Log.i("cocos2dx_sjf", "sjf:log  "+str) ;
	} ;
	public class MsgReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
//			allRecorders = notificationService.getCount();
//			getNotificationswithouthint(id);
		}
	}
	
	public static native void set3rdLoginResult(String openID, String token);
	
	public static native void nativeNetworkChanged(final int status);
	
	public static native void nativeUpdateChanged();
	
	public static native void callPauseNetWork(boolean flag);
	
    public  static native void callReturnToLogin();
    
    public static native void setNeedUpdate(boolean flag) ;//是否需要更新
    
    public static native void setNewVersion(String version) ;//更新的版本
    
    public static native void setUpdateContent(String content) ;//更新的内容
    
    public static native void setUpdateLoadingPro(final int loadingPro) ;//更新进度
    
    public static native void showUpdateScene();//进入更新界面
    
    public static native void callGameFinish();

    public static native void PushJson(String str) ;
}
