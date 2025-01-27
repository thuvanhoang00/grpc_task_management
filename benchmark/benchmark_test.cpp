#include <benchmark/benchmark.h>
// #include "../include/admin.h"
// #include "../include/user.h"
#include "../include/messagequeue.h"
#include "../include/task.h"
#include <thread>
#define MAX_LOOP 1000000

void do_push(thu::MutexMessageQueue<thu::Task>& mutex_queue)
{
    for(int i=0; i<MAX_LOOP; i++)
    {
        mutex_queue.push(thu::Task());
    }
}

void do_pop(thu::MutexMessageQueue<thu::Task>& mutex_queue)
{
    for(int i=0; i<MAX_LOOP; i++)
    {
        mutex_queue.pop();
    }
}

void do_Apush(thu::AtomicMessageQueue<thu::Task>& atomic_queue)
{
    for(int i=0; i<MAX_LOOP; i++)
    {
        atomic_queue.push(thu::Task());
    }
}

void do_Apop(thu::AtomicMessageQueue<thu::Task>& atomic_queue)
{
    for(int i=0; i<MAX_LOOP; i++)
    {
        atomic_queue.pop();
    }
}

static void benchmark_mutex_queue(benchmark::State& s)
{
    thu::MutexMessageQueue<thu::Task> mutex_queue;
    do_push(mutex_queue);
    for(auto state : s)
    {
        std::thread t1(do_push, std::ref(mutex_queue));
        std::thread t2(do_pop, std::ref(mutex_queue));
        std::thread t3(do_push, std::ref(mutex_queue));
        std::thread t4(do_pop, std::ref(mutex_queue));
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    // std::cout << "MUTEX QUEUE SIZE: " << mutex_queue.size() << std::endl;
}

static void benchmark_atomic_queue(benchmark::State& s)
{
    thu::AtomicMessageQueue<thu::Task> atomic_queue;
    do_Apush(atomic_queue);
    for(auto state : s)
    {
        std::thread t1(do_Apush, std::ref(atomic_queue));
        std::thread t2(do_Apop, std::ref(atomic_queue));
        std::thread t3(do_Apush, std::ref(atomic_queue));
        std::thread t4(do_Apop, std::ref(atomic_queue));
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    // std::cout << "ATOMIC QUEUE SIZE: " << atomic_queue.size() << std::endl;
}
BENCHMARK(benchmark_mutex_queue);
BENCHMARK(benchmark_atomic_queue);
BENCHMARK_MAIN();