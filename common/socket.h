/*
	auth: ugpu
	date: 11/13/2022
	description: common base socket  . the class not use and only suport addtiopn other fun class . all noblock!
*/
#ifndef CSOCKET_H
#define CSOCKET_H
#include "global.h"

#include "define.h"

class CSocket
{
public:
    CSocket();
	//common base use virtual,  not memery leak. 
	virtual ~CSocket();
	
public:
	//init socker
	//type: SOCK_STREAM&SOCK_DGRAM
	int init(const char* ip = "", int port = -1, int type = SOCK_STREAM, bool is_reuseaddr = true, bool is_nagale = false);

	int bind();
	int accept(int & _fd);
	int listen(int num = DEFAULT_LISTEN_NUM);
	int connect(char* ip, int port);
	//no block. epoll et mode only suprt noBlock. default use;
	int setNoBlock();
	int setNoBlock(int fd);
	int setNagle(int val);
	int setOpt(int opt_id, int opt_val , int& opt_ret);

	int getFD() const;
	char* getIP();
	int getPort();

public:
	int send(const char* pData, int len);
	int recv(char* pRecvDataBuff, int buffLen);


private:
	char m_ip[128];
    int  m_port;
	int  m_fd;
	int  m_sType;
};

#endif