#include "../include/task.h"
namespace thu
{
std::string Task::getTitle() const
{
    return m_title;
}

std::string Task::getDescription() const
{
    return m_description;
}

std::string Task::getDuedate() const
{
    return m_duedate;
}

void Task::setTitle(const std::string& title)
{
    m_title = title;
}

void Task::setDescription(const std::string& des)
{
    m_description = des;
}

void Task::setDuedate(const std::string& date)
{
    m_duedate = date;
}

}

