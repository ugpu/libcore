/*
	auth: ugpu
	date: 09/11/2022
	description: log
*/

#ifndef CLOG_H
#define CLOG_H

#include "define.h"

#define MAX_LOG_FILE_NUMS (100)
enum LogLevel {
	error = 1,
	warn,
	info,
	debug,
};

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