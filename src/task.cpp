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

}