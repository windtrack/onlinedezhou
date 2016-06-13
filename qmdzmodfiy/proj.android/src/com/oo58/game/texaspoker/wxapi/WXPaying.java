package com.oo58.game.texaspoker.wxapi;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import net.sourceforge.simcpux.MD5;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import com.oo58.game.texaspoker.R;
import com.oo58.game.texaspoker.ThreadPoolWrap;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class WXPaying {
	private static final int RQF_PAY = 1;
	private static final int RQF_LOGIN = 2;

	private String wxorder = "";
	private String wxprice = "";

	
	public static final String APP_ID = "wx0688737e161b7703";
	public static final String PARTNER_ID = "1306710701";
	public static final String API_KEY = "ASBN1WHAT1FUCK891022HOW0ARE1YOU1";
	public static final String SECRET_ID = "f03993dde9233dfdb159789f04bb612e";
	
	
	PayReq req;
	IWXAPI msgApi = null;
	private Context mCtx = null;
	public void init(Context ctx)
	{
		mCtx = ctx;

	}

	public void order(String orderId)
	{
		wxorder = orderId;
		msgApi = WXAPIFactory.createWXAPI(mCtx, null);
		msgApi.registerApp(APP_ID);
		req = new PayReq();
		
		//IntentFilter filter = new IntentFilter(WXPayEntryActivity.action);
		//registerReceiver(broadcastReceiver, filter);
		sendPayReq();
	}

	private void sendPayReq() {
		
		req.appId = APP_ID;
		req.partnerId = PARTNER_ID;
		req.prepayId = wxorder;
		req.packageValue = "Sign=WXPay";
		req.nonceStr = genNonceStr();
		req.timeStamp = String.valueOf(genTimeStamp());
		List<NameValuePair> signParams = new LinkedList<NameValuePair>();
		signParams.add(new BasicNameValuePair("appid", req.appId));
		signParams.add(new BasicNameValuePair("noncestr", req.nonceStr));
		signParams.add(new BasicNameValuePair("package", req.packageValue));
		signParams.add(new BasicNameValuePair("partnerid", req.partnerId));
		signParams.add(new BasicNameValuePair("prepayid", req.prepayId));
		signParams.add(new BasicNameValuePair("timestamp", req.timeStamp));
		req.sign = genAppSign(signParams);
		Log.i("lvjian", "------------>" + genAppSign(signParams));
		// 在支付之前，如果应用没有注册到微信，应该先调用IWXMsg.registerApp将应用注册到微信
		msgApi.sendReq(req);
	}

	private String genAppSign(List<NameValuePair> params) {
		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < params.size(); i++) {
			sb.append(params.get(i).getName());
			sb.append('=');
			sb.append(params.get(i).getValue());
			sb.append('&');
		}
		sb.append("key=");
		sb.append(API_KEY);

		String appSign = MD5.getMessageDigest(sb.toString().getBytes())
				.toUpperCase();
		Log.e("orion", appSign);
		return appSign;
	}

	private String genNonceStr() {
		Random random = new Random();
		return MD5.getMessageDigest(String.valueOf(random.nextInt(10000))
				.getBytes());
	}

	private long genTimeStamp() {
		return System.currentTimeMillis() / 1000;
	}
}
