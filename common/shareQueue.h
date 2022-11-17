/*
	auth: ugpu
	date: 17/11/2022
	description: share queue  & thread safe & ref stackoverflow
*/


#ifndef CSHAREQUEUE_H
#define CSHAREQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class CSharedQueue
{
    public:
        CSharedQueue();
        ~CSharedQueue();

        T& front();
        void pop_front();

        void push_back(const T& item);
        void push_back(T&& item);

        int size();
        bool empty();

    private:
        std::deque<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable cond_;
}; 

template <typename T>
CSharedQueue<T>::CSharedQueue()
{

}

template <typename T>
CSharedQueue<T>::~CSharedQueue()
{

}

template <typename T>
T& CSharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
        cond_.wait(mlock);
    }
    return m_queue.front();
}

template <typename T>
void CSharedQueue<T>::pop_front()
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
        cond_.wait(mlock);
    }
    m_queue.pop_front();
}     

template <typename T>
void CSharedQueue<T>::push_back(const T& item)
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push_back(item);
    mlock.unlock();
    cond_.notify_one();
}

template <typename T>
void CSharedQueue<T>::push_back(T&& item)
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push_back(std::move(item));
    mlock.unlock();
    cond_.notify_one();
}

template <typename T>
int CSharedQueue<T>::size()
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    int size = m_queue.size();
    mlock.unlock();
    return size;
}


#endif
