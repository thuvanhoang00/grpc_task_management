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
#include "../include/requesthandler.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using greet::Greeter;
using greet::HelloReply;
using greet::HelloRequest;
using greet::TaskRequest;
using greet::TaskResponse;

#define ATOMIC_QUEUE
int task_id = 0;

class GreeterServiceImpl final : public Greeter::Service
{
    Status SayHello(ServerContext* context, const HelloRequest* request, HelloReply* reply) override
    {
        std::string prefix = "Hello, ";
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
    Status RequestTask(ServerContext* context, const TaskRequest* request, TaskResponse* response) override
    {
#ifdef ATOMIC_QUEUE
        
        m_handler.receivedRequest();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        thu::Task task(m_handler.respondRequest(), "learn cpp", "learn cpp everyday", "today", 1, thu::Status::Completed);
        auto str = task.taskToString();
        response->set_message(str);
        return Status::OK;
#else
        // need to protect task id from multi request
        task_id++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        thu::Task task(task_id, "learn cpp", "learn cpp everyday", "today", 1, thu::Status::Completed);
        auto str = task.taskToString();
        response->set_message(str);
        return Status::OK;
#endif
    }

private:
    thu::RequestHandler m_handler;
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
    RunServer();
    return 0;
}