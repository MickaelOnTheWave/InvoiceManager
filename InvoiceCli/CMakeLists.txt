cmake_minimum_required(VERSION 3.14)

project(InvoiceCli LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/)

set(COMMANDS_SRC
    src/Commands/AbstractCommand.cpp src/Commands/AbstractCommand.h
    src/Commands/InvoiceCommands.cpp src/Commands/InvoiceCommands.h
    src/Commands/CreateFromLastCommand.cpp src/Commands/CreateFromLastCommand.h
    src/Commands/CreatePdfCommand.cpp src/Commands/CreatePdfCommand.h
    src/Commands/ListCommand.cpp src/Commands/ListCommand.h
)

add_executable(InvoiceCli
  src/main.cpp
  src/commandlinemanager.cpp src/commandlinemanager.h
  src/InvoicePrinter.cpp src/InvoicePrinter.h
  src/Tools.cpp src/Tools.h
  src/CliParametersDefinitions.h
  src/SelectCliParamHandler.h src/SelectCliParamHandler.cpp
  ${COMMANDS_SRC}
)
target_compile_definitions(InvoiceCli PRIVATE APP_VERSION="${APP_VERSION}")
target_link_libraries(InvoiceCli InvoiceLib)
