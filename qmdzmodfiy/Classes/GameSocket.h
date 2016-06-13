#ifndef __GAMESCOKET_H__
#define __GAMESCOKET_H__

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


#define _MAX_MSGSIZE (65536)     // 暂定一个消息最大为16k 

#ifndef CHECKF 
#define CHECKF(x)
//
//#define CHECKF(x) \ 
//do \ 
//{ \ 
//if (!(x)) { \ 
//	log_msg("CHECKF", #x, __FILE__, __LINE__); \ 
//	return 0; \ 
//} \ 
//} while (0) 

#endif 


#define BLOCKSECONDS    12          // INIT函数阻塞时间 
#define INBUFSIZE   (65536)       //? 具体尺寸根据剖面报告调整  接收数据的缓存 
#define OUTBUFSIZE  (4096)        //? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次 

class CGameSocket { 
public: 
	CGameSocket(void);
	~CGameSocket();
	bool    Create(const char* pszServerIP, int nServerPort, int nBlockSec, bool bKeepAlive); 
	bool    SendMsg(const void* pBuf, int nSize); 
	bool    ReceiveMsg(void* pBuf, int& nSize); 
	bool    Flush(void); 
	bool    Check(void); 
	void    Destroy(void); 
	SOCKET  GetSocket(void) const { return m_sockClient; } 
	void clear() ;
private: 
	bool    recvFromSock(void);     // 从网络中读取尽可能多的数据 
	bool    hasError();         // 是否发生错误，注意，异步模式未完成非错误 
	void    closeSocket(); 

	SOCKET  m_sockClient; 

	// 发送数据缓冲 
	char    m_bufOutput[OUTBUFSIZE];    //? 可优化为指针数组 
	int     m_nOutbufLen; 

	// 环形缓冲区 
	// char    m_bufInput[INBUFSIZE]; 
	char    *m_bufInput;
	int     m_nInbufLen; 
	int     m_nInbufStart;              // INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1) 
}; 


#endif