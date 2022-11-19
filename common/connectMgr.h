/*
	auth: ugpu
	date: 11/14/2022
	description: Connect Mgr / use in the server
*/


#ifndef CCONECTMGR_H
#define CCONECTMGR_H

#include "socket.h"
#include "define.h"
#include "epoll.h"
#include <map>
class ConnectMgr
{
public:
	ConnectMgr();
	ConnectMgr(const char* ip, const int port);
	~ConnectMgr();

public:
	int init_con();

	int work();

	bool is_run();

	void set_port(const int port);
	void set_ip(const char * ip);

	void close();

	virtual int new_accept(int _fd, const char* ip);

	virtual int recv_data(int _fd, char* pData, int len);

	virtual int send_data(int _fd, char* pData, int len);
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

	char      m_recv_buff[PACKAGE_MAX_SIZE];

	std::map <int, int> m_connected_map;

	int       m_conflag;
};

#endif