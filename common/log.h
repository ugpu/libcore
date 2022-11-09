/*
	auth: ugpu
	date: 09/11/2022
	description: log
*/

#ifndef LOG_LIB_H
#define LOG_LIB_H

#include "define.h"

enum LogLevel {
	error = 1,
	warn  = 2,
	info  = 3,
	debug = 4,
}

static const char* LogLevelStr[] = {
	"",
	"[ERROR]",
	"[WARN]",
	"[INFO]",
	"[DEBUG]",
}

class CLog
{
public:
	//maxSize = 1MB
	CLog(const char* pFileName, const char* pDir,  int loglevel,int logSize = 10240);
	~CLog();

public:
	int write_file()

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
	chat  m_FileFullName[MAX_FULL_PATH_LEN];
};

#endif