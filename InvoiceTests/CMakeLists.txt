cmake_minimum_required(VERSION 3.14)

project(InvoiceTests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)

add_executable(InvoiceTests src/tests.cpp)
target_link_libraries(InvoiceTests InvoiceLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(InvoiceTests)

