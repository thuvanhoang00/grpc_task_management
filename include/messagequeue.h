#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <queue>
#include <mutex>
#include <atomic>
namespace thu
{

template<typename T>
class MutexMessageQueue
{
public:
    MutexMessageQueue(){}
    size_t size();
    T pop();
    void push(const T& e);
    bool empty();
private:
    std::queue<T>   m_queue;
    std::mutex      m_mutex;;
};

template<typename T>
class AtomicMessageQueue
{
public:
    AtomicMessageQueue() : flag(ATOMIC_FLAG_INIT) {}
    size_t size();
    T pop();
    void push(const T& e);
    bool empty();
private:
    std::queue<T> m_queue;
    std::atomic_flag flag;
};

} // namespace thu


#endif