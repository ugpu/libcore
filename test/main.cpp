#include "../common/global.h"
#include "../common/define.h"
#include "../common/thread.h"
#include "../common/connectMgr.h"
#include "../common/shareQueue.h"




#define TEST_SVR_PORT  (19001)
//max package size 40K
//only test net transport
//you can use google protobuffer or other proto
//the buff only sample test!
const int max_msg_len = 10 * 4096;
struct net_msg_buff {
    int len;
    char buff[max_msg_len];
    
    net_msg_buff()
    {
        len = 0;
        memset(this, 0, sizeof(net_msg_buff));
    }

    void clear()
    {
        len = 0;
        memset(this, 0, sizeof(net_msg_buff));
    }

    bool add_data(const char* pData, int n)
    {
        if(n + len >= max_msg_len) return false;
        memcpy(buff + len, pData, n);
        len += n;
        return true;
    }

    char* pack()
    {
        len = *(int*)(buff);
        memmove(buff + sizeof(int), buff, len);
        memcpy(buff, (void*)(&len), sizeof(len));
        return buff;
    }

    void unpack(char* data)
    {
        len = *(int*)data;
        memcpy(buff, data + sizeof(int), len);
    }
};

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


class CClient : public CThread
{
public:
    CClient(){};
    ~CClient(){};

    void init()
    {
        m_socket.init("127.0.0.1", 10092);
        if(m_socket.connect("127.0.0.1", 10091) != 0)
        {
            ERROR_LOG("client connect server failed!");
            return;
        }
        m_socket.setNoBlock();
    }

    void make_data(struct net_msg_buff & send_buff)
    {
        for(int i =0; i < 2; ++i)
        {
            std::string r_str = rand_str(4096);
            send_buff.add_data(r_str.c_str(), r_str.length());
        }
    }

    char msg[PACKAGE_MAX_SIZE] = {0};
    virtual int run()
    {   
        struct net_msg_buff recv_buff;
        struct net_msg_buff send_buff;

        DEBUG_LOG("Client start run! ");
        //make test data to server
        memset(msg, 0 , sizeof(msg));
        send_buff.clear();

        make_data(send_buff);
        DEBUG_LOG("send_buff.len = %d", send_buff.len);


        m_socket.send((const char*)(&send_buff.len), sizeof(int));
        m_socket.send(send_buff.buff, send_buff.len);
        DEBUG_LOG("first send pack = %d", send_buff.len);
        while(true)
        {   
            int ret = 0;
            char chPkgLen[sizeof(int)] = {0};
            ret = m_socket.recv(chPkgLen, sizeof(chPkgLen));
            if (ret > 0)
            {
                DEBUG_LOG("ret = %d", ret);
                DEBUG_LOG("client chPkgLen = %d", *(int*)chPkgLen);
                while(true)
                {
                    int iResult = m_socket.recv(recv_buff.buff, PACKAGE_MAX_SIZE);
                    if (iResult == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                    }
                }
                send_buff.clear();
                make_data(send_buff);
                m_socket.send((const char*)(&send_buff.len), sizeof(int));
                m_socket.send(send_buff.buff, send_buff.len);
                DEBUG_LOG("recv data after send = %d", send_buff.len);
            }
            if(ret == 0)
            {
                ERROR_LOG(" client recv err buff!");
                return -1;
            }

            sleep(1);
        }
    }
private:
    CSocket m_socket;
};

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
        INFO_LOG("recv data = %s len = %d", data.buff, data.len);

        //return pack data
        ::send(_fd, (const char*)(&data.len), sizeof(data.len), 0);
        ::send(_fd, (const char*)(data.buff), data.len, 0);
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
    std::string str = rand_str(512);
    
    void* pStatus;
    CTestThread* pThread = new CTestThread();
    pThread->start();
    //pThread->join(0, &pStatus);


    CServer a;
    a.init_svr();
    a.start();

    sleep(1);

    CClient c;
    c.init();
    c.start();
    void *status;
    c.join(c.getTID(), &status);

    INFO_LOG("#########   test end  #####");

    return 0;
}