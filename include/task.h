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
    Task(int32_t id, std::string title, std::string description, std::string duedate, int32_t priority, Status status)
        : m_id(id), m_title(title), m_description(description), m_duedate(duedate), m_priority(priority), m_status(status)
    {}
    
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