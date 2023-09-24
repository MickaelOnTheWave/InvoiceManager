# InvoiceManager

A simple Invoice Database with a GUI client to handle it

![New Invoice](https://github.com/MickaelOnTheWave/InvoiceManager/assets/61214819/a2aeddc5-52c3-4fb4-8138-7b599b6bbbef)

## Usage

The idea of this manager is to generate / use the data of the invoices and store them in an SQLite Database. The visual representation is generated as an HTML page.
For that it uses two concepts :
- A Template file.
  This file is the HTML structure of the invoice. The final values are placeholders that are replace by the invoice data when the visualization is generated.
- A Stylesheet file.
  This is a CSS stylesheet. It is used to change the appearance and the style of the HTML invoice.
Both the template files and the stylesheets are also stored in the database. At the moment, only a reference to the files is stored in the database, so you need to keep the files.
One example of each file is available in the data/ folder of the project.

## How to build it

The project is built with Cmake. You can either use CMakeLists.txt directly in your IDE (tested only with QT Creator) or build using the command line.
Here are details instructions on how to build it from command line on Linux :

- Clone the git repository into your local machine
- Create a new folder where the build will be done (we will take build-InvoiceManager here as an example)
- Go to this directory, and run "cmake [PATH_TO_INVOICEMANAGER_REPOSITORY] ./
- run "cmake --build ."
You should have an "InvoiceManager" executable file. You can launch it directly to run InvoiceManager.

## Platforms

InvoiceManager is built around Qt Framework, and as such should run on all platforms that are supported by Qt : Windows, Mac and Linux.
Please note however that as of today, it has been tested only on Linux.

This program with all its components are released according to the terms of the GNU GPLv3 license. The details about this license are provided in the LICENSE file.
