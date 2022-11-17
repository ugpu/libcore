#include "connectMgr.h"
#include "define.h"

ConnectMgr::ConnectMgr()
{
    ConnectMgr("'\0'", -1);
}

ConnectMgr::ConnectMgr(const char* ip, const int port)
{
    m_work       = false;
    m_maxMsgNum  = SVR_CONN_MAX_MSG_NUM;
    m_listernNum = SVR_CONN_MAX_LISTEN_NUM;

    memset(m_ip, 0 ,sizeof(m_ip));
    strcpy(m_ip, ip);
    m_port = port;

    memset(m_recv_buff, 0 , sizeof(m_recv_buff));
}

ConnectMgr::~ConnectMgr()
{

}
void ConnectMgr::set_port(const int port)
{
    m_port = port;
}

void ConnectMgr::set_ip(const char * ip)
{
    memset(m_ip, 0 ,sizeof(m_ip));
    strcpy(m_ip, ip);
}


bool ConnectMgr::is_run()
{
    return m_work;
}

int ConnectMgr::init_con()
{
    m_socket.init(m_ip, m_port);

    m_socket.bind();

    m_socket.listen();

    m_epoll.create();

    INFO_LOG("start listen host=%s port:%d", m_ip, m_port);
}

int ConnectMgr::work()
{
    DEBUG_LOG("connect_mgr start run!");
    m_epoll.create();

    m_epoll_event.data.fd = m_socket.getFD();  
    m_epoll_event.events = EPOLLIN | EPOLLET;
    m_epoll.addListen(m_socket.getFD(), m_epoll_event);


    m_work = true;
    int fdCount = 0;
    struct in_addr client_addr;
    while(m_work)
    {   
        fdCount = m_epoll.listen(m_events, MAX_LISTEN_EVENT_CNT, 100);
        for (int idx = 0; idx < fdCount; ++idx)
		{
            if(m_events[idx].data.fd == m_socket.getFD())
            {
                //new connect
                
                int new_fd = m_events[idx].data.fd;
                DEBUG_LOG("new connect fd = %d", new_fd);
                if(m_socket.accept(new_fd) == -1)
                {
                    ERROR_LOG("ConnectMgr run error!");
                    return -1;
                }
                m_socket.setNoBlock(new_fd);
                struct epoll_event e_event;
                e_event.data.fd = new_fd;
                e_event.events = EPOLLIN | EPOLLOUT | EPOLLET;
                m_epoll.addListen(new_fd, e_event);

            }
            else if(m_events[idx].events & EPOLLIN)
            {
                //read data
                int _fd = m_events[idx].data.fd;
                char chPkgLen[sizeof(int)] = {0};
                int flagLen = ::recv(_fd, chPkgLen, sizeof(chPkgLen), 0);
                if(flagLen == 0)
                {
                    m_epoll.removeListen(_fd);
                    continue;
                }

                int readSize = 0;
                int dataLen = *(int*)chPkgLen;
                //write package len
                DEBUG_LOG(" flagLen = %d", flagLen);
                DEBUG_LOG("socket =%d packageLen data len = %d", _fd, dataLen);
                if(flagLen > 0 )
                {
                    readSize += flagLen;
                    memcpy(m_recv_buff, chPkgLen, sizeof(chPkgLen));
                }
                while(dataLen > 0)
                {
                    int ret = ::recv(_fd, m_recv_buff + readSize, sizeof(m_recv_buff) - readSize, 0);
                    if (ret == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                    }
                    if(ret > 0)
                    {
                        readSize += ret;
                        dataLen -= ret;
                    }
                }
                
                DEBUG_LOG("socket =%d read data len = %d", _fd, readSize);
                recv_data(_fd, m_recv_buff, readSize);

                /*        test code!    */
                if(dataLen != 0)
                {
                    ERROR_LOG("recv error data! readSize=%d packageLen = %d ", readSize, dataLen);
                }
                /*        test code!    */
            }
            else if(m_events[idx].events & EPOLLOUT)
            {
                //send data
            }
		}
    }
    

}