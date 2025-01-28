#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "greet.grpc.pb.h"
#include "../include/task.h"
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using greet::Greeter;
using greet::HelloReply;
using greet::HelloRequest;
using greet::TaskRequest;
using greet::TaskResponse;
class GreeterClient
{
public:
    GreeterClient(std::shared_ptr<Channel> channel) : m_stub(Greeter::NewStub(channel)) {}
    std::string SayHello(const std::string& user)
    {
        HelloRequest request;
        request.set_name(user);

        HelloReply reply;
        ClientContext context;

        Status status = m_stub->SayHello(&context, request, &reply);

        if(status.ok())
        {
            return reply.message();
        }
        else
        {
            std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
            return "gRPC call failed";
        }
    }

    std::string RequestTask(const std::string& id)
    {
        TaskRequest request;
        request.set_client_id(id);

        TaskResponse response;
        ClientContext context;

        Status status = m_stub->RequestTask(&context, request, &response);

        if(status.ok())
        {
            return response.message();
        }
        else
        {
            std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
            return "gRPC call failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> m_stub;
};

int main(int argc, char** argv)
{
    std::string target_address = "localhost:50051";
    GreeterClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
    std::string id;
    if(argc>1) id = std::string(argv[1]);    
    auto sendRequest = [&](){
        for(int i=0; i < 100000; i++)
        {
            std::cout << "Client received: " << client.RequestTask(id) << std::endl;
        }
    };

    std::thread t1(sendRequest);
    std::thread t2(sendRequest);
    std::thread t3(sendRequest);
    std::thread t4(sendRequest);
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}