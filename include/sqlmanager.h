#include <pqxx/pqxx>
#include "../include/task.h"
namespace thu
{
#ifndef SQLMANAGER_H
#define SQLMANAGER_H
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


private:
    SQLManager(){};
};
#endif
}