#include "GameMessageIO.h"
#include "GlobalUserData.h"
#include "Util.h"

#ifdef WIN32 
#include <WinSock2.h> 
#else 
#include <sys/socket.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#endif 

#include "cocos2d.h"

GameMsgHead::GameMsgHead(GameMsgCmdC cmdC, unsigned uBodySize)
	: m_uPktSize(sizeof(GameMsgHead) + uBodySize)
	, m_uCmd(cmdC)
	, m_uBodySize(uBodySize)
{
}

void N2L(GameMsgHead& head)
{
	unsigned uPktSize = ntohl(head.m_uPktSize);
	unsigned uCmd = ntohl(head.m_uCmd);
	unsigned uBodySize = ntohl(head.m_uBodySize);
	
	head.m_uPktSize = uPktSize;
	head.m_uCmd = uCmd;
	head.m_uBodySize = uBodySize;
}

void L2N(GameMsgHead& head)
{
	unsigned uPktSize = htonl(head.m_uPktSize);
	unsigned uCmd = htonl(head.m_uCmd);
	unsigned uBodySize = htonl(head.m_uBodySize);
	
	head.m_uPktSize = uPktSize;
	head.m_uCmd = uCmd;
	head.m_uBodySize = uBodySize;
}

GameMessageIO::GameMessageIO()
	: m_gameSocket(nullptr)
{
}

GameMessageIO::~GameMessageIO()
{
	StopIO();
}

bool GameMessageIO::StartIO(const std::string& strIp, unsigned uPort)
{
	if (nullptr == m_gameSocket)
	{
		m_gameSocket = new CGameSocket();
		bool bConnected = m_gameSocket->Create(strIp.c_str(), uPort, BLOCKSECONDS, true);

		return bConnected;
	}
	return true;
}

void GameMessageIO::ProcIO()
{
	if (nullptr == m_gameSocket) 
	{
		return;
	}

	if (!m_gameSocket->Check()) 
	{
		StopIO();
		// 掉线了
		if (m_gameIOAbordProc != nullptr)
		{
			m_gameIOAbordProc();
		}
		return;
	}

	// 发送数据（向服务器发送消息）
	m_gameSocket->Flush();

	// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
	while (true)
	{
		// char buffer[_MAX_MSGSIZE] = { 0 };
		std::vector<char> buffer(_MAX_MSGSIZE);
		int nSize = sizeof(buffer);
		char* pbufMsg = &buffer[0];
		if (m_gameSocket == nullptr)
		{
			break;
		}
		if (!m_gameSocket->ReceiveMsg(pbufMsg, nSize)) 
		{
			break;
		}
		
		while (true)
		{


			GameMsgHead *pHead = (GameMsgHead*)pbufMsg;
			N2L(*pHead);

			pbufMsg += sizeof(GameMsgHead);
			nSize -= sizeof(GameMsgHead);

			unsigned uBodySize = pHead->m_uBodySize;
			ProcInGameMsg(*pHead, pbufMsg, uBodySize);

			pbufMsg	+= uBodySize;
			nSize	-= uBodySize;

			if (nSize <= 0)
			{
				break;
			}
		}
	}
}
void GameMessageIO::StopIO()
{
	if (m_gameSocket != nullptr) {
		delete m_gameSocket;
		m_gameSocket = nullptr;
	}
}

void GameMessageIO::SetGameIOAbordProc(GameIOAbordProc gameIOAbordProc)
{
	m_gameIOAbordProc = gameIOAbordProc;
}

void GameMessageIO::SetGameMsgProc(GameMsgProc gameMsgProc)
{
	m_gameMsgProc = gameMsgProc;
}

void GameMessageIO::SendGameMessage(GameMsgCmdC cmdC)
{
	SendGameMessage(cmdC, Json::Value());
}

void GameMessageIO::SendGameMessage(GameMsgCmdC cmdC, const Json::Value& body)
{
	//Json::Value param;
	//param["cmd"] = cmdC;
	//param["body"] = body;
	std::string strBody = body.toStyledString();
	cocos2d::log("sendmsg %d:%s", cmdC, strBody.c_str());
	unsigned uBodySize = strBody.size();
	GameMsgHead head(cmdC, uBodySize);
	L2N(head);
	if (m_gameSocket != nullptr)
	{
		m_gameSocket->SendMsg(&head, sizeof(head));
		m_gameSocket->SendMsg(strBody.c_str(), uBodySize);
	}
}

void GameMessageIO::ProcInGameMsg(const GameMsgHead& msgHead, const char*pBody, unsigned uBodySize)
{
	if (m_gameMsgProc != nullptr)
	{
		m_gameMsgProc(msgHead, pBody, uBodySize);
	}
}

bool GameMessageIO::check()
{
	if (nullptr == m_gameSocket) 
	{
		return false;
	}

	return m_gameSocket->Check();
}

void GameMessageIO::clear()
{
	if (nullptr == m_gameSocket) 
	{
		return ;
	}
	m_gameSocket->clear() ;
}

void GameMessageIO::Flush()
{
	if (nullptr == m_gameSocket)
		return;

	m_gameSocket->Flush();
}