#include <iostream>
#include <string>
#include "../include/sqlmanager.h"

namespace thu
{
void SQLManager::connect()
{
    try
    {
        pqxx::connection conn("dbname=task_db user=task_user password=password123 host=localhost port5432");

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
        }
        conn.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
}