package com.oo58.game.texaspoker;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;
import com.oo58.game.texaspoker.alipay.Keys;
import com.oo58.game.texaspoker.alipay.PayResult;
import com.oo58.game.texaspoker.alipay.Rsa;
import com.oo58.game.texaspoker.alipay.SignUtils;

public class AliPaying extends Activity {
	private String order_id = "";
	private String money = "";
	private String name = "";
	
	private Context mContext = null;

	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;
	
	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				PayResult payResult = new PayResult((String) msg.obj);
				
				// 支付宝返回此次支付结果及加签，建议对支付宝签名信息拿签约时支付宝提供的公钥做验签
				String resultInfo = payResult.getResult();
				
				String resultStatus = payResult.getResultStatus();

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					//Toast.makeText(mContext, "支付成功",
					//		Toast.LENGTH_SHORT).show();
					AliPaying.this.finish();
					UniPaying.setMobilePayResult(1);
				} else {
					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000”代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					if (TextUtils.equals(resultStatus, "8000")) {
						//Toast.makeText(mContext, "支付结果确认中",
						//		Toast.LENGTH_SHORT).show();
						AliPaying.this.finish();
						UniPaying.setMobilePayResult(3);
					} else {
						// 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
						//Toast.makeText(mContext, "支付失败",
						//		Toast.LENGTH_SHORT).show();
						AliPaying.this.finish();
						UniPaying.setMobilePayResult(2);
					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
//				Toast.makeText(mContext, "检查结果为：" + msg.obj,
//						Toast.LENGTH_SHORT).show();
				break;
			}
			default:
				break;
			}
		};
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		order_id = intent.getStringExtra("order_id");
		money = intent.getExtras().getString("money");
		name = intent.getExtras().getString("name");
		Log.i("lvjian", "dddddddd----money===>" + money);
		Log.i("lvjian", "dddddddd----===>" + order_id);
		// 調用支付寶
		
		pay();
	}
	
	/**
	 * call alipay sdk pay. 调用SDK支付
	 * 
	 */
	public void pay() {
		// 订单
		String orderInfo = getNewOrderInfo();

		// 对订单做RSA 签名
		String sign = sign2(orderInfo);
		try {
			// 仅需对sign 做URL编码
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		// 完整的符合支付宝参数规范的订单信息
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				try {
					// 构造PayTask 对象
					PayTask alipay = new PayTask(AliPaying.this);
					
					// 调用支付接口，获取支付结果
					String result = alipay.pay(payInfo);
					Message msg = new Message();
					msg.what = SDK_PAY_FLAG;
					msg.obj = result;
					mHandler.sendMessage(msg);
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		};

		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * check whether the device has authentication alipay account.
	 * 查询终端设备是否存在支付宝认证账户
	 * 
	 */
//	public void check(View v) {
//		Runnable checkRunnable = new Runnable() {
//
//			@Override
//			public void run() {
//				// 构造PayTask 对象
//				PayTask payTask = new PayTask(PayDemoActivity.this);
//				// 调用查询接口，获取查询结果
//				boolean isExist = payTask.checkAccountIfExist();
//
//				Message msg = new Message();
//				msg.what = SDK_CHECK_FLAG;
//				msg.obj = isExist;
//				mHandler.sendMessage(msg);
//			}
//		};
//
//		Thread checkThread = new Thread(checkRunnable);
//		checkThread.start();
//
//	}

	/**
	 * get the sdk version. 获取SDK版本号
	 * 
	 */
//	public void getSDKVersion() {
//		PayTask payTask = new PayTask(mContext);
//		String version = payTask.getVersion();
//		Toast.makeText(mContext, version, Toast.LENGTH_SHORT).show();
//	}

	/**
	 * create the order info. 创建订单信息
	 * 
	 */
	public String getNewOrderInfo() {
		// 签约合作者身份ID
		String orderInfo = "partner=" + "\"" + Keys.DEFAULT_PARTNER + "\"";

		// 签约卖家支付宝账号
		orderInfo += "&seller_id=" + "\"" + Keys.DEFAULT_SELLER + "\"";

		// 商户网站唯一订单号
		orderInfo += "&out_trade_no=" + "\"" + order_id + "\"";

		// 商品名称
		orderInfo += "&subject=" + "\"" + name + "\"";

		// 商品详情
		orderInfo += "&body=" + "\"" + "人民币兑换乐币套餐" + "\"";

		// 商品金额
		orderInfo += "&total_fee=" + "\"" + money + "\"";

		// 服务器异步通知页面路径
//		orderInfo += "&notify_url=" + "\"" + "http://60.174.249.98:8001/api/pay/malipay/notify_url.php"
//				+ "\"";

		orderInfo += "&notify_url=" + "\"" + "http://www.wepoker.cn/api/pay/malipay/notify_url.php"+ "\"";
		
		// 服务接口名称， 固定值
		orderInfo += "&service=\"mobile.securitypay.pay\"";
		
		// 支付类型， 固定值
		orderInfo += "&payment_type=\"1\"";
		
		// 参数编码， 固定值
		orderInfo += "&_input_charset=\"utf-8\"";

		// 设置未付款交易的超时时间
		// 默认30分钟，一旦超时，该笔交易就会自动被关闭。
		// 取值范围：1m～15d。
		// m-分钟，h-小时，d-天，1c-当天（无论交易何时创建，都在0点关闭）。
		// 该参数数值不接受小数点，如1.5h，可转换为90m。
		orderInfo += "&it_b_pay=\"30m\"";

		// extern_token为经过快登授权获取到的alipay_open_id,带上此参数用户将使用授权的账户进行支付
		// orderInfo += "&extern_token=" + "\"" + extern_token + "\"";

		// 支付宝处理完请求后，当前页面跳转到商户指定页面的路径，可空
		orderInfo += "&return_url=\"m.alipay.com\"";

		// 调用银行卡支付，需配置此参数，参与签名， 固定值 （需要签约《无线银行卡快捷支付》才能使用）
		// orderInfo += "&paymethod=\"expressGateway\"";

		return orderInfo;
	}

	/**
	 * sign the order info. 对订单信息进行签名
	 * 
	 * @param content
	 *            待签名订单信息
	 */
	public String sign2(String content) {
		//return SignUtils.sign(content, Keys.PRIVATE);
		return Rsa.sign(content, Keys.PRIVATE);
	}

	/**
	 * get the sign type we use. 获取签名方式
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

}
