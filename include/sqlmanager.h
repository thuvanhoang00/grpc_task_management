#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <pqxx/pqxx>
#include "../include/task.h"
namespace thu
{

class SQLManager
{
public:
    static SQLManager& getInstance();
    bool connect(pqxx::connection& conn);
    SQLManager(const SQLManager& o) = delete;
    SQLManager& operator=(const SQLManager& o) = delete;

    // not necessary
    // SQLManager(SQLManager&& o) = delete;
    // SQLManager& operator=(SQLManager&&) = delete;

    void insert(const Task& task);
    void read(Task& task);

private:
    SQLManager(){};
};
}
#endif
