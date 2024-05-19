cmake_minimum_required(VERSION 3.5)

project(InvoiceGui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets Sql Charts WebEngineWidgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets Sql Charts WebEngineWidgets REQUIRED)

set(UI_SOURCES
    src/ui/mainwindow.h src/ui/mainwindow.cpp src/ui/mainwindow.ui
    src/ui/CreatePage.h src/ui/CreatePage.cpp src/ui/CreatePage.ui
    src/ui/MainPage.h src/ui/MainPage.cpp src/ui/MainPage.ui
    src/ui/NewInvoicePage.h src/ui/NewInvoicePage.cpp src/ui/NewInvoicePage.ui
    src/ui/ChartsPage.h src/ui/ChartsPage.cpp src/ui/ChartsPage.ui
    src/ui/MorePage.h src/ui/MorePage.cpp src/ui/MorePage.ui
    src/ui/DbStatusForm.h src/ui/DbStatusForm.cpp src/ui/DbStatusForm.ui
    src/ui/DataHandlerWidget.h src/ui/DataHandlerWidget.cpp src/ui/DataHandlerWidget.ui
    src/ui/CompanyDetailsWidget.h src/ui/CompanyDetailsWidget.cpp src/ui/CompanyDetailsWidget.ui
    src/ui/FileResourceAddWidget.h src/ui/FileResourceAddWidget.cpp src/ui/FileResourceAddWidget.ui
    src/ui/InvoiceViewWidget.h src/ui/InvoiceViewWidget.cpp src/ui/InvoiceViewWidget.ui
    src/ui/NewDataDialog.h src/ui/NewDataDialog.cpp src/ui/NewDataDialog.ui
    src/ui/InvoiceViewDialog.h src/ui/InvoiceViewDialog.cpp src/ui/InvoiceViewDialog.ui
    src/ui/CompanyDetailsDialog.h src/ui/CompanyDetailsDialog.cpp src/ui/CompanyDetailsDialog.ui
    src/ui/SettingsPage.h src/ui/SettingsPage.cpp src/ui/SettingsPage.ui
    src/ui/AboutPage.h src/ui/AboutPage.cpp src/ui/AboutPage.ui
    src/ui/TitleBarWidget.h src/ui/TitleBarWidget.cpp src/ui/TitleBarWidget.ui
    src/ui/InvoiceSortProxyModel.h src/ui/InvoiceSortProxyModel.cpp
    src/ui/GuiUtils.h src/ui/GuiUtils.cpp
    src/ui/FileResourceDialog.h src/ui/FileResourceDialog.cpp src/ui/FileResourceDialog.ui
)

set(OTHER_SOURCES
    src/main.cpp
    src/ClientModel.h src/ClientModel.cpp
    src/FileResourceModel.h src/FileResourceModel.cpp
    src/InvoiceModel.h src/InvoiceModel.cpp
)
set(PROJECT_SOURCES ${UI_SOURCES} ${OTHER_SOURCES})

add_executable(InvoiceGui ${PROJECT_SOURCES})

target_compile_definitions(InvoiceGui PRIVATE APP_VERSION="${APP_VERSION}")
target_include_directories(InvoiceGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(InvoiceGui PRIVATE Qt6::Widgets Qt6::Sql Qt6::Charts Qt6::WebEngineWidgets InvoiceLib)

set_target_properties(InvoiceGui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
