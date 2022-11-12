#include "../common/global.h"
#include "../common/thread.h"

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


int main(int argc, char **argv)
{   
    
    INFO_LOG("#########   test start  #####");
    INFO_LOG("");
    INFO_LOG("");
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
    CThread* pThread = new CThread();
    pThread->start();
    pThread->join(0, &pStatus);

    INFO_LOG("");
    INFO_LOG("");
    INFO_LOG("");
    INFO_LOG("#########   test end  #####");
    return 0;
}