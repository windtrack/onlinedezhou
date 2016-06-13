/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include "../clientconnector.h"
#include "../exception.h"
#ifdef WIN32
#include "curl/curl.h"
#else 
#include <curl/curl.h>
#endif 
namespace jsonrpc
{
    
    class HttpClient : public AbstractClientConnector
    {
        public:
            HttpClient(const std::string& url) throw (JsonRpcException);
            virtual ~HttpClient();

            virtual void SendRQMessage(const std::string& message, const std::string &agent,std::string& result) throw (JsonRpcException);

            void SetUrl(const std::string& url);

        private:
            std::string url;
            CURL* curl;
    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
