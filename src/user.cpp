#include "../include/user.h"
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

void User::receiveTask(const Task& task)
{
    std::cout << m_name << " receiving task: " << task.getTitle() << std::endl;
    m_lstTask.push(task);
}

} // namespace thu
