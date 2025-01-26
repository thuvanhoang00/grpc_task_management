#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <iostream>
namespace thu
{

template<typename T>
class MutexMessageQueue
{
public:
    MutexMessageQueue() : pimpl(std::make_unique<Impl>()) {}
    // ~MutexMessageQueue();
    size_t size() const;
    // size_t size() const
    // {
    //     return pimpl->size();
    // }
    T pop()
    {
        return pimpl->pop();
    }
    void push(const T& element)
    {
        pimpl->push(element);
    }
    bool empty() const
    {
        return pimpl->empty();
    }
private:
    // Pointer to implementation
    class Impl; 
    std::unique_ptr<Impl> pimpl;
};

template<typename T>
class MutexMessageQueue<T>::Impl
{
public:
    Impl() = default;
    size_t size() const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_queue.size();
    }

    void push(const T& element)
    {
        std::lock_guard guard(m_mutex);
        m_queue.push(element);
    }
    
    T pop()
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        if (m_queue.empty())
            throw std::runtime_error("Empty queue\n");
        T ret = m_queue.front();
        m_queue.pop();
        return ret;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_queue.empty();
    }
private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex; // mutable?
};
template<typename T>
size_t MutexMessageQueue<T>::size() const
{
    return pimpl->size();
}

/*
 * Atomic class
*/
template<typename T>
class AtomicMessageQueue
{
public:
    AtomicMessageQueue() : pimpl(std::make_unique<Impl>()) {}
    // ~AtomicMessageQueue();
    size_t size()
    {
        return pimpl->size();
    }
    T pop()
    {
        return pimpl->pop();
    }
    void push(const T& e)
    {
        // std::cout << "Pushing task into AQueue\n";
        pimpl->push(e);
    }
    bool empty()
    {
        return pimpl->empty();
    }
private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

template<typename T>
class AtomicMessageQueue<T>::Impl
{

public:
    Impl() : flag(ATOMIC_FLAG_INIT) {}
    size_t size()
    {
        while (flag.test_and_set()); // spin lock
        auto ret = m_queue.size();
        flag.clear();
        return ret;        
    }

    T pop()
    {
        while(flag.test_and_set());
        if(m_queue.empty()) throw std::runtime_error("AQueue is empty\n");
        auto ret = m_queue.front();
        m_queue.pop();
        flag.clear(); // may be data race here
        return ret;
    }

    void push(const T& e)
    {
        while(flag.test_and_set()); // spin lock
        m_queue.push(e);
        flag.clear();
    }

    bool empty()
    {
        while(flag.test_and_set()); // spin lock
        auto ret = m_queue.empty();
        flag.clear(); // perhaps race here
        return ret;
    }

private:
    std::queue<T> m_queue;
    std::atomic_flag flag;
};
} // namespace thu


#endif