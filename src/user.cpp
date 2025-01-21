#include "../include/user.h"
#include "../include/admin.h"
#include "../include/sqlmanager.h"
#include <thread>
#include <chrono>
#include <iostream>
namespace thu
{
uInt32 User::getId() const
{
    return m_id;
}

std::string User::getName() const
{
    return m_name;
}

void User::receiveTask()
{
    std::cout << m_name << " receiving task!\n";

    // read task from DB
    auto task = getTaskFromDB();
    // do Task
    doTask(task);
}

void User::addAdm(const std::shared_ptr<Admin>& pAdm)
{
    m_pAdmin = pAdm;
}

void User::notify()
{
    m_pAdmin->receiveEvent();
}

Task User::getTaskFromDB()
{
    Task task;
    SQLManager::getInstance().read(task);
    return task;
}

void User::doTask(const Task& task)
{
    std::cout << "User: " << m_name << " is doing task: " << task.getTitle() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "User: " << m_name << " have done task!\n";
    notify();
}

} // namespace thu
