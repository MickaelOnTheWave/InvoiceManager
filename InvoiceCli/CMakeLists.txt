cmake_minimum_required(VERSION 3.14)

project(InvoiceCli LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core Widgets REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(InvoiceCli
  src/main.cpp
  src/commandlinemanager.cpp src/commandlinemanager.h
  src/InvoicePrinter.cpp src/InvoicePrinter.h
  src/Tools.cpp src/Tools.h
  src/CliParametersDefinitions.h
  src/CreateFromLastCommand.cpp src/CreateFromLastCommand.h
  src/CreatePdfCommand.h src/CreatePdfCommand.cpp
  src/ShowCommand.cpp src/ShowCommand.h
  src/SelectCliParamHandler.h src/SelectCliParamHandler.cpp
)
target_link_libraries(InvoiceCli Qt6::Core Qt6::Widgets InvoiceLib)
