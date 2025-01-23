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
    thu::Task t;
    std::future<void> insertFut = std::async(std::launch::async, &thu::SQLManager::insert, &thu::SQLManager::getInstance(), thu::Task());
    insertFut.get();

    std::future<void> readFut = std::async(std::launch::async, &thu::SQLManager::read, &thu::SQLManager::getInstance(), std::ref(t));
    readFut.get(); 
    // --> able to do
    // -> so need mutex



    RunServer();
    return 0;
}