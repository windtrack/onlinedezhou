#include "RpcRoutine.h"
#include "jsonrpc/client.h"
#include "jsonrpc/connectors/httpclient.h"
#include  <chrono>
#include "cocos2d.h"

RpcRoutine::RpcRoutine()
	: m_bContinue(true)
{
}
RpcRoutine::~RpcRoutine()
{
	Stop();
}

void RpcRoutine::SetRpcResultProc(FuncRpcResultProc funcRpcResultProc)
{
	m_funcRpcResultProc = funcRpcResultProc;
}

void RpcRoutine::Start(const std::string& strRpcUrl, const std::string& strHttpUserAgent)
{
	m_bContinue = true;
	m_thread = std::thread(std::bind(&RpcRoutine::ThreadRoutine, this, strRpcUrl, strHttpUserAgent));
}

void RpcRoutine::Stop()
{
	if (m_bContinue)
	{
		m_bContinue = false;
		std::unique_lock<std::mutex> uniLock(m_mutex);
		m_conditionVar.notify_one();
	}
	if (m_thread.joinable())
		m_thread.join();
}

void RpcRoutine::AddRequest(const RPC& rpc)
{
	std::unique_lock<std::mutex> uniLock(m_mutex);
	m_deqRequest.Push(rpc);
	m_conditionVar.notify_one();
}

void RpcRoutine::ProcResult()
{
	RPC rpcResult;
	if (m_deqResult.TryPop(rpcResult))
	{
		if (m_funcRpcResultProc != nullptr)
		{
			m_funcRpcResultProc(rpcResult);
		}
	}
}

void RpcRoutine::clear()
{
	RPC rpcResult;
	if (m_deqResult.TryPop(rpcResult))
	{
		if (m_funcRpcResultProc != nullptr)
		{
			rpcResult.m_rpcResult.clear() ;
		}
	}
}
void RpcRoutine::ThreadRoutine(const std::string& strRpcUrl, const std::string& strHttpUserAgent)
{
	std::string rpcUrl(strRpcUrl);
	std::string userAgent(strHttpUserAgent);

	RPC rpc;
	while (m_bContinue)
	{
		if (m_deqRequest.Pop(rpc))
		{
			try
			{
				cocos2d::log("start rpc:%s \n %s", rpc.m_rpcName.c_str(), rpc.m_rpcParam.toStyledString().c_str());
				jsonrpc::Client client(new jsonrpc::HttpClient(rpcUrl));
				rpc.m_rpcResult = client.CallMethod(rpc.m_rpcName, userAgent, rpc.m_rpcParam);
			}
			catch(jsonrpc::JsonRpcException e)
			{
				rpc.m_rpcException = e.what();
			}
			catch(...)
			{
				rpc.m_rpcException = "rpc unknown exception";
			}
			cocos2d::log("end rpc:%s", rpc.m_rpcName.c_str());
			m_deqResult.Push(rpc);
		}
		else
		{
			std::unique_lock<std::mutex> uniLock(m_mutex);
			m_conditionVar.wait(uniLock);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}