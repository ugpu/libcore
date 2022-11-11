/*
	auth: ugpu
	date: 11/11/2022
	description: thread api
*/

#ifndef CTHREAD_H
#define CTHREAD_H

#include "define.h"

class CThread
{

public:
	CThread();
	~CThread();

public:
	// exit if success
	int cancel(pthread_t pid);

    //wait thread exit
    int join(pthread_t pid, void** ret_status);

    //detach thread
	int detach(pthread_t pid);

    //call start.... thread start work!
    int start();

    //Override the fun in derive of class
    virtual int run();
    
    //get thread_id
    pthread_t getId();
private:
    // disable copy and opearator
    CThread(const CThread&);
    CThread& operator =(const CThread&);

private:
    static void* call_func_run(void* pObj);

private:
    pthread_t    m_pId; //thrad id
    
};


#endif //CTHREAD_H