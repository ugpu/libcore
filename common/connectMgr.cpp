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

    m_epoll_event.data.fd= m_socket.getFD();  
    m_epoll_event.events= EPOLLIN|EPOLLET;
    m_epoll.addListen(m_socket.getFD(), m_epoll_event);


    m_work = true
    int idx = 0;
    int fdCount = 0;
    struct in_addr client_addr;
    while(m_work)
    {   
        fdCount = m_epoll.listen(m_events, MAX_LISTEN_EVENT_CNT, 100);
        for (idx = 0; idx < fdCount; ++idx)
		{
            if(m_events[idx].data.fd == m_socket.getFD())
            {
                //new connect
                int new_fd = -1;
                if(m_socket.accept(new_fd) == -1)
                {
                    ERROR_LOG("ConnectMgr run error!");
                    return -1;
                }
                connsockfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
                if(connsockfd < 0)
                {
                    fprintf(stderr, "accept function failed.\n");
                    exit(RT_ERR);
                }
                fprintf(stdout, "accept a new session from %s port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    
                m_epoll_event.data.fd = new_fd;
                m_epoll_event.events = EPOLLIN|EPOLLET; 
                epoll_ctl(epfd, EPOLL_CTL_ADD, connsockfd, &event);

            }

			conn = (Connect*)(waitEvents[i].data.ptr);			
			if (conn->fd == listenConn.fd)
			{
				acceptConnect(waitEvents[i].events, conn);        // 建立新连接
			}
			else if (conn->id != 0)
			{
				handleConnect(waitEvents[i].events, conn);        // 从已建立的连接读写业务逻辑数据
			}
			else
			{
				onActiveConnect(waitEvents[i].events, conn);      // 主动建立的连接需要初始化才能读写业务逻辑数据
			}
		}
    }
    

}