/*
	auth: ugpu
	date: 09/11/2022
	description: .....
*/

#ifndef DEFINE_H
#define DEFINE_H


#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_FILE_NAME_LEN (255)
#define MAX_FULL_PATH_LEN (4096)
#define MAX_LOG_FILE_NUMS (100)

#define PACKAGE_MAX_SIZE (100 * 1024 +4)

#define NET_MSG_MAX_BUFF_LEN (10 * 4096)

#define DEFAULT_LISTEN_NUM (1024 * 10)
#define MAX_LISTEN_EVENT_CNT (1024 * 10)
#define PRINT_FONT_BLA  printf("\033[30m"); //back
#define PRINT_FONT_RED  printf("\033[31m"); //red
#define PRINT_FONT_GRE  printf("\033[32m"); //green
#define PRINT_FONT_YEL  printf("\033[33m"); //yellow
#define PRINT_FONT_BLU  printf("\033[34m"); //blue
#define PRINT_FONT_PUR  printf("\033[35m"); //purple
#define PRINT_FONT_WHI  printf("\033[37m"); //white

#define LOG_FILE_NAME ("log.log")
#define LOG_FILE_PATH ("../log")

#define SVR_CONN_MAX_LISTEN_NUM (1024 * 10)
#define SVR_CONN_MAX_MSG_NUM (1024 * 10)

/*
	enum LogLevel {
	error = 1,
	warn,
	info,
	debug,
};
*/
#define LOG_FILE_LEVEL (4)


#endif

