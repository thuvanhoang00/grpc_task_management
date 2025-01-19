#include "../include/admin.h"

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
            user->receiveTask();
        }
    }
}

Task Admin::createTask()
{
    return thu::Task(1 
        , "Insert"
        , "Do insert Task into PostgreSQL"
        , "01-18-2025"
        , 1
        , thu::Status::Completed
    );
}

void Admin::notify()
{

}

}