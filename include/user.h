#ifndef USER_H
#define USER_H
#include <string>
#include <queue>
#include <memory>
#include "../include/define.h"
#include "../include/task.h"
namespace thu
{
class User
{
public:
    User(std::string name, uInt32 id) : m_name(name), m_id(id) {}
    uInt32 getId() const;
    std::string getName() const;
    void receiveTask();

private:
    std::string m_name;
    uInt32 m_id;
    std::queue<Task> m_lstTask;
};


} // namespace thu



#endif