#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include <list>
#include <memory>
#include "../include/define.h"
#include "../include/task.h"
#include "../include/user.h"
namespace thu
{
class User;
class Admin
{
public:
    Admin(const std::string& _name) : m_name(_name) {}
    void addUser(std::shared_ptr<User> pUser);
    void assign(uInt32 id, const Task& task );
    Task createTask(std::string taskname = "", std::string taskDes = "");
    void receiveEvent();
    void notify(const thu::Task& task);
private:
    std::list<std::shared_ptr<User>> m_lstUser;
    std::string m_name;
};


}
#endif