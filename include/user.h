#ifndef USER_H
#define USER_H
#include <string>
#include <queue>
#include <memory>
#include "../include/define.h"
#include "../include/task.h"
namespace thu
{

class Admin;
class User
{
public:
    User(std::string name, uInt32 id) : m_name(name), m_id(id) {}
    uInt32 getId() const;
    std::string getName() const;
    void receiveTask(const Task& task);
    
    void notify();
    void addAdm(const std::shared_ptr<Admin>& pAdm);

private:
    std::string m_name;
    uInt32 m_id;
    std::queue<Task> m_lstTask;
    std::shared_ptr<Admin> m_pAdmin;
};


} // namespace thu



#endif