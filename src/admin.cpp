#include "../include/admin.h"
#include "../include/sqlmanager.h"
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
            user->receiveTask();
        }
    }
}

void Admin::createTask(std::string taskname, std::string taskdes)
{
    Task task(1, taskname, taskdes, "01-18-2025", 1, thu::Status::Completed);
    saveTaskToDB(task);

    uInt32 userID = 1;
    notifyUser(userID);
}

void Admin::notifyUser(uInt32 userID)
{
    for(const auto& e : m_lstUser)
    {
        if(e->getId() == userID)
        {
            e->receiveTask();
        }
    }
}

void Admin::receiveEvent()
{
    std::cout << m_name << " received event that user have done tasks\n";
}

void Admin::saveTaskToDB(const Task& task)
{
    // insert into DB
    SQLManager::getInstance().insert(task);
}

}