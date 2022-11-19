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
    m_conflag = 0;
    memset(m_recv_buff, 0 , sizeof(m_recv_buff));
}

ConnectMgr::~ConnectMgr()
{
    close();
}

void ConnectMgr::close()
{
    m_conflag = 0;
    m_socket.close();
    m_epoll.close();
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

    m_socket.setNoBlock();

    m_epoll.create();

    INFO_LOG("start listen host=%s port:%d", m_ip, m_port);
}

int ConnectMgr::recv_data(int _fd, char* pData, int len)
{
    
}

int ConnectMgr::send_data(int _fd, char* pData, int len)
{
    ::send(_fd, pData, len, 0);
}

int ConnectMgr::new_accept(int _fd, const char* ip)
{
    m_connected_map[_fd] = ++m_conflag;
    INFO_LOG("new client accept  fd = %d ip = %s ", _fd, ip);
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
                INFO_LOG("new connect fd = %d", new_fd);
                struct sockaddr_in new_client_addr;
                if(m_socket.accept(new_fd, new_client_addr) == -1)
                {
                    ERROR_LOG("ConnectMgr run error!");
                    return -1;
                }
                m_socket.setNoBlock(new_fd);
                struct epoll_event e_event;
                e_event.data.fd = new_fd;
                e_event.events = EPOLLIN | EPOLLOUT | EPOLLET;
                m_epoll.addListen(new_fd, e_event);
                
                new_accept(new_fd, inet_ntoa(new_client_addr.sin_addr));
            }
            else if(m_events[idx].events & EPOLLIN)
            {
                //read data
                int _fd = m_events[idx].data.fd;
                std::map<int, int>::iterator iter = m_connected_map.find(_fd);
                if( iter != m_connected_map.end())
                {
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
                        else
                        {
                            ERROR_LOG("recv data falied! err = %d", errno);
                        }
                    }
                    
                    recv_data(_fd, m_recv_buff, dataLen);

                    if(dataLen != 0)
                    {
                        ERROR_LOG("recv error data! readSize=%d packageLen = %d ", readSize, dataLen);
                    }
                }
             
            }
            else if(m_events[idx].events & EPOLLOUT)
            {
                //send data
            }
		}
    }
    

}