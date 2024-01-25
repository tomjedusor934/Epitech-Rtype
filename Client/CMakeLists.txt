cmake_minimum_required(VERSION 3.15)

enable_language(CXX)

project(client VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DWITH_MYSQL)
find_package(asio REQUIRED)
find_package(raylib REQUIRED)
find_package(cpr REQUIRED)
find_package(libmysqlclient REQUIRED)
find_package(SFML REQUIRED COMPONENTS graphics system)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}includes)

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml::sfml)

file(GLOB_RECURSE SOURCES_CLIENT "sources/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_CLIENT})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0501)