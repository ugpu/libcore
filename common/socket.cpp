#include "socket.h"


CSocket::CSocket()
{
	memset(m_ip, 0 , sizeof(m_ip));
	m_port = -1;
	m_fd = -1;
}

CSocket::CSocket(const char* ip, const unsigned short port)
{

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


int CSocket::init(const char* ip, int port, int type = SOCK_STREAM)
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
	strcat(m_ip, ip, strlen(ip));


	m_fd = socket(AF_INET, m_sType, 0);
	if (m_fd == -1)
	{
		ERROR_LOG("init socket failed! ip = %s port = %d", ip, m_port);
		return -1;
	}
	
	return 0;

}


int CSocket::bind()
{
	if (m_ip[0] == "\0" || m_port < 1)
	{
		return -1;
	}

	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(m_port);
	if (inet_aton(m_ip, &servAddr.sin_addr) == 0)
	{
		ReleaseErrorLog("bind convert ip = %s failed", m_ip);
		return InValidIp;
	}
	
	if (::bind(m_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		ReleaseErrorLog("bind ip = %s, port = %d, error = %d, info = %s", m_ip, m_port, errno, strerror(errno));
		return BindIpPortFailed;
	}
	
	return Success;
}


int CSocket::getFd() const
{
	return m_fd;
}

const char* CSocket::getIp() const
{
	return m_ip;
}

unsigned short CSocket::getPort() const
{
	return m_port;
}