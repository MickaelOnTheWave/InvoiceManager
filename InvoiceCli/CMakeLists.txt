cmake_minimum_required(VERSION 3.14)

project(InvoiceCli LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core LinguistTools Widgets PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Core LinguistTools Widgets PrintSupport REQUIRED)

set(TS_FILES InvoiceCli_en_GB.ts)

add_executable(InvoiceCli
  src/main.cpp
  src/commandlinemanager.cpp src/commandlinemanager.h
  src/InvoicePrinter.cpp src/InvoicePrinter.h
  src/Tools.cpp src/Tools.h
  src/CliParametersDefinitions.h
  src/CreateFromLastCommand.cpp src/CreateFromLastCommand.h
  src/CreatePdfCommand.h src/CreatePdfCommand.cpp
  src/Commands.cpp src/Commands.h
  src/SelectCliParamHandler.h src/SelectCliParamHandler.cpp
  ${TS_FILES}
)
target_link_libraries(InvoiceCli Qt5::Core Qt5::Widgets Qt5::PrintSupport InvoiceLib)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
