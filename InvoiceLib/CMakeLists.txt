cmake_minimum_required(VERSION 3.14)

project(InvoiceLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core LinguistTools Sql PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core LinguistTools Sql PrintSupport REQUIRED)

set(TS_FILES InvoiceLib_en_GB.ts)

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
  ${TS_FILES}
)

target_include_directories(InvoiceLib PUBLIC include/)
target_link_libraries(InvoiceLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql
                                         Qt5::PrintSupport)

target_compile_definitions(InvoiceLib PRIVATE INVOICELIB_LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
