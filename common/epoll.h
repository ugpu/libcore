/*
	auth: ugpu
	date: 11/13/2022
	description: epoll api
*/


 #include "global.h"

#ifndef CEPOLL_H
#define CEPOLL_H


struct epoll_event;

class CEpoll
{
public:
	CEpoll();
	~CEpoll();
	
public:
	int create(int listenCount);

	int listen(struct epoll_event* wait_events, int event_max_cnt, int timeout  = -1);

	int add_new_client(int fd, struct epoll_event& ev);

	
	int removeListenter(int fd);

private:
	CEpoll(const CEpoll&);
	CEpoll& operator =(const CEpoll&);
	
private:
    int m_fd;

};