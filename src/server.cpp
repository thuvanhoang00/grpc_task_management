#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <future>
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

    thu::Task temp;
    thu::MutexMessageQueue<thu::Task> mutexTaskQueue;
    std::thread threads[10];
    for(int i = 0; i < 10; i++)
    {
        threads[i] = std::thread([&mutexTaskQueue, temp](){mutexTaskQueue.push(temp);});
    }
    for(int i = 0; i < 10; i++)
    {
        threads[i].join();
    }

    thu::AtomicMessageQueue<thu::Task> atomicTaskQueue;
    std::thread atomic_threads[10];
    for(int i = 0; i < 10; i++)
    {
        atomic_threads[i] = std::thread([&atomicTaskQueue, temp](){atomicTaskQueue.push(temp);});
    }
    for(int i = 0; i < 10; i++)
    {
        atomic_threads[i].join();
    }

    RunServer();
    return 0;
}