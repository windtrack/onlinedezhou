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
package com.oo58.game;

import java.io.File;
import java.io.FileInputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import org.alexd.jsonrpc.JSONRPCException;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONException;
import org.json.JSONObject;

import com.oo58.game.texaspoker.AppActivity;
import com.oo58.game.texaspoker.ThreadPoolWrap;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Handler;
import android.os.Message;

public class UpdateUtils {
	public void initVersion()
	{
		try {
			PackageInfo packageInfo = ctx.getApplicationContext()
					.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
			localVersion = packageInfo.versionCode;
			versionName = packageInfo.versionName;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	public static String get_version() {
		String s = "";
		try {
//			String uri = "http://60.174.249.98:8000/api/rpc/pc.php";
			String uri = "http://www.wepoker.cn/api/rpc/pc.php";
			MyJSONRPCHttpClient client = new MyJSONRPCHttpClient(
					new DefaultHttpClient(), uri);
			client.setConnectionTimeout(5000);
			client.setSoTimeout(5000);
			JSONObject result = client.callJSONObject("m_game_ver", gameId, "android");
			s = result.toString();
		} catch (JSONRPCException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return s;

	}
	public static String mVersioncode ;
	public static String mUpdateInfo ;
	/**
	 * 检查版本，版本更新
	 */
	public void checkVersion() {
		initVersion();
		
		Runnable checkversionrun = new Runnable() {

			public void run() {
				String s = get_version();
				boolean startNormal = true;
				try {
					JSONObject obj = new JSONObject(s);
					int state = obj.getInt("s");
					if (state == 1) {
						JSONObject data = obj.getJSONObject("data");
						int ver = data.getInt("ver");
						mVersioncode = data.getString("version") ;
						mUpdateInfo = data.getString("content") ;
						
						
						if (localVersion < ver) {
							AppActivity.setNeedUpdate(true);
							AppActivity.setNewVersion(mVersioncode);
							
							
							String temp[]=mUpdateInfo.split("&");
							
							for(int i=0;i<temp.length;i++){
								ArrayList splitStr = split(temp[i], 24*3) ;
								for(int j=0; j<splitStr.size(); j++){
									String str = splitStr.get(j).toString() ;
									String sStr ;
									if(j>0){
										sStr = "      "+str ;
									}else{
										sStr = str ;
									}
								
									AppActivity.setUpdateContent(sStr);
								}
							}
							
							Message msg = mHandler.obtainMessage();
							msg.obj = data.getString("url");
							msg.arg1 = data.getInt("force");
							msg.arg2 = ver;
//							msg.what = APP_VERSION;
							msg.what = APP_UPDATE_START ;
							mHandler.sendMessage(msg);
							startNormal = false;
							
						}
					} 
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					startNormal = true;
				} finally {
					if (startNormal) {
						final Message msg = mHandler.obtainMessage();
						msg.what = APP_START;
						mHandler.postDelayed(new Runnable() {
							@Override
							public void run() {
								// TODO Auto-generated method stub
								mHandler.sendMessage(msg);
							}							
						}, 1000);
						
					}
				}
			}
		};
		ThreadPoolWrap.getThreadPool().executeTask(checkversionrun);
	}
	
	public static UpdateUtils getInstance()
	{
		if (instance == null) {
			instance = new UpdateUtils();
		}
		
		return instance;
	}
	
	public void init(Activity ctx, String gId)
	{
		this.ctx = ctx;
		gameId = gId;
	}
	
	public static UpdateManager mUpdateManager ;
	
	private Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {

			switch (msg.what) {
			case APP_VERSION:
				// 这里来检测版本是否需要更新
				{
					String url = (String)msg.obj;
					int forceUpdate = msg.arg1;
					int vercode = msg.arg2;
					mUpdateManager = new UpdateManager(ctx);
					mUpdateManager.checkUpdateInfo(url, forceUpdate, vercode);
				}

				break;
			case APP_START:
				startApp();
				break;
			case APP_UPDATE_START:
			{
				String url = (String)msg.obj;
				int forceUpdate = msg.arg1;
				int vercode = msg.arg2;
				mUpdateManager = new UpdateManager(ctx);
				mUpdateManager.checkUpdateInfo(url, forceUpdate, vercode);
				startUpdateApp();
		
			}

				break;
			}
			
		}
	};
	
	public void startApp()
	{
		//Intent it = new Intent(ctx, AppActivity.class);
		//it.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
		//ctx.startActivity(it);
		//ctx.finish();
		AppActivity.nativeUpdateChanged();
	}
	public void startUpdateApp()
	{

		AppActivity.showUpdateScene();
	}
	
	public static void doUpdate(){
		if (mUpdateManager.installApk()) {
			return;
		}
		
		mUpdateManager.downloadApk();
	}
	private static UpdateUtils instance = null;
	private Activity ctx = null;
	
	public static int localVersion = 0;// 本地安装版本
	public static String versionName = "1.0";// 本地安装版本
	public static String gameId = "";
	public static boolean forceUpdate = false;
	
	public final static int APP_VERSION = 3;
	public final static int APP_START = 4;
	public final static int APP_UPDATE_START = 5;
	
	

	
	
	
	/**
     * 按指定长度和编码拆分中英混合字符串 
     * @param text 被拆分字符串
     * @param length 指定长度，即子字符串的最大长度
     * @param encoding 字符编码
     * @return 拆分后的子字符串列表
     * @throws UnsupportedEncodingException
     */
    public static ArrayList split(String text, int length, String encoding)
            throws UnsupportedEncodingException {
        ArrayList texts = new ArrayList();
        int pos = 0;
        int startInd = 0;
        for(int i=0;text!=null&&i<text.length();) {
            byte[] b = String.valueOf(text.charAt(i)).getBytes(encoding);
            if(b.length>length) {
                i++;
                startInd = i;
                continue;
            }
            pos += b.length;
            if(pos>=length) {
                int endInd;
                if(pos==length) {
                    endInd = ++i;
                } else {
                    endInd = i;
                }
                texts.add(text.substring(startInd, endInd));
                pos = 0;
                startInd = i;
            } else {
                i++;
            }
        }
        if(startInd<text.length()) {
            texts.add(text.substring(startInd));
        }
        return texts;
    }
    /**
     * 指定长度按UTF-8编码拆分中英混合字符串，即一个非ASCII码长度为3 
     * @param text 被拆分字符串
     * @param length 指定长度，即子字符串的最大长度
     * @return
     */
    public static ArrayList split(String text, int len) {
        try {
            return split(text, len, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return null;
        }
    }
}
