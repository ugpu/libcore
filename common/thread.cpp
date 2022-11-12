#include "thread.h"
#include "global.h"


CThread::CThread() : m_pId(0)
{

}

CThread::~CThread()
{
    m_pId = 0;
}


int CThread::detach(pthread_t pid)
{
	int ret = pthread_detach(pid);
    if(ret != 0)
    {
        //dosomething... throw..error?
        ERROR_LOG("cancel thread failed! pid = %d", pid);
    }
	return ret;
}

int CThread::join(pthread_t pid, void** ret_status)
{
	int ret = pthread_join(m_pId, ret_status);
	if (ret != 0)
	{
		//dosomething... throw..error?
        ERROR_LOG("join thread failed! pid = %d", m_pId);
	}
	return ret;
}

int CThread::cancel(pthread_t pid)
{
	int ret = pthread_cancel(m_pId);
	if (ret != 0)
	{
		//dosomething... throw..error?
        ERROR_LOG("cancel thread failed! pid = %d", pid);
	}
	return ret;
}

void* CThread::call_func_run(void* pObj)
{
    if(!pObj)
    {
        //throw error!
        ERROR_LOG("start_fun failed! not found pObj");
        return NULL;
    }
    CThread* pThread = (CThread*)pObj;
    pThread->run();
    return pThread;
}

int CThread::start()
{	
	DEBUG_LOG("thread start!!!!!");
	int ret = pthread_create(&m_pId, NULL, call_func_run, (void*)this);
	if (ret != 0)
	{
		ERROR_LOG("start failed! pid = %d", m_pId);
	}
	return ret;
}

int CThread::run()
{
    INFO_LOG(" thread_id =%d start run!", m_pId);
    return 0;
}