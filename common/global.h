#ifndef GLOBAL_H
#define GLOBAL_H

#include "define.h"
#include "log.h"

extern CLog* pGlobalLog = NULL;

#define INIT_LOG(log_name, log_path, log_level)  \
pGlobalLog = new CLog(log_name, log_path, log_level);

#ifndef DESTROY_LOG
#define DESTROY_LOG() \
delete pGlobalLog;
#endif

#ifndef DEBUG
#define DEBUG(fmt, args...) \
PRINT_FONT_WHI \
if(pGlobalLog) \
pGlobalLog->writeFile(LogLevel::debug, fmt, ## args);
#endif

#ifndef INFO
#define INFO(fmt, args...) \
PRINT_FONT_WHI \
if(pGlobalLog) \
pGlobalLog->writeFile(LogLevel::info, fmt, ## args);
#endif

#ifndef WARN
#define WARN(fmt, args...) \
PRINT_FONT_YEL \
if(pGlobalLog) \
pGlobalLog->writeFile(LogLevel::warn, fmt, ## args);
#endif

#ifndef ERROR
#define ERROR(fmt, args...) \
PRINT_FONT_RED \
if(pGlobalLog) \
pGlobalLog->writeFile(LogLevel::error, fmt, ## args);
#endif

#ifndef GETLOGSIZE
#define GETLOGSIZE \
if(pGlobalLog) \
pGlobalLog->getBuffSize()
#endif

#endif