cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0100 NEW)

project(InvoiceLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core LinguistTools Sql PrintSupport Gui WebKit Network XmlPatterns Svg WebKitWidgets REQUIRED)
find_package(Qt5 COMPONENTS Core LinguistTools Sql PrintSupport Gui WebKit Network XmlPatterns Svg WebKitWidgets REQUIRED)

set(TS_FILES InvoiceLib_en_GB.ts)

set(WKHTMLTOPDF_SOURCES
    src/wkhtmltopdf/shared/outputter.cc
    src/wkhtmltopdf/shared/manoutputter.cc
    src/wkhtmltopdf/shared/htmloutputter.cc
    src/wkhtmltopdf/shared/textoutputter.cc
    src/wkhtmltopdf/shared/arghandler.cc
    src/wkhtmltopdf/shared/commondocparts.cc
    src/wkhtmltopdf/shared/commandlineparserbase.cc
    src/wkhtmltopdf/shared/commonarguments.cc
    src/wkhtmltopdf/shared/progressfeedback.cpp
    src/wkhtmltopdf/lib/loadsettings.cc
    src/wkhtmltopdf/lib/logging.cc
    src/wkhtmltopdf/lib/multipageloader.cpp
    src/wkhtmltopdf/lib/tempfile.cc
    src/wkhtmltopdf/lib/converter.cpp
    src/wkhtmltopdf/lib/websettings.cc
    src/wkhtmltopdf/lib/reflect.cc
    src/wkhtmltopdf/lib/utilities.cpp
    src/wkhtmltopdf/lib/pdfsettings.cc
    src/wkhtmltopdf/lib/pdfconverter.cpp
    src/wkhtmltopdf/lib/outline.cc
    src/wkhtmltopdf/lib/tocstylesheet.cc
    src/wkhtmltopdf/lib/imagesettings.cc
    src/wkhtmltopdf/lib/imageconverter.cc
    src/wkhtmltopdf/lib/pdf_c_bindings.cc
    src/wkhtmltopdf/lib/image_c_bindings.cc
)

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

set(WKHTMLTOPDF_HEADERS
    include/wkhtmltopdf/lib/converter.h
    include/wkhtmltopdf/lib/multipageloader.h
    include/wkhtmltopdf/lib/dllbegin.inc
    include/wkhtmltopdf/lib/dllend.inc
    include/wkhtmltopdf/lib/loadsettings.hh
    include/wkhtmltopdf/lib/websettings.hh
    include/wkhtmltopdf/lib/utilities.h
    include/wkhtmltopdf/lib/multipageloader_p.h
    include/wkhtmltopdf/lib/converter_p.h
    include/wkhtmltopdf/lib/pdfconverter.h
    include/wkhtmltopdf/lib/pdfsettings.hh
    include/wkhtmltopdf/lib/pdfconverter_p.h
    include/wkhtmltopdf/lib/imageconverter.hh
    include/wkhtmltopdf/lib/imagesettings.hh
    include/wkhtmltopdf/lib/imageconverter_p.hh
    include/wkhtmltopdf/lib/pdf.h
    include/wkhtmltopdf/lib/image.h
    include/wkhtmltopdf/lib/pdf_c_bindings_p.hh
    include/wkhtmltopdf/lib/image_c_bindings_p.hh
    include/wkhtmltopdf/shared/progressfeedback.h
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

add_library(InvoiceLib STATIC
  ${SOURCES} ${WKHTMLTOPDF_SOURCES}
  ${HEADERS} ${WKHTMLTOPDF_HEADERS}
  ${TS_FILES}
)

target_include_directories(InvoiceLib PUBLIC include/ include/wkhtmltopdf/ include/wkhtmltopdf/shared include/wkhtmltopdf/lib)
target_link_libraries(InvoiceLib PUBLIC Qt5::Core Qt5::Sql Qt5::PrintSupport Qt5::Gui Qt5::WebKit Qt5::Network Qt5::XmlPatterns Qt5::Svg Qt5::WebKitWidgets)

target_compile_definitions(InvoiceLib PRIVATE INVOICELIB_LIBRARY)
target_compile_definitions(InvoiceLib PUBLIC BUILDING_WKHTMLTOX)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
