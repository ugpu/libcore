/*
	auth: ugpu
	date: 11/14/2022
	description: tcp connect / use in the client
*/


#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include "socket.h"
#include "define.h"
#include "thread.h"
#include "global.h"
#include "shareQueue.h"

class CTcpConnect : public CThread
{
public:
	CTcpConnect();
	~CTcpConnect();


    void connect(const char* ip, const int port, bool isNoblock = true);

    void disconnect();
    virtual int run();

    // @pBuff: get data buff
    // @buffSize: buff size
    // @ret: return read size
    virtual int read_data(char* pBuff, int buffSize, int & ret);
    virtual int send_data(const char* pBuff, int buffSize);

private:
	bool          m_work;
	CSocket       m_socket;
    char          m_cache_buff[PACKAGE_MAX_SIZE];
    struct net_msg_buff  m_msg_pack; 
    CSharedQueue<net_msg_buff> m_msg_queue;
};




#endif