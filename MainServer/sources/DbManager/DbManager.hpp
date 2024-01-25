/*
** EPITECH PROJECT, 2023
** RType
** File description:
** DbManager
*/

#pragma once
    #include "SqlReadable.hpp"
    #include "SqlSendable.hpp"

namespace rtype {
    class DbManager {
        public:
            DbManager();
            ~DbManager();

            void connect(std::string host, std::string user, std::string password, std::string database);
            void disconnect();

            // this query is for the select only and need to be prototyped like this: SELECT ... FROM ... (WHERE ...)
            std::vector<std::map<std::string, std::string>> query(std::string query, int print=0);

            std::vector<std::vector<std::string>> customQuery(std::string &query);
            void insert(std::string query);
            void update(std::string query);
            void remove(std::string query);

            void dump_result(std::vector<std::map<std::string, std::string>> result);

        protected:
        private:
            MYSQL *_connection;
            SqlReadable _sqlReadable;

        private:
            std::vector<std::string> split(std::string str, std::string delimiter);
    };
} // namespace rtype