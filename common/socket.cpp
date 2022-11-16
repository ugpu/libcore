#include "socket.h"
#include <netinet/tcp.h>

CSocket::CSocket()
{
	memset(m_ip, 0 , sizeof(m_ip));
	m_port = -1;
	m_fd = -1;
}


CSocket::~CSocket()
{
	if(m_fd > 0)
	{
		::close(m_fd);
	}
	memset(m_ip, 0 , sizeof(m_ip));
	m_port = -1;
	m_fd = -1;
}


int CSocket::setOpt(int opt_id , int opt_val , int& opt_ret)
{
	if(m_fd < 1)
	{
		return -1;
	}

	setsockopt(m_fd, opt_id, opt_val, &opt_ret, sizeof(int));
	return 0;
}

int CSocket::init(const char* ip, int port, int type /* = SOCK_STREAM */, bool is_reuseaddr /* = true */, bool is_nagale /* = false */)
{
	if(!ip)
	{
		return -1;
	}
	if(port < 1)
	{
		return -1;
	}

	m_sType = type;
	m_port  = port;
	strcat(m_ip, ip);


	m_fd = socket(AF_INET, m_sType, 0);
	if (m_fd == -1)
	{
		ERROR_LOG("init socket failed! ip = %s port = %d", ip, m_port);
		return -1;
	}

	if(is_reuseaddr)
	{
		int val = 1;
		int op_id = SOL_SOCKET;
		this->setOpt(op_id, SO_REUSEADDR , val);
	}

	if(!is_nagale)
	{
		int val = 1;
		int op_id = IPPROTO_TCP;
		this->setOpt(op_id, TCP_NODELAY, val);
	}

	this->setNoBlock();
	
	this->bind();

	this->listen(DEFAULT_LISTEN_NUM);

	return 0;
}


int CSocket::accept(int & _fd)
{
	struct sockaddr_in client_addr;
	socklen_t nLen = sizeof(client_addr);
	memset(&client_addr, 0, nLen);
	_fd = ::accept(m_fd, (struct sockaddr*)&client_addr, &nLen);
	if(_fd < 0 )
	{
		return -1;
	}
	return 0;
}

int CSocket::bind()
{
	if (m_fd < 0) return -1;
	if (m_ip[0] == '\0' || m_port < 1) return -1;

	struct sockaddr_in _addr;
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(m_port);
	if (inet_aton(m_ip, &_addr.sin_addr) == 0)
	{
		return -2;
	}
	
	if (::bind(m_fd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
	{
		return -3;
	}
	
	return 0;
}


int CSocket::listen(int num /* = DEFAULT_LISTEN_NUM */)
{
	if (num < 1) return -1;
	
	if (::listen(m_fd, num) == -1)
	{
		return -2;
	}
	
	return 0;
}

int CSocket::connect()
{
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(m_port);
	if (::connect(m_fd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1)
	{
		return -1;
	}
	
	return 0;
}


int CSocket::setNoBlock()
{
	int flag = fcntl(m_fd, F_GETFL, 0);
	return fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
}

int CSocket::setNoBlock(int fd)
{
	int flag = fcntl(m_fd, F_GETFL, 0);
	return fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
}

int CSocket::setNagle(int val)
{
	if (setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == -1)
	{
		return -1;
	}
	
	return 0;
}


int CSocket::getFD() const
{
	return m_fd;
}

char* CSocket::getIP()
{
	return m_ip;
}

int CSocket::getPort()
{
	return (int)m_port;
}