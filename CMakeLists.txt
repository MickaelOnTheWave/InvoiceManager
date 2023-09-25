cmake_minimum_required(VERSION 3.5)

project(InvoiceManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(InvoiceGUI)
#add_subdirectory(InvoiceCLI)
add_subdirectory(InvoiceModule)
