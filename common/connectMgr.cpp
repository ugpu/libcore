#include "connectMgr.h"
#include "define.h"

ConnectMgr::ConnectMgr(const char* ip, const int port)
{
    m_status     = -1;
    m_socket.init(ip, port);
    m_maxMsgNum  = SVR_CONN_MAX_MSG_NUM;
    m_listernNum = SVR_CONN_MAX_LISTEN_NUM;
}


ConnectMgr::~ConnectMgr()
{

}