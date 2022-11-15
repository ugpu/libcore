#include "connectMgr.h"
#include "define.h"

ConnectMgr::ConnectMgr()
{

}

ConnectMgr::ConnectMgr(const char* ip, const int port)
{
    m_status     = -1;
    
    m_maxMsgNum  = SVR_CONN_MAX_MSG_NUM;
    m_listernNum = SVR_CONN_MAX_LISTEN_NUM;

    memset(m_ip, 0 ,sizeof(m_ip));
    strcpy(m_ip, ip);
    m_port = port;
}

ConnectMgr::~ConnectMgr()
{

}

int ConnectMgr::start()
{
    m_socket.init(m_ip, m_port);
}