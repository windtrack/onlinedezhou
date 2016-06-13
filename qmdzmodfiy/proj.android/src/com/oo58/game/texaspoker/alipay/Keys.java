/*
 * Copyright (C) 2010 The MobileSecurePay Project
 * All right reserved.
 * author: shiqun.shi@alipay.com
 * 
 *  提示：如何获取安全校验码和合作身份者id
 *  1.用您的签约支付宝账号登录支付宝网站(www.alipay.com)
 *  2.点击“商家服务”(https://b.alipay.com/order/myorder.htm)
 *  3.点击“查询合作者身份(pid)”、“查询安全校验码(key)”
 */

package com.oo58.game.texaspoker.alipay;

//
// 请参考 Android平台安全支付服务(msp)应用开发接口(4.2 RSA算法签名)部分，并使用压缩包中的openssl RSA密钥生成工具，生成一套RSA公私钥。
// 这里签名时，只需要使用生成的RSA私钥。
// Note: 为安全起见，使用RSA私钥进行签名的操作过程，应该尽量放到商家服务器端去进行。
public final class Keys {

	//合作身份者id，以2088开头的16位纯数字
	public static final String DEFAULT_PARTNER = "2088021781462186";

	//收款支付宝账号
	public static final String DEFAULT_SELLER = "2355708761@qq.com";

	//商户私钥，自助生成
	public static final String PRIVATE ="MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBANnTCjCkxChFcYCqxwjMwgjvQtZDhF1TV+layBNn/2xDzdkWL3S1IodCaRrXQAU3BbJ8t8VL/sYKUr3U+eGVKrlvDYN7khkaJgXiRQLEXXGkSNBohMSEiG2UckwFjZa/mYKQ8f9n8N1YzptdijOafOOf5KBEuIhZyksPDja3pawlAgMBAAECgYAMK9yv5yUG0wqKlABXH4ngEe58Jn2o8VVCNs5jNzZwiEFiVfkE3nI1eKKC23A6Wl65IHrsTVjJHB6PeO0Vi849P/Div6r/LXLiuEcAT782fpeXRxequPvZnrAkmKRL1MXL8osYMO7Fz4xwVJvM/QQ8PlknGcj5IyrCbXOF1YhxwQJBAO83WprXgqV8gxQSK3WmQELae0LK3+DqN8U3suELvkcitePowL2uHDaE5OXGVpnZUJ7kQSRLwmdvsKvFedOlSm0CQQDpG3WY4dEBuvnYkHm4KzjN68FQ3NwQohUx9FJXeh9r6CN6EiQ8/5dyYlQa61GwuC280HQsTc9N8IMp9reA/lWZAkAp9xL9APuZXmt8VOes3vp8/+kJwZ9ezJf2EBxgYHgD8LHqBoXxgJ7QbaXeLVo56Sa8iRxoaqXzEWPz7lO97kxNAkAderuEegpUjbDLEnpSMCUe5ZGtCEFMWTMU/jDrnyYaH2FdLq4miKw+avD39op+LsyZ9EK815o/4Qq1Y94REhkBAkA/Wp7+gV72GzNT9fiddSyu380eNQrkiZuTkMdEv5s8ufevmiXX9Rx8QNjJg/Aaz/iuvIN7DztwcE/t3yegQBMB";
	public static final String PUBLIC = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB";

}
