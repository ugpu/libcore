#ifndef GLOBAL_H
#define GLOBAL_H

#include "log.h"
#include "define.h"
#include <string>
#include <string.h>
#include <unordered_map>
#include <stdlib.h>
#include <errno.h>
#include <map>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum LogLevel {
	error = 1,
	warn,
	info,
	debug,
};

#ifndef __FILE__
#define __FILE__ ""
#endif

#ifndef __LINE__
#define __LINE__ 0
#endif


#define DEBUG_LOG(fmt, args...)  CLog::inputFile()->writeFile(__FILE__, __LINE__, LogLevel::debug, fmt, ##args)
#define ERROR_LOG(fmt, args...)  CLog::inputFile()->writeFile(__FILE__, __LINE__, LogLevel::error, fmt, ##args)
#define WARN_LOG(fmt,  args...)  CLog::inputFile()->writeFile(__FILE__, __LINE__, LogLevel::warn, fmt, ##args)
#define INFO_LOG(fmt,  args...)  CLog::inputFile()->writeFile(__FILE__, __LINE__, LogLevel::info, fmt, ##args)





struct net_msg_buff {
    int len;
    char buff[PACKAGE_MAX_SIZE];
    
    net_msg_buff()
    {
        len = 0;
        memset(this, 0, sizeof(net_msg_buff));
    }

    void clear()
    {
        len = 0;
        memset(this, 0, sizeof(net_msg_buff));
    }

    bool add_data(const char* pData, int n)
    {
        if(n + len >= PACKAGE_MAX_SIZE) return false;
        memcpy(buff + len, pData, n);
        len += n;
        return true;
    }

    int pack(char* pBuff, int buffLen)
    {
		if(buffLen <= len) return false;
		memcpy(pBuff, (void*)(&len), sizeof(len));
		memcpy(pBuff + sizeof(len), buff, len);
        return (sizeof(len) + len);
    }

    void unpack(char* data)
    {
        len = *(int*)data;
        memcpy(buff, data + sizeof(int), len);
    }
};


#endif