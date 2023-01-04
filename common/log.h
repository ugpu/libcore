/*
	auth: ugpu
	date: 09/11/2022
	description: log
*/

#ifndef CLOG_H
#define CLOG_H

#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "define.h"




class CLog
{
public:
	//maxSize = 1GB
	CLog(const char* pFileName, const char* pDir,  int loglevel,int logSize = 1024 * 1024 * 1024);
	~CLog();

	static CLog* inputFile();
public:
	int writeFile(const char* fileName, const int fileLine, int logLevel, const char* pFormat, ...);
	int getBuffSize();
protected:
	int genFileHead(struct tm* pTm, const struct timeval& tv, int logLevel, const char* fileName, const int fileLine);
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