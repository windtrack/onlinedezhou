package com.oo58.game.texaspoker.wxapi;

import com.oo58.game.texaspoker.UniPaying;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler {

	private IWXAPI api;
	public static final String action = "wxaction";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// setContentView(R.layout.pay_result);

		api = WXAPIFactory.createWXAPI(this, WXPaying.APP_ID);
		api.handleIntent(getIntent(), this);
	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
		api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
	}

	@Override
	public void onResp(BaseResp resp) {
		

		
		
		
//		 Log.i("lvjian", "onPayFinish, errCode = " + resp.errCode);
//		// 支付结果
//		 
// 
//		 
		Intent intent = new Intent(action);
		if (resp.errCode == 0) {
			// 成功
			//intent.putExtra("type", "0");
			//sendBroadcast(intent);
		
			UniPaying.setMobilePayResult(1);
			finish();
		} else if (resp.errCode == -1) {
			// 失败
			//intent.putExtra("type", "-1");
			//sendBroadcast(intent);
			
			UniPaying.setMobilePayResult(2);
			finish();
		} else if (resp.errCode == -2) {
			//intent.putExtra("type", "-2");
			//sendBroadcast(intent);
			
			UniPaying.setMobilePayResult(3);
			finish();
		}
	}
}