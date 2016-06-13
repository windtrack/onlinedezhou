/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.cpp
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "client.h"
#include "rpcprotocolclient.h"
#include "exception.h"


namespace jsonrpc
{
    Client::Client(AbstractClientConnector* connector)
            : connector(connector)
    {
    }

    Client::~Client()
    {
        delete this->connector;
    }

    void Client::CallMethod(const std::string &name,const std::string &agent, const Json::Value &paramter, Json::Value& result) throw(JsonRpcException)
    {
        std::string request, response;
        protocol.BuildRequest(name, paramter, request, false);
        connector->SendRQMessage(request,agent, response);
        protocol.HandleResponse(response, result);
		//\u57ce\u2606\u7ba1\u2602\u65e0\u2606\u75d5  100351626
    }
    
    Json::Value Client::CallMethod(const std::string& name,const std::string &agent,const Json::Value& parameter) throw(JsonRpcException)
    {
        Json::Value result;
        this->CallMethod(name,agent, parameter, result);
        return result;
    }
    
    void Client::CallNotification(const std::string& name,const std::string &agent, const Json::Value& parameter) throw(JsonRpcException)
    {
        std::string request, response;
        protocol.BuildRequest(name, parameter, request, true);
        connector->SendRQMessage(request,agent, response);
    }

} /* namespace jsonrpc */
