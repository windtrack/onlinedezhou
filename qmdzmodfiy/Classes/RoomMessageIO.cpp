#include "RoomMessageIO.h"
#include "jsonrpc/json/writer.h"
#include "Util.h"


#ifdef WIN32 
#include <WinSock2.h> 
#else 
#include <sys/socket.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#define SOCKET int 
#define SOCKET_ERROR -1 
#define INVALID_SOCKET -1 
#endif 




struct RoomMessageHead 
{
	RoomMessageHead();
	~RoomMessageHead();
	RoomMessageHead(unsigned uTID, unsigned uBodySize);
	void ResetHead(unsigned uTID, unsigned uBodySize);

	unsigned m_uPacketSize;
	unsigned m_uTID;
	unsigned m_uBodySize;
};

struct RoomMessage
{
	RoomMessage(unsigned uTID, unsigned uBodySize);
	RoomMessage(const RoomMessageHead& roomMessageHead);
	~RoomMessage();

	char *GetBuffer() const;
	unsigned GetBufferSize() const;

	char *GetBodyBuffer() const;
	unsigned GetBodyBufferSize() const;

	unsigned GetTID() const;

	RoomMessageHead *m_pHead;
};

RoomMessageHead::RoomMessageHead()
	: m_uPacketSize(sizeof(*this))
	, m_uTID(0)
	, m_uBodySize(0)
{
}

RoomMessageHead::RoomMessageHead(unsigned uTID, unsigned uBodySize)
	: m_uPacketSize(sizeof(*this) + uBodySize)
	, m_uTID(uTID)
	, m_uBodySize(uBodySize)
{
}
RoomMessageHead::~RoomMessageHead()
{

}
void RoomMessageHead::ResetHead(unsigned uTID, unsigned uBodySize)
{
	m_uPacketSize = (sizeof(*this) + uBodySize);
	m_uTID = uTID;
	m_uBodySize = uBodySize;
}

bool N2L(RoomMessageHead& head)
{
	u_long uPacketSize = ntohl(head.m_uPacketSize);
	u_long uTID = ntohl(head.m_uTID);
	u_long uBodySize = ntohl(head.m_uBodySize);
	if (uPacketSize < sizeof(RoomMessageHead) ||
		uBodySize + sizeof(RoomMessageHead) != uPacketSize)
	{
		return false;
	}
	head.m_uPacketSize = uPacketSize;
	head.m_uTID = uTID;
	head.m_uBodySize = uBodySize;
	return true;
}

bool L2N(RoomMessageHead& head)
{
	if (head.m_uPacketSize < sizeof(RoomMessageHead) ||
		head.m_uBodySize + sizeof(RoomMessageHead) != head.m_uPacketSize)
	{
		return false;
	}
	head.m_uPacketSize = htonl(head.m_uPacketSize);
	head.m_uTID = htonl(head.m_uTID);
	head.m_uBodySize = htonl(head.m_uBodySize);
	return true;
}

RoomMessage::RoomMessage(unsigned uTID, unsigned uBodySize)
	: m_pHead(nullptr)
{
	m_pHead = (RoomMessageHead*)new char[uBodySize + sizeof(RoomMessageHead)];
	memset(m_pHead, 0, sizeof(char)*(uBodySize + sizeof(RoomMessageHead)));
	m_pHead->ResetHead(uTID, uBodySize);
}

RoomMessage::RoomMessage(const RoomMessageHead& roomMessageHead)
	: m_pHead(nullptr)
{
	m_pHead = (RoomMessageHead*)new char[roomMessageHead.m_uPacketSize];
	memset(m_pHead, 0, sizeof(char)*roomMessageHead.m_uPacketSize);
	m_pHead->ResetHead(roomMessageHead.m_uTID, roomMessageHead.m_uBodySize);
}

RoomMessage::~RoomMessage()
{
	delete [] (char*)m_pHead;
	m_pHead = nullptr;
}

char *RoomMessage::GetBuffer() const
{
	if (m_pHead)
	{
		return (char*)m_pHead;
	}
	return nullptr;
}

unsigned RoomMessage::GetBufferSize() const
{
	if (m_pHead)
	{
		return m_pHead->m_uPacketSize;
	}
	return 0;
}

char *RoomMessage::GetBodyBuffer() const
{
	if (m_pHead)
	{
		return (char*)(m_pHead+1);
	}
	return nullptr;
}
unsigned RoomMessage::GetBodyBufferSize() const
{
	if (m_pHead)
	{
		return m_pHead->m_uBodySize;
	}
	return 0;
}

unsigned RoomMessage::GetTID() const
{
	if (m_pHead)
	{
		return m_pHead->m_uTID;
	}
	return 0;
}

RoomMessageIO::RoomMessageIO()
	: m_gameSocket(nullptr)
{
}

RoomMessageIO::~RoomMessageIO()
{
	StopIO();
}

bool RoomMessageIO::StartIO(const std::string& strIp, unsigned uPort)
{
	if (nullptr == m_gameSocket)
	{
		m_gameSocket = new CGameSocket();
		bool bConnected = m_gameSocket->Create(strIp.c_str(), uPort, BLOCKSECONDS, true);
		return bConnected;
	}
	return true;
}


void RoomMessageIO::ProcIO()
{
	if (nullptr == m_gameSocket) 
	{
		return;
	}

	if (!m_gameSocket->Check()) 
	{
		StopIO();
		// 掉线了
		if (m_roomIOAbordProc != nullptr)
		{
			m_roomIOAbordProc();
		}
		return;
	}

	// 发送数据（向服务器发送消息）
	m_gameSocket->Flush();
	// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
	while (true)
	{
		char buffer[_MAX_MSGSIZE] = { 0 };
		int nSize = sizeof(buffer);
		char* pbufMsg = buffer;
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
			RoomMessageHead *pHead = (RoomMessageHead*)pbufMsg;
			if (!N2L(*pHead))
			{
				break;
			}

			if (pHead->m_uPacketSize > nSize)
			{
				break;
			}

			ProcInRoomMsg(static_cast<RoomMsgCmdS>(pHead->m_uTID), pbufMsg + sizeof(RoomMessageHead), pHead->m_uBodySize);

			pbufMsg	+= pHead->m_uPacketSize;
			nSize	-= pHead->m_uPacketSize;

			if (nSize <= 0)
			{
				break;
			}
		}
	}
}

void RoomMessageIO::StopIO()
{
    if (m_gameSocket != nullptr) {
        delete m_gameSocket;
        m_gameSocket = nullptr;
    }
}

void RoomMessageIO::SetRoomIOAbordProc(RoomIOAbordProc roomIOAbordProc)
{
	m_roomIOAbordProc = roomIOAbordProc;
}

void RoomMessageIO::SetRoomMsgProc(RoomMsgProc roomMsgProc)
{
	m_roomMsgProc = roomMsgProc;
}

void RoomMessageIO::ProcInRoomMsg(RoomMsgCmdS cmdS, char *pMsgBody, unsigned uMsgBodySize)
{
	if (m_roomMsgProc != nullptr)
	{
		m_roomMsgProc(cmdS, pMsgBody, uMsgBodySize);
	}
}

void RoomMessageIO::SendRoomMessage(RoomMsgCmdC cmdC, const Json::Value& msg)
{
	Json::FastWriter fast_writer;
	std::string strBody = fast_writer.write(msg);
	SendRoomMessage(cmdC, (const unsigned char *)strBody.c_str(), strBody.length());
}

void RoomMessageIO::SendRoomMessage(RoomMsgCmdC cmdC, const unsigned char *pMsg, unsigned uMsgSize)
{
	RoomMessageHead head(as_integer(cmdC), uMsgSize);
	L2N(head);
	if (m_gameSocket != nullptr)
	{
		m_gameSocket->SendMsg(&head, sizeof(head));
		m_gameSocket->SendMsg(pMsg, uMsgSize);
	}
}

void RoomMessageIO::clear()
{
	if (m_gameSocket==nullptr)
	{
		return ;
	}
	m_gameSocket->clear() ;
}