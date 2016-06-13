package com.oo58.game;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import com.oo58.game.texaspoker.AppActivity;
import com.oo58.game.texaspoker.R;
import com.oo58.game.texaspoker.R.id;
import com.oo58.game.texaspoker.R.layout;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ProgressBar;

public class UpdateManager {

	private Context mContext;

	// 提示语
	private String updateMsg = "有最新的软件包哦，亲快下载吧~,如果更新遇到问题,请从官网下载最新版本,卸载旧版本后,重新安装";

	// 返回的安装包url
	private String apkUrl = "http://appsdown.wepoker.cn/android/dzshowhand.apk";

	private boolean forceUpdate = false;
	private int verCode = 0;

	private Dialog noticeDialog;

	private Dialog downloadDialog;
	/* 下载包安装路径 */
	private String savePath = "/update_path/";

	private String saveFileName = "UpdatePokerRelease";
	private String saveFileNameExt = ".apk";

	/* 进度条与通知ui刷新的handler和msg常量 */
	private ProgressBar mProgress;

	public boolean isInstalling;
	private static final int DOWN_UPDATE = 1;

	private static final int DOWN_OVER = 2;

	private static final int DOWN_EXCEPTION = 3;

	private int progress;

	private Thread downLoadThread;

	private boolean interceptFlag = false;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case DOWN_UPDATE:
				if (mProgress != null)
					mProgress.setProgress(progress);
				break;
			case DOWN_OVER:
				// downloadDialog.dismiss();
				// downloadDialog = null;
				installApk();
				break;
			case DOWN_EXCEPTION:
				if(downloadDialog!=null){
					downloadDialog.dismiss();
					downloadDialog = null;
				}
				
				if (forceUpdate) {
					System.exit(0);
				} else {
					UpdateUtils.getInstance().startApp();
				}
				break;
			default:
				break;
			}
		};
	};

	public UpdateManager(Context context) {
		this.mContext = context;
	}

	// 外部接口让主Activity调用
	public void checkUpdateInfo(String url, int force, int vercode) {
		apkUrl = url;
		verCode = vercode;
		forceUpdate = force == 1 ? true : false;
		saveFileName = saveFileName + verCode + saveFileNameExt;
		savePath = Environment.getExternalStorageDirectory() + savePath;

	}

	public void showUpdatePromptDialog() {
		noticeDialog = new AlertDialog.Builder(mContext).create();
		noticeDialog.setCancelable(false);
		noticeDialog.show();
		Window localWindow = noticeDialog.getWindow();
		localWindow.setGravity(Gravity.CENTER);
		View vUpdatePromptDlg = LayoutInflater.from(mContext).inflate(
				R.layout.loading_dialog, null);
		localWindow.setContentView(vUpdatePromptDlg);
		// 设置大小

		android.view.WindowManager.LayoutParams lp = localWindow
				.getAttributes();
		lp.width = LayoutParams.WRAP_CONTENT;
		lp.height = LayoutParams.WRAP_CONTENT;
		localWindow.setAttributes(lp);

		Button updateBtn = (Button) vUpdatePromptDlg
				.findViewById(R.id.btn_download);
		// updateBtn.setOnTouchListener(new OnTouchListener() {
		//
		// @Override
		// public boolean onTouch(View arg0, MotionEvent arg1) {
		// // TODO Auto-generated method stub
		// noticeDialog.dismiss();
		// //noticeDialog = null;
		// showUpdateProgressDialog();
		// return false;
		// }
		//
		// });
		updateBtn.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				noticeDialog.dismiss();
				noticeDialog = null;

				if (installApk()) {
					return;
				}

				showUpdateProgressDialog();
			}

		});

		Button updateLaterBtn = (Button) vUpdatePromptDlg
				.findViewById(R.id.btn_updatelater);
		if (forceUpdate) {
			updateLaterBtn.setEnabled(false);
		}
		updateLaterBtn.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				noticeDialog.dismiss();
				noticeDialog = null;
				UpdateUtils.getInstance().startApp();
			}
		});

		noticeDialog.setOnKeyListener(new OnKeyListener() {
			@Override
			public boolean onKey(DialogInterface arg0, int arg1, KeyEvent arg2) {
				// TODO Auto-generated method stub
				if (arg1 == KeyEvent.KEYCODE_BACK) {
					System.exit(0);
				}
				return false;
			}
		});
	}

	public void showUpdateProgressDialog() {
		downloadDialog = new AlertDialog.Builder(mContext).create();
		downloadDialog.setCancelable(false);
		downloadDialog.show();
		Window localWindow = downloadDialog.getWindow();
		localWindow.setGravity(Gravity.CENTER);
		View vUpdatePgDlg = LayoutInflater.from(mContext).inflate(
				R.layout.loading_dialog2, null);

		localWindow.setContentView(vUpdatePgDlg);
		// 设置大小

		android.view.WindowManager.LayoutParams lp = localWindow
				.getAttributes();
		lp.width = LayoutParams.WRAP_CONTENT;
		lp.height = LayoutParams.WRAP_CONTENT;
		localWindow.setAttributes(lp);

		Button cancelBtn = (Button) vUpdatePgDlg.findViewById(R.id.btn_cancel);

		cancelBtn.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				downloadDialog.dismiss();
				downloadDialog = null;
				interceptFlag = true;
				if (forceUpdate) {
					System.exit(0);
				} else {

					UpdateUtils.getInstance().startApp();
				}
			}
		});

		downloadDialog.setOnKeyListener(new OnKeyListener() {
			@Override
			public boolean onKey(DialogInterface arg0, int arg1, KeyEvent arg2) {
				// TODO Auto-generated method stub
				if (arg1 == KeyEvent.KEYCODE_BACK) {
					System.exit(0);
				}
				return false;
			}
		});

		isInstalling = false;
		mProgress = (ProgressBar) vUpdatePgDlg
				.findViewById(R.id.update_progress);

		Log.e("开始下载了。。。。。", "");
		downloadApk();
		
	}

	/*
	 * private void showNoticeDialog(){ AlertDialog.Builder builder = new
	 * Builder(mContext); builder.setTitle("软件版本更新");
	 * builder.setCancelable(false); builder.setMessage(updateMsg);
	 * builder.setPositiveButton("下载", new OnClickListener() {
	 * 
	 * @Override public void onClick(DialogInterface dialog, int which) {
	 * dialog.dismiss(); showDownloadDialog(); } });
	 * builder.setNegativeButton("以后再说", new OnClickListener() {
	 * 
	 * @Override public void onClick(DialogInterface dialog, int which) {
	 * dialog.dismiss(); } }); noticeDialog = builder.create();
	 * noticeDialog.show(); }
	 * 
	 * private void showDownloadDialog(){ AlertDialog.Builder builder = new
	 * Builder(mContext); builder.setTitle("软件版本更新");
	 * builder.setCancelable(false); final LayoutInflater inflater =
	 * LayoutInflater.from(mContext); View v =
	 * inflater.inflate(R.layout.progress, null); mProgress =
	 * (ProgressBar)v.findViewById(R.id.progress);
	 * 
	 * builder.setView(v); builder.setNegativeButton("取消", new OnClickListener()
	 * {
	 * 
	 * @Override public void onClick(DialogInterface dialog, int which) {
	 * dialog.dismiss(); interceptFlag = true; } });
	 * 
	 * downloadDialog = builder.create(); downloadDialog.show();
	 * Log.e("开始下载了。。。。。", ""); downloadApk(); }
	 */

	private Runnable mdownApkRunnable = new Runnable() {
		@Override
		public void run() {
			try {
				URL url = new URL(apkUrl);

				Log.e("开始下载了。。。。。下载地址", apkUrl);
				HttpURLConnection conn = (HttpURLConnection) url
						.openConnection();
				conn.connect();
				int length = conn.getContentLength();
				InputStream is = conn.getInputStream();

				// File sdcardDir=Environment.getExternalStorageDirectory();
				// savePath=sdcardDir+savePath;
				Log.e("保存地址", savePath);
				File file = new File(savePath);
				if (!file.exists()) {
					file.mkdir();
				}

				String apkFile = savePath + "temp" + saveFileName;

				Log.e("apkFile", apkFile);
				File ApkFile = new File(apkFile);
				if (ApkFile.exists())
					ApkFile.delete();
				FileOutputStream fos = new FileOutputStream(ApkFile);

				int count = 0;
				byte buf[] = new byte[4096];

				do {
					int numread = is.read(buf);
					count += numread;
					progress = (int) (((float) count / length) * 100);
					AppActivity.setUpdateLoadingPro(progress);
					Log.e("进度与剩余数据", progress + " " + numread);
					// 更新进度
					mHandler.sendEmptyMessage(DOWN_UPDATE);
					if (numread <= 0 && !isInstalling) {
						isInstalling = true;
						// 下载完成通知安装
						// mHandler.sendEmptyMessage(DOWN_OVER);
						break;
					}
					fos.write(buf, 0, numread);
				} while (!interceptFlag && !isInstalling);// 点击取消就停止下载.
				Log.e("关闭文件结束", saveFileName);
				fos.close();
				is.close();
					
				if (!interceptFlag) {
					String newName = savePath + saveFileName;
					File apkNewFile = new File(newName);
					if (apkNewFile.exists())
						apkNewFile.delete();
					ApkFile.renameTo(apkNewFile);
					// mHandler.sendEmptyMessage(DOWN_OVER);

					String installFile = savePath + saveFileName;
					Log.e("安装文件", installFile);
					File apkfile = new File(installFile);

					Intent i = new Intent(Intent.ACTION_VIEW);
					i.setDataAndType(Uri.parse("file://" + apkfile.toString()),
							"application/vnd.android.package-archive");
					mContext.startActivity(i);
					System.exit(0);
						
				}
			} catch (Exception e) {
				e.printStackTrace();
				mHandler.sendEmptyMessage(DOWN_EXCEPTION);
			}

		}
	};

	/**
	 * 下载apk
	 * 
	 * @param url
	 */

	public void downloadApk() {
		downLoadThread = new Thread(mdownApkRunnable);
		downLoadThread.start();
	}

	/**
	 * 安装apk
	 * 
	 * @param url
	 */
	public boolean installApk() {
		// String installFile =savePath+saveFileName;
		// Log.e("安装文件", installFile);
		// File apkfile = new File(installFile);
		// if (!apkfile.exists()) {
		// return false;
		// }
		// Intent i = new Intent(Intent.ACTION_VIEW);
		// i.setDataAndType(Uri.parse("file://" + apkfile.toString()),
		// "application/vnd.android.package-archive");
		// mContext.startActivity(i);
		// System.exit(0);

		String installFile = savePath + saveFileName;
		File file = new File(installFile);
		if (!file.exists()) {
			return false;
		}
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(Intent.ACTION_VIEW);
		String type = "application/vnd.android.package-archive";
		intent.setDataAndType(Uri.parse("file://" + file.toString()), type);
		mContext.startActivity(intent);
		System.exit(0);
		Log.e("开始安装", "安装中，唤起界面");

		// if(!isInstalling){
		// if (!file.exists()) {
		// return false;
		// }
		// Intent intent = new Intent();
		// intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		// intent.setAction(Intent.ACTION_VIEW);
		// String type = "application/vnd.android.package-archive";
		// intent.setDataAndType(Uri.parse("file://" + file.toString()), type);
		// mContext.startActivity(intent);
		// System.exit(0);
		// Log.e("开始安装", "安装中，唤起界面");
		// }else{
		//
		// while(isInstalling){
		// file = new File(installFile);
		// if (!file.exists()) {
		// isInstalling = false ;
		// Intent intent = new Intent();
		// intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		// intent.setAction(Intent.ACTION_VIEW);
		// String type = "application/vnd.android.package-archive";
		// intent.setDataAndType(Uri.parse("file://" + file.toString()), type);
		// mContext.startActivity(intent);
		// System.exit(0);
		// Log.i("开始安装", "安装中，唤起界面");
		// }else{
		// try {
		// Thread.sleep(1000);
		// } catch (InterruptedException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }
		//
		// }
		//
		// }
		//

		return true;
	}

}
