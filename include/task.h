#ifndef TASK_H
#define TASK_H
#include <string>

namespace thu
{

enum class Status
{
    Pending,
    Inprogress,
    Completed,
    None
};

class Task
{
public:
    Task() {}
    Task(int32_t id, std::string title, std::string description, std::string duedate, int32_t priority, Status status)
        : m_id(id), m_title(title), m_description(description), m_duedate(duedate), m_priority(priority), m_status(status)
    {}
    
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setDuedate(const std::string& duedate);
    int32_t getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDuedate() const;
private:
    int32_t m_id;
    std::string m_title;
    std::string m_description;
    std::string m_duedate;
    int32_t m_priority;
    Status m_status;
};
}
#endif
