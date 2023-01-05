/**
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;

void producer()
{
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lk(m);
        q.push(i);
        std::cout << "Produced " << i << '\n';
        cv.notify_one();
    }
}

void consumer()
{
    while (true) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{ return !q.empty(); });
        int val = q.front();
        q.pop();
        std::cout << "Consumed " << val << '\n';
        if (val == 4) {
            break;
        }
    }
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
}

**/