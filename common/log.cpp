#include "log.h"
#include "define.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

static const char* LogLevelStr[] = {
	"",
	"[ERROR]",
	"[WARN]",
	"[INFO]",
	"[DEBUG]",
};




const int logBuffLen = 64 * 1024;
static char _logBuff[logBuffLen] = { '\0' };
CLog::CLog(const char* pFileName, const char* pDir,  int loglevel,int logSize /* = 10240*/ )
{
	m_fd = -1;
	m_fileIdx = 1;
	m_logMaxSize = logSize;
	m_logLevel = loglevel;
	memset(m_Dir, 0, sizeof(m_Dir));
	memset(m_FileFullName, 0, sizeof(m_FileFullName));
	size_t nLen = strlen(pDir);
	strcat(m_FileFullName, pDir);
	createDir();
	strcat(m_FileFullName, "/");
	strcat(m_FileFullName + nLen, pFileName);
	strcat(m_fileName, pFileName);
	m_maxFileNums = MAX_LOG_FILE_NUMS;
	updateFileIndex();
}

CLog::~CLog()
{
	closeFile();
	PRINT_FONT_WHI
	printf("");
}

void CLog::createDir()
{
	int ret = access(m_FileFullName, F_OK | W_OK);
	if (ret != 0)
	{
		mkdir(m_FileFullName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
}

void CLog::closeFile()
{
	if (m_fd >= 0)
	{
		close(m_fd);
		
	}
	m_fd = -1;
}

bool CLog::checkFileExiste()
{
	return (access(m_FileFullName, F_OK | W_OK) == 0);
}

bool CLog::checkFileSize()
{
	return (m_curSize >= m_logMaxSize);
}

void CLog::bakCurFile()
{
	if(!checkFileExiste()) return; 

	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* pTm = localtime(&tv.tv_sec);

	char newFileBakName[MAX_FULL_PATH_LEN] = {0};
	snprintf(newFileBakName, sizeof(newFileBakName), "%s_%d%d%d_%d", m_FileFullName ,pTm->tm_year + 1900, 
		pTm->tm_mon + 1, pTm->tm_mday, m_fileIdx);
	rename(m_FileFullName, newFileBakName);

	m_fileIdx++;
}

void CLog::updateFileIndex()
{	
	char fullBakName[MAX_FULL_PATH_LEN]  = {0};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* pTm = localtime(&tv.tv_sec);

	for(int i = 1 ; i < m_maxFileNums; ++i)
	{	
		m_fileIdx = i;
		snprintf(fullBakName, sizeof(fullBakName), "%s_%d%d%d%d%d%d", m_FileFullName, i);
		if((access(fullBakName, F_OK | W_OK) != 0))
		{
			break;
		}
	}
}

bool CLog::createFile()
{
	if(m_fd > 0)
	{
		return false;
	}
	m_fd = open(m_FileFullName, O_WRONLY | O_APPEND | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (m_fd >= 0)
	{	
		struct stat fileInfo;
		m_curSize = 0;
		if (stat(m_FileFullName, &fileInfo) == 0)
		{
			m_curSize = fileInfo.st_size;
		}
		return true;
	}
	return false;
}

int CLog::genFileHead(struct tm* pTm, const struct timeval& tv, int logLevel)
{
	int nLen = snprintf(_logBuff, logBuffLen, "%d-%02d-%02d %02d:%02d:%02d.%03d %s: ", (pTm->tm_year + 1900), (pTm->tm_mon + 1), pTm->tm_mday,
		    pTm->tm_hour, pTm->tm_min, pTm->tm_sec, (int)(tv.tv_usec / 1000), LogLevelStr[logLevel] ? LogLevelStr[logLevel] : "INFO");

	return nLen;
}

int CLog::getBuffSize()
{
	return m_curSize;
}

CLog* CLog::inputFile()
{
	static CLog g_log(LOG_FILE_NAME, LOG_FILE_PATH, LOG_FILE_LEVEL);
	return &g_log;
}

int CLog::writeFile(int logLevel, const char* pFormat, ...)
{
	if(logLevel > m_logLevel)
	{
		return 0;
	}
	if(m_fd < 0 ||  checkFileSize() || !checkFileExiste())
	{
		closeFile();
		bakCurFile();
		createFile();
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* pTm = localtime(&tv.tv_sec);

	int headLen = genFileHead(pTm, tv, logLevel);
	va_list valp;
	va_start(valp, pFormat);
	int logLen = vsnprintf(_logBuff + headLen, logBuffLen - headLen - 1, pFormat, valp);
	va_end(valp);

	logLen += headLen;
	_logBuff[logLen] = '\n';
    logLen = write(m_fd, _logBuff, logLen + 1);
	printf("%s", _logBuff);
	m_curSize += logLen;
	return logLen;
}
