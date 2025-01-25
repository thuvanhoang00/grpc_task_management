#include "../include/messagequeue.h"
#include "../include/task.h"
#include <stdexcept>
#include <iostream>
namespace thu
{

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
MutexMessageQueue<T>::MutexMessageQueue() : pimpl(new MutexMessageQueue<T>::Impl())
{
}

template<typename T>
MutexMessageQueue<T>::~MutexMessageQueue()
{
    if(pimpl != nullptr) delete pimpl;
}

template<typename T>
size_t MutexMessageQueue<T>::size() const
{
    return pimpl->size();
}

template<typename T>
void MutexMessageQueue<T>::push(const T& element)
{
    std::cout << "Pushing task into queue \n";
    pimpl->push(element);
}

template<typename T>
T MutexMessageQueue<T>::pop()
{
    return pimpl->pop();
}

template<typename T>
bool MutexMessageQueue<T>::empty() const
{
    return pimpl->empty();
}


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

template<typename T>
AtomicMessageQueue<T>::AtomicMessageQueue() : pimpl(new AtomicMessageQueue<T>::Impl()) 
{

}

template<typename T>
AtomicMessageQueue<T>::~AtomicMessageQueue()
{
    if(pimpl != nullptr) delete pimpl;
}

template<typename T>
size_t AtomicMessageQueue<T>::size()
{
    return pimpl->size();
}

template<typename T>
T AtomicMessageQueue<T>::pop()
{
    return pimpl->pop();
}

template<typename T>
void AtomicMessageQueue<T>::push(const T& e)
{
    std::cout << "Pushing task into AQueue\n";
    pimpl->push(e);
}

template<typename T>
bool AtomicMessageQueue<T>::empty()
{
    return pimpl->empty();
}
template class thu::MutexMessageQueue<thu::Task>;
template class thu::AtomicMessageQueue<thu::Task>;
}