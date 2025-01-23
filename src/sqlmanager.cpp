#include <iostream>
#include <string>
#include <thread>
#include "../include/sqlmanager.h"

namespace thu
{

SQLManager& SQLManager::getInstance()
{
    static SQLManager instance;
    return instance;
}

bool SQLManager::connect(pqxx::connection& conn)
{
    std::cout << __FUNCTION__ << std::endl;
    try
    {
        pqxx::connection conn("dbname=task_db user=task_user password=password123 host=localhost port=5432");
        if(conn.is_open())
        {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;

            std::string create_table_query=
                "CREATE TABLE IF NOT EXISTS tasks("
                "id SERIAL PRIMARY KEY, "
                "title TEXT NOT NULL, "
                "description TEXT, "
                "due_date TIMESTAMP, "
                "status TEXT NOT NULL DEFAULT 'Pending'"
                ");";

            pqxx::work txn(conn);
            txn.exec(create_table_query);
            txn.commit();
            std::cout << "Table created successfully!\n";

            pqxx::work insert_txn(conn);
            insert_txn.exec("INSERT INTO tasks (title, description, due_date) VALUES "
                            "('Task 1', 'First Task Description', '2025-01-20');");
            insert_txn.commit();
            std::cout << "Data inserted successfully!\n";
        }
        else
        {
            std::cerr << "Failed to connect to database!\n";
            return false;
        }
        conn.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

void SQLManager::insert(const Task &task)
{
    std::cout << __FUNCTION__ << std::endl;
    try
    {
        pqxx::connection conn("dbname=task_db user=task_user password=password123 host=localhost port=5432");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (conn.is_open())
        {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;

            std::string create_table_query=
                "CREATE TABLE IF NOT EXISTS tasks("
                "id SERIAL PRIMARY KEY, "
                "title TEXT NOT NULL, "
                "description TEXT, "
                "due_date TIMESTAMP, "
                "status TEXT NOT NULL DEFAULT 'Pending'"
                ");";

            pqxx::work txn(conn);
            txn.exec(create_table_query);
            txn.commit();
            std::cout << "Table created successfully!\n";
            pqxx::work insert(conn);

            std::string sqlcommand = "INSERT INTO tasks (title, description, due_date) VALUES ('" +
                                     task.getTitle() + "', '" + task.getDescription() + "', '2025-01-20');";
            insert.exec(sqlcommand);
            insert.commit();
            std::cout << "Inserted successfully!\n";
        }
        else
        {
            std::cerr << "Failed to connect to database!\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void SQLManager::read(Task& task)
{
    try
    {
        pqxx::connection conn("dbname=task_db user=task_user password=password123 host=localhost port=5432");
        if (conn.is_open())
        {
            // VERIFY
            // Create a non-transactional query
            pqxx::nontransaction verify_txn(conn);

            // Query the data
            pqxx::result res = verify_txn.exec("SELECT * FROM tasks;");

            // Display the results
            for (const auto &row : res)
            {
                // std::cout << "ID: " << row["id"].c_str()
                //           << ", Title: " << row["title"].c_str()
                //           << ", Description: " << row["description"].c_str()
                //           << ", Due Date: " << row["due_date"].c_str()
                //           << ", Status: " << row["status"].c_str()
                //           << std::endl;
                if(std::string(row["title"].c_str()).compare("LearnCPP") == 0)
                {
                    Task tsk(1, std::string(row["title"].c_str()), std::string(row["description"].c_str()), std::string(row["due_date"].c_str()), 1, Status::Completed);
                    task = tsk;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(4));
            std::cout << "Read successfully!\n";
        }
        else
        {
            std::cerr << "Failed to connect to database!\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
}