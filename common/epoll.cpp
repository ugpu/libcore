#include "epoll.h"
#include "global.h"



CEpoll::CEpoll()
{
	m_fd = -1;
}

CEpoll::~CEpoll()
{
		if (m_fd != -1)
	{
		if (::close(m_fd) != 0)
		{
			ERROR_LOG("close epoll ");
		}
		m_fd = -1;
	}
}

int CEpoll::create(int listenCount)
{
    m_fd = ::epoll_create(listenCount);
	return m_fd;
}


int CEpoll::listen(struct epoll_event* wait_events, int event_max_cnt, int timeout /* = -1*/)
{	
	int cnt = epoll_wait(m_fd, wait_events, event_max_cnt, timeout);
	if (cnt == -1)
	{
		ERROR_LOG("epoll wait error! ");
	}
	return cnt;
}


int CEpoll::addListen(int fd, struct epoll_event& ev)
{
    if (::epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{	
		ERROR_LOG("add_new_client failed!");
        return -1;
	}
	return 0;
}

int CEpoll::removeListen(int fd)
{
	struct epoll_event ev;
    ev.events = 0;
	if (::epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, &ev) == -1)
	{
		ERROR_LOG("removeListenter error!");
        return -1;
	}
	return 0;
}