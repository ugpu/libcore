/*
	auth: ugpu
	date: 09/11/2022
	description: .....
*/
#pragma once
#ifndef DEFINE_LIB_H
#define DEFINE_LIB_H
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
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define MAX_FILE_NAME_LEN (255)
#define MAX_FULL_PATH_LEN (4096)

#define PRINT_FONT_BLA  printf("\033[30m"); //back
#define PRINT_FONT_RED  printf("\033[31m"); //red
#define PRINT_FONT_GRE  printf("\033[32m"); //green
#define PRINT_FONT_YEL  printf("\033[33m"); //yellow
#define PRINT_FONT_BLU  printf("\033[34m"); //blue
#define PRINT_FONT_PUR  printf("\033[35m"); //pup
#define PRINT_FONT_WHI  printf("\033[37m"); //white

#endif