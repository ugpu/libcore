/*
	auth: ugpu
	date: 11/14/2022
	description: Connect Mgr / use in the server
*/


#ifndef CCONECTMGR_H
#define CCONECTMGR_H

#include "socket.h"

class ConnectMgr
{
public:
	ConnectMgr(const char* ip, const int port);
	~ConnectMgr();

public:
	int start();

protected:
	ConnectMgr();
private:
	int       m_status;
	CSocket   m_socket;
	int       m_maxMsgNum;
	int       m_listernNum;
	char      m_ip[128];
	int       m_port;
};

#endif