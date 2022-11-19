/*
	auth: ugpu
	date: 11/13/2022
	description: epoll api
*/

#ifndef CEPOLL_H
#define CEPOLL_H

#include "global.h"
#include "define.h"
struct epoll_event;
class CEpoll
{
public:
	CEpoll();
	~CEpoll();
	
public:
	int create(int listenCount = DEFAULT_LISTEN_NUM);

	int listen(struct epoll_event* wait_events, int event_max_cnt, int timeout  = -1);

	int addListen(int fd, struct epoll_event& ev);

	
	int removeListen(int fd);

	int close();
private:
	CEpoll(const CEpoll&);
	CEpoll& operator =(const CEpoll&);
	
private:
    int m_fd;
};

#endif