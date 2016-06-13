package com.oo58.game.texaspoker.sina;

import java.text.SimpleDateFormat;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.Toast;

import com.oo58.game.texaspoker.AppActivity;
import com.oo58.game.texaspoker.R;
import com.sina.weibo.sdk.auth.AuthInfo;
import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.auth.sso.SsoHandler;
import com.sina.weibo.sdk.exception.WeiboException;
import com.sina.weibo.sdk.net.RequestListener;
import com.sina.weibo.sdk.utils.LogUtil;

public class SinaLoginLogout {

	/** 登陆认证对应的listener */
	private AuthListener mLoginListener = new AuthListener();
	/** 登出操作对应的listener */
	private LogOutRequestListener mLogoutListener = new LogOutRequestListener();
	private AuthInfo mAuthInfo;
	private Activity mContentx;
	public SsoHandler mSsoHandler;

	public SinaLoginLogout(Activity act) {
		init(act);
	}

	public void init(Activity act) {
		isLogin = false;
		mContentx = act;
		mAuthInfo = new AuthInfo(act, Constants.APP_KEY,Constants.REDIRECT_URL, Constants.SCOPE);
		if (null == mSsoHandler && mAuthInfo != null) {
			mSsoHandler = new SsoHandler((Activity) act, mAuthInfo);
		}

	}

	public boolean isLogin;

	public void doLogin() {
		isLogin = false;// sina的失败监听 接口不足，无法监听到所有类型失败 无法加判断
		if (!isLogin && mSsoHandler != null) {
			isLogin = true;
			mSsoHandler.authorize(mLoginListener);
		} else {
			// LogUtil.e(TAG, "Please setWeiboAuthInfo(...) for first");
		}
	}

	public void doLogout() {
		new LogoutAPI(mContentx, Constants.APP_KEY,AccessTokenKeeper.readAccessToken(mContentx)).logout(mLogoutListener);
	}

	/**
	 * 登入按钮的监听器，接收授权结果。
	 */
	private class AuthListener implements WeiboAuthListener {
		@Override
		public void onComplete(Bundle values) {
			isLogin = false;
			Oauth2AccessToken accessToken = Oauth2AccessToken
					.parseAccessToken(values);
			if (accessToken != null && accessToken.isSessionValid()) {
				String date = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss")
						.format(new java.util.Date(accessToken.getExpiresTime()));
				String format = mContentx
						.getString(R.string.weibosdk_demo_token_to_string_format_1);
				// mTokenView.setText(String.format(format,
				// accessToken.getToken(), date));

				AccessTokenKeeper.writeAccessToken(mContentx, accessToken);
			}
		}

		@Override
		public void onWeiboException(WeiboException e) {
			isLogin = false;
			Toast.makeText(mContentx, e.getMessage(), Toast.LENGTH_SHORT)
					.show();
		}

		@Override
		public void onCancel() {
			isLogin = false;
			Toast.makeText(mContentx,
					R.string.weibosdk_demo_toast_auth_canceled,
					Toast.LENGTH_SHORT).show();
		}
	}

	/**
	 * 登出按钮的监听器，接收登出处理结果。（API 请求结果的监听器）
	 */
	private class LogOutRequestListener implements RequestListener {
		@Override
		public void onComplete(String response) {
			if (!TextUtils.isEmpty(response)) {
				try {
					JSONObject obj = new JSONObject(response);
					String value = obj.getString("result");
					isLogin = false;
					if ("true".equalsIgnoreCase(value)) {
						AccessTokenKeeper.clear(mContentx);
						// mTokenView.setText(R.string.weibosdk_demo_logout_success);

						Toast.makeText(mContentx,
								R.string.weibosdk_demo_logout_success,
								Toast.LENGTH_SHORT).show();

						AppActivity.callReturnToLogin();
					}
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
		}

		@Override
		public void onWeiboException(WeiboException e) {
			// mTokenView.setText(R.string.weibosdk_demo_logout_failed);
			Toast.makeText(mContentx, R.string.weibosdk_demo_logout_failed,
					Toast.LENGTH_SHORT).show();
		}
	}

	public static native void setSinaLoginResult(String openID, String token);

}
