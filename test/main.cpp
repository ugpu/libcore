
#pragma once

#include "../common/define.h"
#include "../common/global.h"
#include "../common/log.h"
#include "../common/Thread.h"

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

    INIT_LOG("log.log", "../log", LogLevel::debug);


    std::string str = rand_str(512);
    for(int i =0; i < 1; ++i)
    {
        ERROR(str.c_str());
        INFO(str.c_str());
        WARN(str.c_str());
    }

    CThread* pThread = new CThread();
    pThread->start();

    //DESTROY_LOG
    return 0;
}