#include "connectMgr.h"
#include "define.h"

CConnectManager::CConnectManager(const char* ip, const int port)
{
    m_status     = -1;
    m_socket.init(ip, port);
    m_maxMsgNum  = SVR_CONN_MAX_MSG_NUM;
    m_listernNum = SVR_CONN_MAX_LISTEN_NUM;
}


CConnectManager::~CConnectManager()
{

}