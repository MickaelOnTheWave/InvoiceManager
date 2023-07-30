cmake_minimum_required(VERSION 3.5)

project(InvoiceManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        CreatePage.h CreatePage.cpp CreatePage.ui
        MainPage.h MainPage.cpp MainPage.ui
        NewInvoicePage.h NewInvoicePage.cpp NewInvoicePage.ui
        MorePage.h MorePage.cpp MorePage.ui
        DbStatusForm.h DbStatusForm.cpp DbStatusForm.ui
        DataHandlerWidget.h DataHandlerWidget.cpp DataHandlerWidget.ui
        CompanyDetailsWidget.h CompanyDetailsWidget.cpp CompanyDetailsWidget.ui
        StylesheetDetailsWidget.h StylesheetDetailsWidget.cpp StylesheetDetailsWidget.ui
        NewDataDialog.h NewDataDialog.cpp NewDataDialog.ui
        GuiUtils.h GuiUtils.cpp
        InvoiceDbController.h InvoiceDbController.cpp
        Company.h Company.cpp
        InvoiceDetail.h InvoiceDetail.cpp
        ClientModel.h ClientModel.cpp
        StylesheetModel.h StylesheetModel.cpp
        InvoiceModel.h InvoiceModel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(InvoiceManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET InvoiceManager APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(InvoiceManager SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(InvoiceManager
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(InvoiceManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(InvoiceManager PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(InvoiceManager)
endif()
