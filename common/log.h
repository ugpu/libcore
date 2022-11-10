/*
	auth: ugpu
	date: 09/11/2022
	description: log
*/
#pragma once
#ifndef LOG_LIB_H
#define LOG_LIB_H

#include "define.h"


#define MAX_LOG_FILE_NUMS (100)
enum LogLevel {
	error = 1,
	warn,
	info,
	debug,
};

static const char* LogLevelStr[] = {
	"",
	"[ERROR]",
	"[WARN]",
	"[INFO]",
	"[DEBUG]",
};


#define INIT_LOG(log_name, log_path, log_level)  \
CLog* pLog = new CLog(log_name, log_path, log_level);


#define DESTROY_LOG \
delete pLog;

#define DEBUG(fmt, args...) \
PRINT_FONT_WHI \
pLog->writeFile(LogLevel::debug, fmt, ## args);

#define INFO(fmt, args...) \
PRINT_FONT_WHI \
pLog->writeFile(LogLevel::info, fmt, ## args);

#define WARN(fmt, args...) \
PRINT_FONT_YEL \
pLog->writeFile(LogLevel::warn, fmt, ## args);

#define ERROR(fmt, args...) \
PRINT_FONT_RED \
pLog->writeFile(LogLevel::error, fmt, ## args);

#define GETLOGSIZE  pLog->getBuffSize()


class CLog
{
public:
	//maxSize = 1GB
	CLog(const char* pFileName, const char* pDir,  int loglevel,int logSize = 1024 * 1024 * 1024);
	~CLog();

public:
	int writeFile(int logLevel, const char* pFormat, ...);
	int getBuffSize();
protected:
	int genFileHead(struct tm* pTm, const struct timeval& tv, int logLevel);
	bool createFile();
	bool checkFileSize();
	bool checkFileExiste();
	void updateFileIndex();
	void bakCurFile();
	void closeFile();
	void createDir();
private:
	CLog();
    CLog(const CLog&);
	CLog& operator =(const CLog&);


private:
	int   m_maxFileNums;
	char  m_fileName[MAX_FILE_NAME_LEN];
	char  m_Dir[MAX_FULL_PATH_LEN];
	int   m_logMaxSize; 
	int   m_fd;
	int   m_logLevel;
	int   m_curSize;
	int   m_fileIdx;
	char  m_FileFullName[MAX_FULL_PATH_LEN];
};

#endif