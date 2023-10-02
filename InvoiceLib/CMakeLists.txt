cmake_minimum_required(VERSION 3.14)

project(InvoiceLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core Sql Gui REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Sql Gui REQUIRED)

set(SOURCES
    src/InvoiceDbController.cpp
    src/CompanyData.cpp
    src/InvoiceDetail.cpp
    src/InvoiceData.cpp
    src/InvoiceDocument.cpp
)

set(HEADERS
    include/InvoiceDbController.h
    include/CompanyData.h
    include/InvoiceDetail.h
    include/InvoiceData.h
    include/InvoiceDocument.h
)

add_library(InvoiceLib STATIC
  ${SOURCES}
  ${HEADERS}
)

target_include_directories(InvoiceLib PUBLIC include/)
target_link_libraries(InvoiceLib PRIVATE Qt6::Core Qt6::Sql Qt6::Gui)

target_compile_definitions(InvoiceLib PRIVATE INVOICELIB_LIBRARY)
