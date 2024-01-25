cmake_minimum_required(VERSION 3.15)

enable_language(CXX)

project(client VERSION 1.0 LANGUAGES CXX)

add_subdirectory(Client)
add_subdirectory(GameServer)
add_subdirectory(MainServer)