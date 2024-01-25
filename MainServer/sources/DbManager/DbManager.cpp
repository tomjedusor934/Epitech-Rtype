/*
** EPITECH PROJECT, 2023
** RType
** File description:
** DbManager
*/

#include "DbManager.hpp"

rtype::DbManager::DbManager()
{
    this->_connection = mysql_init(nullptr);
    this->_sqlReadable = SqlReadable();

    if (this->_connection == nullptr)
        throw std::runtime_error("mysql_init() failed");
}

rtype::DbManager::~DbManager()
{
    std::cout << "del" << std::endl;
    try {
        this->disconnect();
    } catch (std::exception &e) {
        std::cerr << "already disconnected" << std::endl;
    }
}

void rtype::DbManager::connect(std::string host, std::string user, std::string password, std::string database)
{
    std::cout << "connection" << std::endl;
    if (mysql_real_connect(this->_connection, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 6666, nullptr, 0) == nullptr) {
        this->disconnect();
        throw std::runtime_error("mysql_real_connect() failed");
    }
}

void rtype::DbManager::disconnect()
{
    std::cout << "disconnect" << std::endl;
    mysql_close(this->_connection);
}

std::vector<std::map<std::string, std::string>> rtype::DbManager::query(std::string query, int print)
{
    // je veux split la query en fonction des espaces et recuperer la case apres le from
    std::vector<std::string> splitted_query;
    std::string conversion_for;
    int findIndex = 0;
    std::vector<std::map<std::string, std::string>> result;
    MYSQL_RES *res;

    splitted_query = this->split(query, " ");
    if (splitted_query.size() < 4)
        throw std::runtime_error("Error: the query is not valid");

    if (std::find(splitted_query.begin(), splitted_query.end(), "FROM") == splitted_query.end())
        throw std::runtime_error("Error: the query is not valid");

    findIndex = std::find(splitted_query.begin(), splitted_query.end(), "FROM") - splitted_query.begin();

    conversion_for = splitted_query[findIndex + 1];

    mysql_query(this->_connection, query.c_str());
    res = mysql_store_result(this->_connection);
    result = this->_sqlReadable.convert_sql_result_to_map(res, conversion_for);

    if (print)
        this->dump_result(result);

    mysql_free_result(res);
    return result;
}

std::vector<std::vector<std::string>> rtype::DbManager::customQuery(std::string &query)
{
    std::vector<std::vector<std::string>> queryResult;
    MYSQL_RES *res;

    mysql_query(this->_connection, query.c_str());

    res = mysql_store_result(this->_connection);

    if (res == nullptr) {
        return queryResult;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::vector<std::string> rowResult;
        for (int i = 0; i < mysql_num_fields(res); i++) {
            rowResult.push_back(row[i]);
        }
        queryResult.push_back(rowResult);
    }
    return queryResult;
}

void rtype::DbManager::insert(std::string query)
{
    MYSQL_RES *res;

    mysql_query(this->_connection, query.c_str());

    res = mysql_store_result(this->_connection);

    if (res != nullptr)
        throw std::runtime_error("Error: the query failed");

    mysql_free_result(res);
}

void rtype::DbManager::update(std::string query)
{
    MYSQL_RES *res;

    mysql_query(this->_connection, query.c_str());

    res = mysql_store_result(this->_connection);

    if (res != nullptr)
        throw std::runtime_error("Error: the query failed");

    mysql_free_result(res);
}

void rtype::DbManager::remove(std::string query)
{
    MYSQL_RES *res;

    mysql_query(this->_connection, query.c_str());

    res = mysql_store_result(this->_connection);

    if (res != nullptr)
        throw std::runtime_error("Error: the query failed");

    mysql_free_result(res);
}

void rtype::DbManager::dump_result(std::vector<std::map<std::string, std::string>> result)
{
    int i = 0;
    for (auto info : result) {
        std::cout << "[" << i++ << "]" << std::endl;
        for (auto it = info.begin(); it != info.end(); it++)
            std::cout << "  " << it->first << ": " << it->second << std::endl;
        std::cout << std::endl;
    }
}

std::vector<std::string> rtype::DbManager::split(std::string str, std::string delimiter)
{
    std::vector<std::string> splitted;
    size_t pos = 0;
    std::string token;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        splitted.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    splitted.push_back(str);
    return splitted;
}