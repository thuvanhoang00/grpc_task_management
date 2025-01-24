#include "../include/messagequeue.h"
#include <stdexcept>

namespace thu
{

template<typename T>
size_t MutexMessageQueue<T>::size()
{
    std::lock_guard guard(m_mutex);
    return m_queue.size();
}

template<typename T>
T MutexMessageQueue<T>::pop()
{
    std::lock_guard guard(m_mutex);
    if(m_queue.empty()) throw std::runtime_error("Empty queue\n");
    T ret = m_queue.front();
    m_queue.pop();
    return ret;
}

template<typename T>
void MutexMessageQueue<T>::push(const T& e)
{
    std::lock_guard guard(m_mutex);
    m_queue.push(e);
}

template<typename T>
bool MutexMessageQueue<T>::empty()
{
    std::lock_guard guard(m_mutex);
    return m_queue.empty();
}

template<typename T>
size_t AtomicMessageQueue<T>::size()
{
    while(flag.test_and_set()); // spin lock
    auto ret = m_queue.size();
    flag.clear(); // unlock
    return ret;
}

template<typename T>
T AtomicMessageQueue<T>::pop()
{
    while(flag.test_and_set()); // spin lock

    T ret = m_queue.front();
    m_queue.pop();

    flag.clear(); // unlock

    return ret;
}

template<typename T>
void AtomicMessageQueue<T>::push(const T& e)
{
    while(flag.test_and_set()); // spin lock
    m_queue.push(e);
    flag.clear(); // unlock
}

template<typename T>
bool AtomicMessageQueue<T>::empty()
{
    while(flag.test_and_set()); // spin lock
    auto ret = m_queue.empty();
    flag.clear(); // unlock
    return ret;
}

}