#include "connectMgr.h"
#include "define.h"

ConnectMgr::ConnectMgr()
{

}

ConnectMgr::ConnectMgr(const char* ip, const int port)
{
    m_work       = false;
    m_maxMsgNum  = SVR_CONN_MAX_MSG_NUM;
    m_listernNum = SVR_CONN_MAX_LISTEN_NUM;

    memset(m_ip, 0 ,sizeof(m_ip));
    strcpy(m_ip, ip);
    m_port = port;
}

ConnectMgr::~ConnectMgr()
{

}

bool ConnectMgr::is_run()
{
    return m_work;
}

int ConnectMgr::start()
{
    m_socket.init(m_ip, m_port);

    m_epoll.create();
}

int ConnectMgr::run()
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
                DEBUG_LOG("read data fd = %d", _fd);
                char _buff[4096] = {0};
                ::recv(_fd, _buff, sizeof(_buff), 0);
                DEBUG_LOG("recv new data = %s", _buff);
            }
            else if(m_events[idx].events & EPOLLOUT)
            {
                //send data
            }
		}
    }
    

}