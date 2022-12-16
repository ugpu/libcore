#include <cstddef> // for std::size_t
#include <new> // for std::bad_alloc
#include <vector> // for std::vector

template <typename T>
class MemoryPool {
public:
    // 定义类型别名
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    // 分配一块大小为 n 的内存，并返回指向该内存的指针
    static pointer allocate(std::size_t n);

    // 释放指针 p 指向的内存
    static void deallocate(pointer p, std::size_t n);

    // 调用对象的构造函数，将对象放在 p 指向的内存中
    static void construct(pointer p, const_reference val);

    // 调用对象的析构函数，析构 p 指向的对象
    static void destroy(pointer p);

private:
    // 分配内存块的最小大小
    static constexpr std::size_t chunk_size = 1024;

    // 存储内存块的 vector
    static std::vector<char*> chunks;

    // 当前内存块的起始地址和结束地址
    static char* start;
    static char* end;
};

// 初始化内存块 vector 和起始地址和结束地址
template <typename T>
std::vector<char*> MemoryPool<T>::chunks;
