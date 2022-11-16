/*
	auth: ugpu
	date: 11/14/2022
	description: Connect Mgr / use in the server
*/


#ifndef CCONECTMGR_H
#define CCONECTMGR_H

#include "socket.h"
#include "define.h"

class ConnectMgr
{
public:
	ConnectMgr(const char* ip, const int port);
	~ConnectMgr();

public:
	int start();

	void run();

	bool is_run();

protected:
	ConnectMgr();
private:
	bool      m_work;
	CSocket   m_socket;
	int       m_maxMsgNum;
	int       m_listernNum;
	char      m_ip[128];
	int       m_port;
	CEpoll    m_epoll;

	struct  epoll_event   m_epoll_event;
	struct  epoll_event   m_events[MAX_LISTEN_EVENT_CNT];
};

#endif