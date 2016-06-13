package com.oo58.game.texaspoker;




import com.oo58.game.texaspoker.wxapi.WXPaying;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class WEIXINLogin extends Activity {
	private IWXAPI mWeixinAPI;
	// 注册到微信
	//private static final String APP_ID = "wxf2774f075b1c646c";
//	public static final String APP_ID = "wx0688737e161b7703";
//	public static final String SECRET_ID = "f03993dde9233dfdb159789f04bb612e";
//	private static final String APP_ID = "wxc05ce32e96608fc4";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		Log.i("lvjian","hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
		goweixin();
	}
	
	/**
	 * 微信登录
	 */
	private void goweixin() {
		if (mWeixinAPI == null) {
			mWeixinAPI = WXAPIFactory.createWXAPI(this, WXPaying.APP_ID, false);
		}
		
		if (!mWeixinAPI.isWXAppInstalled()) {
			// 提醒用户没有按照微信
			Toast.makeText(this, "没有安装微信客户端", 1000).show();
			return;
		}
		mWeixinAPI.registerApp(WXPaying.APP_ID);
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.transaction = WXPaying.APP_ID;
		req.state = "none";
		mWeixinAPI.sendReq(req);
		finish();
	}
}
