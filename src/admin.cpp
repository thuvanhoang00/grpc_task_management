#include "../include/admin.h"
#include <iostream>
namespace thu
{

void Admin::addUser(std::shared_ptr<User> pUser)
{
    m_lstUser.push_back(pUser);
}

void Admin::assign(uInt32 id, const Task& task )
{
    for(const auto& user : m_lstUser)
    {
        if(user->getId() == id)
        {
            user->receiveTask(task);
        }
    }
}

Task Admin::createTask(std::string taskname, std::string taskdes)
{
    return thu::Task(1 
        , taskname
        , taskdes
        , "01-18-2025"
        , 1
        , thu::Status::Completed
    );
}

void Admin::notify(const thu::Task& task)
{
    for(const auto& e : m_lstUser)
    {
        e->receiveTask(task);
    }
}

void Admin::receiveEvent()
{
    std::cout << m_name << " received event that user have done tasks\n";
}

}