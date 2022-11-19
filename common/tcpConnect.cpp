#include "tcpConnect.h"

CTcpConnect::CTcpConnect()
{
    m_work = false;
}

CTcpConnect::~CTcpConnect()
{   
    m_socket.close();
    m_work = false;
}


void CTcpConnect::connect(const char* ip, const int port, bool isNoblock /* = true */)
{
    INFO_LOG("connect ip = %s port = %d", ip, port);

    m_socket.init(ip, port);
    if(m_socket.connect(ip, port) != 0)
    {
        ERROR_LOG("client connect server failed!");
        return;
    }
    
    //must connected success set block!!!  when if before connect become syn!
    if(isNoblock)
    {
        m_socket.setNoBlock();
    }

    m_work = true;

    return;
}



void CTcpConnect::disconnect()
{
    m_socket.close();
}

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

//read data
int CTcpConnect::run()
{   
    int _pkgLen = 0;
    //package heade size 4
    char chPkgLen[4] = {0};
    memset(m_cache_buff, 0 ,sizeof(m_cache_buff));

    std::string _test_str = rand_str(4096);
    struct net_msg_buff  tmp; 
    tmp.add_data(_test_str.c_str(), _test_str.length());
    char send_tmp[8192] = {0};
    int pack_len = tmp.pack(send_tmp, 8192);

    while(m_work)
    {   

        send_data(send_tmp, pack_len);
        
        int ret = 0;
        memset(chPkgLen, 0, sizeof(chPkgLen));
        ret = m_socket.recv(chPkgLen, sizeof(chPkgLen));
        _pkgLen = *(int*)chPkgLen;
        
        if (ret > 0)
        {   
            DEBUG_LOG("CTcpConnect recv msg package len = %d", *(int*)chPkgLen);
            int readSize = 0;
            while(true)
            {
                int iResult = m_socket.recv(m_cache_buff + readSize, PACKAGE_MAX_SIZE - readSize);
                if (iResult == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    }
                }
                else if(iResult > 0)
                {
                    readSize += iResult;
                    continue;
                }
                else
                {
                    ERROR_LOG("CTcpConnect recv data error = %d", errno);
                    break;
                }
            }

            if(_pkgLen != readSize)
            {
                WARN_LOG("recv data length not eq!");
            }
            else
            {
                //push data to queue
                if(readSize > 0)
                {
                    m_msg_pack.clear();
                    m_msg_pack.add_data(m_cache_buff, readSize);
                    m_msg_queue.push_back(m_msg_pack);
                    INFO_LOG("m_msg_queue size = %d", m_msg_queue.size());
                }
            }

        }
        usleep(100);
    }
}

int CTcpConnect::read_data(char* pBuff, int buffSize, int  & ret)
{
    if(!pBuff)
    {
        return false;
    }

    if(m_msg_queue.size() <= 0 )
    {
        ret = 0;
        return false;
    }

    struct net_msg_buff info  = m_msg_queue.front();
    ret = info.len;
    if(info.len > buffSize)
    {
        return false;
    }

    memcpy(pBuff, info.buff, info.len);
    m_msg_queue.pop_front();

    return info.len;
}

int CTcpConnect::send_data(const char* pBuff, int buffSize)
{
    int sendSize = m_socket.send(pBuff, buffSize);
    if(sendSize <= 0)
    {
        ERROR_LOG("send data failed! errno = %d", errno);
        return 0;
    }
    return sendSize;
}
