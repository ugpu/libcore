#include "../common/global.h"
#include "../common/thread.h"
#include "../common/connectMgr.h"
std::string rand_str(const int len)
{
    std::string str;
    int idx;
    for(idx = 0; idx < len; ++idx)
    {
        //ascii rand
        str.push_back('a' + rand()%26);
    }
    return str;
}

class CTestThread : public CThread {
public:
    CTestThread() {};
    ~CTestThread() {};

    virtual int run(){
        while(true)
        {
            for(int i = 0 ; i< 10000; ++i)
            {
                for(int m = 0 ; m < 1000; ++m) float a = ((rand() % 100) + 1) / (rand() % 100 + 1) % (rand() % 100 + 1);
            }
            sleep(0);
        }
    }
};


int main(int argc, char **argv)
{   
    
    INFO_LOG("#########   test start  #####");
    INFO_LOG("");
    std::string str = rand_str(512);
    
    for(int i =0; i < 3; ++i)
    {
        ERROR_LOG(str.c_str());
        INFO_LOG(str.c_str());
        WARN_LOG(str.c_str());
        DEBUG_LOG("####### log print finish ! print count = %d #######", i+1);
    }

    void* pStatus;
    CTestThread* pThread = new CTestThread();
    pThread->start();
    //pThread->join(0, &pStatus);

    ConnectMgr a("127.0.0.1", 10091);
    a.start();
    a.run();

    INFO_LOG("#########   test end  #####");

    return 0;
}