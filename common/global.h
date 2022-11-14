#ifndef GLOBAL_H
#define GLOBAL_H

#include "log.h"

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



#define DEBUG_LOG(fmt, args...)  CLog::inputFile()->writeFile(LogLevel::debug, fmt, ##args)
#define ERROR_LOG(fmt, args...)  CLog::inputFile()->writeFile(LogLevel::error, fmt, ##args)
#define WARN_LOG(fmt,  args...)  CLog::inputFile()->writeFile(LogLevel::warn, fmt, ##args)
#define INFO_LOG(fmt,  args...)  CLog::inputFile()->writeFile(LogLevel::info, fmt, ##args)

#endif