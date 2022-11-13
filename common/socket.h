/*
	auth: ugpu
	date: 11/13/2022
	description: common base socket  . the class not use and only suport addtiopn other fun class
*/


class CSocket
{
public:
    CSocket();
	CSocket(const char* ip, const unsigned short port);

	//common base use virtual,  not memery leak. 
	virtual ~CSocket();
	
public:
	static const char* toIPStr(const struct in_addr& ip);

	//init socker
	//type: SOCK_STREAM&SOCK_DGRAM
	int init(const char* ip, int port, int type = SOCK_STREAM);

	int bind();
	int listen(int num);


	//no block. epoll et mode only suprt noBlock. default use;
	int setNoBlock(int fd);


	int getFD();
	char* getIP();
	int getPort();


private:
	chat[128]  m_ip;
    int m_port;
	int m_fd;
	int m_sType;
