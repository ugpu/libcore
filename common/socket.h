/*
	auth: ugpu
	date: 11/13/2022
	description: common base socket  . the class not use and only suport addtiopn other fun class
*/
#ifndef CSOCKET_H
#define CSOCKET_H
#include "global.h"

class CSocket
{
public:
    CSocket();
	//common base use virtual,  not memery leak. 
	virtual ~CSocket();
	
public:
	//init socker
	//type: SOCK_STREAM&SOCK_DGRAM
	int init(const char* ip, int port, int type = SOCK_STREAM);

	int bind();
	int listen(int num);
	int connect();
	//no block. epoll et mode only suprt noBlock. default use;
	int setNoBlock(int fd = -1);


	int getFD() const;
	char* getIP();
	int getPort();


private:
	char m_ip[128];
    int  m_port;
	int  m_fd;
	int  m_sType;
};

#endif