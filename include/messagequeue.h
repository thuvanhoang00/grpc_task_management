#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
namespace thu
{

template<typename T>
class MutexMessageQueue
{
public:
    MutexMessageQueue();
    ~MutexMessageQueue();
    size_t size() const ;
    T pop();
    void push(const T& e);
    bool empty() const ;
private:
    // Pointer to implementation
    class Impl; 
    Impl* pimpl;
};

template<typename T>
class AtomicMessageQueue
{
public:
    AtomicMessageQueue();
    ~AtomicMessageQueue();
    size_t size();
    T pop();
    void push(const T& e);
    bool empty();
private:
    class Impl;
    Impl* pimpl;
};

} // namespace thu


#endif