cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0100 NEW)

project(InvoiceLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core LinguistTools Sql PrintSupport Gui Network Svg REQUIRED)
find_package(Qt6 COMPONENTS Core LinguistTools Sql PrintSupport Gui Network Svg REQUIRED)


set(SOURCES
    src/InvoiceDbController.cpp
    src/CompanyData.cpp
    src/InvoiceDetail.cpp
    src/InvoiceData.cpp
    src/InvoiceDocument.cpp
    src/InvoiceDetailsModel.cpp
    src/DbMigrator.cpp
    src/WkPdfConverter.cpp
    src/Utils.cpp    
)

set(HEADERS
    include/InvoiceDbController.h
    include/CompanyData.h
    include/InvoiceDetail.h
    include/InvoiceData.h
    include/InvoiceDocument.h
    include/InvoiceDetailsModel.h
    include/DbMigrator.h
    include/WkPdfConverter.h
    include/Utils.h
)

add_library(InvoiceLib STATIC ${SOURCES} ${HEADERS})

target_include_directories(InvoiceLib PUBLIC include/ )
target_link_libraries(InvoiceLib PUBLIC Qt6::Core Qt6::Sql Qt6::PrintSupport Qt6::Gui Qt6::Network Qt6::Svg)

target_compile_definitions(InvoiceLib PRIVATE INVOICELIB_LIBRARY)
