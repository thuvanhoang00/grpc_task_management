#include <iostream>
#include <memory>
#include <string>
#include "../include/grpcpp/grpcpp.h"
#include "greet.grpc.pb.h"
#include "../include/task.h"

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
    thu::Task tsk(1, "Learn", "learn grpc", "today", 1, thu::Status::Inprogress);
    std::cout << tsk.getDescription() << ' ' << tsk.getDuedate() << std::endl;
    RunServer();
    return 0;
}