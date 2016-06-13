package com.oo58.game;

import java.util.UUID;

import org.alexd.jsonrpc.JSONRPCException;
import org.alexd.jsonrpc.JSONRPCHttpClient;
import org.apache.http.client.HttpClient;
import org.apache.http.params.CoreProtocolPNames;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class MyJSONRPCHttpClient extends JSONRPCHttpClient {
	public MyJSONRPCHttpClient(HttpClient cleint, String uri) {
		super(cleint, uri);
		cleint.getParams().setParameter(CoreProtocolPNames.USER_AGENT, "android 0058/" + UpdateUtils.localVersion);
	}

	public MyJSONRPCHttpClient(String uri) {
		super(uri);
	}

	protected JSONObject doRequest(String method, Object[] params)
			throws JSONRPCException {
		// Copy method arguments in a json array
		JSONArray jsonParams = new JSONArray();
		for (int i = 0; i < params.length; i++) {
			if (params[i].getClass().isArray()) {
				jsonParams.put(getJSONArray((Object[]) params[i]));
			}
			jsonParams.put(params[i]);
		}

		// Create the json request object
		JSONObject jsonRequest = new JSONObject();
		try {
			jsonRequest.put("id", UUID.randomUUID().hashCode());
			jsonRequest.put("method", method);
			jsonRequest.put("params", jsonParams);
			jsonRequest.put("jsonrpc", "2.0");
		} catch (JSONException e1) {
			throw new JSONRPCException("Invalid JSON request", e1);
		}
		return doJSONRequest(jsonRequest);
	}

}
