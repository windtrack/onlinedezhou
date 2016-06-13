#ifndef __ROOMMESSAGEIO_H__
#define __ROOMMESSAGEIO_H__

#include "GameSocket.h"
#include "jsonrpc/json/value.h"
#include <functional>
#include <vector>
#include <utility>

enum class RoomMsgCmdS : unsigned
{
	Chat = 2,
	RepeatLogin = 10
};

enum class RoomMsgCmdC : unsigned
{
	Logon = 7,
	Chat = 2
};

typedef std::function<void()> RoomIOAbordProc;
typedef std::function<void(RoomMsgCmdS, const char *, unsigned)> RoomMsgProc;

class RoomMessageIO
{
public:
	RoomMessageIO();
	~RoomMessageIO();

	bool StartIO(const std::string& strIp, unsigned uPort);
	void ProcIO();
	void StopIO();

	void SetRoomIOAbordProc(RoomIOAbordProc roomIOAbordProc);
	void SetRoomMsgProc(RoomMsgProc roomMsgProc);
	
	void SendRoomMessage(RoomMsgCmdC cmdC, const Json::Value& msg);
	void SendRoomMessage(RoomMsgCmdC cmdC, const unsigned char *pMsg, unsigned uMsgSize);

	void clear() ;
protected:
	void ProcInRoomMsg(RoomMsgCmdS cmdS, char *pMsgBody, unsigned uMsgBodySize);


private:
	CGameSocket *m_gameSocket;
	RoomIOAbordProc m_roomIOAbordProc;
	RoomMsgProc m_roomMsgProc;
};


#endif