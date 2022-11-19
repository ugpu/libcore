#include "../common/global.h"
#include "../common/define.h"
#include "../common/thread.h"
#include "../common/connectMgr.h"
#include "../common/shareQueue.h"
#include "../common/tcpConnect.h"




#define TEST_SVR_PORT  (19001)
//max package size 40K
//only test net transport
//you can use google protobuffer or other proto
//the buff only sample test!





class CServer :  public CThread, public ConnectMgr
{
public:
    CServer() {};
    ~CServer() {};

    void init_svr()
    {
        set_ip("127.0.0.1");
        set_port(10091);
    }

    void server_run()
    {
        INFO_LOG("server start running!");
        init_con();
        work();
        INFO_LOG("server exit!");
    }

    virtual int run()
    {
        server_run();
    }

    virtual int recv_data(int _fd, char* pData, int len)
    {
        struct net_msg_buff data;
        data.unpack(pData);
        //INFO_LOG("recv data = %s len = %d", data.buff, data.len);
        char bf[8192] = {0};
        int msg_len = data.pack(bf, 8192);
        ::send(_fd, bf, msg_len, 0);
    }

};

class CClient : public CTcpConnect
{
public:
    CClient() {};
    ~CClient() {};

    void init()
    {
        connect("127.0.0.1", 10091);
    }

};



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
    
    CSharedQueue<char*> _queue;
    INFO_LOG("#########   test start  #####");
    INFO_LOG("");
    
    void* pStatus;
    CTestThread* pThread = new CTestThread();
    pThread->start();
    //pThread->join(0, &pStatus);

    CServer a;
    a.init_svr();
    a.start();

    sleep(3);

    //gen 4096 connect
    for(int i = 0 ; i < 4096; i++)
    {
        CClient*  c = new CClient();
        c->init();
        c->start();
    }

    a.join(0, &pStatus);
    INFO_LOG("#########   test end  #####");

    return 0;
}