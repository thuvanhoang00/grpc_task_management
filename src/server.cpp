#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <future>
#include <vector>
#include "../include/grpcpp/grpcpp.h"
#include "greet.grpc.pb.h"
#include "../include/task.h"
#include "../include/sqlmanager.h"
#include "../include/admin.h"
#include "../include/user.h"
#include "../include/messagequeue.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using greet::Greeter;
using greet::HelloReply;
using greet::HelloRequest;

class GreeterServiceImpl final : public Greeter::Service
{
    Status SayHello(ServerContext* context, const HelloRequest* request, HelloReply* reply) override
    {
        std::string prefix = "Hello, ";
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    // std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

#define MAX_ELEMENTS 10000000

void do_push(thu::MutexMessageQueue<thu::Task>& msqueue)
{
    for(int i=0; i<MAX_ELEMENTS; i++)
    {
        msqueue.push(thu::Task());
    }        
}

void do_pop(thu::MutexMessageQueue<thu::Task>& msqueue)
{
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        msqueue.pop();
    }
}

// thread cannot resolve overloading function->need another name
void do_Apush(thu::AtomicMessageQueue<thu::Task>& msqueue)
{
    for(int i=0; i<MAX_ELEMENTS; i++)
    {
        msqueue.push(thu::Task());
    }        
}

void do_Apop(thu::AtomicMessageQueue<thu::Task>& msqueue)
{
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        msqueue.pop();
    }
}

int main(int argc, char** argv)
{
    std::shared_ptr<thu::Admin> adm = std::make_shared<thu::Admin>("A");
    std::shared_ptr<thu::User> usr = std::make_shared<thu::User>("B", 1);
    
    // adm->createTask("LearnCPP", "LearnCPP Today");

    // // admin let user know they have been received task
    // adm->addUser(usr);
    // adm->notifyUser(usr->getId());
    // // when user done task, notify to admin
    // usr->addAdm(adm);
    // usr->notify();

    // Test conn on multithreading
#if 0
    thu::Task t;
    std::future<void> insertFut = std::async(std::launch::async, &thu::SQLManager::insert, &thu::SQLManager::getInstance(), thu::Task());
    insertFut.get();

    std::future<void> readFut = std::async(std::launch::async, &thu::SQLManager::read, &thu::SQLManager::getInstance(), std::ref(t));
    readFut.get(); 
    // --> able to do
    // -> so need mutex
#endif
    thu::MutexMessageQueue<thu::Task> mutexTaskQueue;
    thu::AtomicMessageQueue<thu::Task> atomicTaskQueue;
#if 1
    do_push(mutexTaskQueue);

    std::thread t1(do_push, std::ref(mutexTaskQueue));
    std::thread t2(do_pop, std::ref(mutexTaskQueue));
    std::thread t3(do_push, std::ref(mutexTaskQueue));
    std::thread t4(do_pop, std::ref(mutexTaskQueue));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout << "MUTEX_QUEUE SIZE: " << mutexTaskQueue.size() << std::endl;
#endif
    do_Apush(atomicTaskQueue);

    std::thread at1(do_Apush, std::ref(atomicTaskQueue));
    std::thread at2(do_Apop, std::ref(atomicTaskQueue));
    std::thread at3(do_Apush, std::ref(atomicTaskQueue));
    std::thread at4(do_Apop, std::ref(atomicTaskQueue));
    at1.join();
    at2.join();
    at3.join();
    at4.join();

    std::cout << "ATOMIC_QUEUE SIZE: " << atomicTaskQueue.size() << std::endl;

    // RunServer();
    return 0;
}