/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    clientconnector.cpp
 * @date    05.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "clientconnector.h"
#include <cstdlib>

using namespace std;

namespace jsonrpc
{
    AbstractClientConnector::~AbstractClientConnector()
    {

    }

    string AbstractClientConnector::SendRQMessage(const string &message,const std::string &agent) throw (JsonRpcException)
    {
        string result;
        this->SendRQMessage(message,agent, result);
        return result;
    }
}
