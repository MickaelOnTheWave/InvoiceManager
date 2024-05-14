cmake_minimum_required(VERSION 3.5)

file (STRINGS "VERSION" APP_VERSION)

project(InvoiceManager VERSION "${APP_VERSION}" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

add_subdirectory(InvoiceLib)
add_subdirectory(InvoiceGui)
add_subdirectory(InvoiceCli)
add_subdirectory(InvoiceTests)

add_custom_target(Other SOURCES VERSION LICENSE)
