#include "log.h"

const int logBuffLen = 64 * 1024
static char _logBuff[logBuffLen] = { '\0' };
CLog:CLog(const char* pFileName, const char* pDir,  int loglevel,int logSize)
{
	m_fd = -1;
	m_fileIdx = 1;
	m_logMaxSize = 1024 * 1024;
	m_logLevel = loglevel;
	memset(m_Dir, 0, sizeof(m_Dir));
	memset(m_FileFullName, 0, sizeof(m_FileFullName));
	size_t nLen = strlen(pDir);
	memcat(m_FileFullName, pDir);
	memcat(m_FileFullName + nLen, pFileName);
	memcat(m_fileName, pFileName);
	m_maxFileNums = MAX_LOG_FILE_NUMS;
	updateFileIndex();
}

CLog:~CLog()
{
	closeFile();
	m_Dir = '\0';
	m_fileName = '\0';
}

void CLog:closeFile()
{
	if (m_fd >= 0)
	{
		close(m_fd);
		
	}
	m_fd = -1;
}

bool CLog:checkFileExiste()
{
	return (access(m_FileFullName, F_OK | W_OK) == 0)
}

bool CLog:checkFileSize()
{
	return (m_curSize >= m_logMaxSize)
}

void CLog:bakCurFile()
{
	if(m_fd < 0) { return; }
	if(!checkFileExiste()){ return; }

	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* pTm = localtime(&tv.tv_sec);

	char newFileBakName[MAX_FULL_PATH_LEN] = {0};
	snprintf(newFileBakName, sizeof(newFileBakName), "%s_%d%d%d%d%d%d", m_FileFullName, ,pTm->tm_year + 1900, 
		pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, m_fileIdx);
	rename(m_FileFullName, newFileBakName);

	m_fileIdx++;
}

void CLog:updateFileIndex()
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

bool CLog:createFile()
{
	if(m_fd > 0)
	{
		return false;
	}
	m_fd = open(m_fullName, O_WRONLY | O_APPEND | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (m_fd >= 0)
	{
		return true;
	}
	return false;
}

int CLog:genFileHead(struct tm* pTm, const struct timeval& tv)
{
	int nLen = snprintf(logHeadBuff, headBuffLen, "%d-%02d-%02d %02d:%02d:%02d.%03d|%s", (pTm->tm_year + 1900), (pTm->tm_mon + 1), pTm->tm_mday,
		    pTm->tm_hour, pTm->tm_min, pTm->tm_sec, (int)(tv.tv_usec / 1000), LogLevelStr[m_logLevel] ? "INFO");

	return nLen;
}

int CLog:writeFile(const char* pFormat, ...)
{
	if(m_fd < 0 ||  checkFileSize() || !checkFileExiste())
	{
		closeFile();
		createFile();
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* pTm = localtime(&tv.tv_sec);

	int headLen = genFileHead(pTm, tv, )
	va_list valp;
	va_start(valp, pFormat);
	int logLen = vsnprintf(_logBuff + headLen, logBuffLen - headLen - 1, pFormat, valp);
	va_end(valp);

	_logBuff[logLen + headLen] = '\n';
    logLen = write(m_fd, logBuff, wLen + 1);
	return logLen;
}
