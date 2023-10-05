# InvoiceManager

A simple Invoice Database with a GUI client and a Command line Interface program to handle it.

![New Invoice](https://github.com/MickaelOnTheWave/InvoiceManager/assets/61214819/a2aeddc5-52c3-4fb4-8138-7b599b6bbbef)

![CLI help](https://github.com/MickaelOnTheWave/InvoiceManager/assets/61214819/5df0e893-6826-4fc7-aad6-1c1b2ff6ae66)


## Usage

The idea of this manager is to generate / use the data of the invoices and store them in an SQLite Database. The visual representation is generated as an HTML page.
For that it uses two concepts :
- A Template file.
  This file is the HTML structure of the invoice. The final values are placeholders that are replace by the invoice data when the visualization is generated.
- A Stylesheet file.
  This is a CSS stylesheet. It is used to change the appearance and the style of the HTML invoice.
Both the template files and the stylesheets are also stored in the database. At the moment, only a reference to the files is stored in the database, so you need to keep the files.
One example of each file is available in the data/ folder of the project.

You can use the GUI application to manage the invoices or use the Command line interface for some commands.
For the command line interface, there is a --help command that shows you the available commands and some usage instructions.

## How to build it

The project is built with Cmake. You can either use CMakeLists.txt directly in your IDE (tested only with QT Creator) or build using the command line.
Here are details instructions on how to build it from command line on Linux :

- Clone the git repository into your local machine
- Create a new folder where the build will be done (we will take build-InvoiceManager here as an example)
- Go to this directory, and run "cmake [PATH_TO_INVOICEMANAGER_REPOSITORY] ./
- run "cmake --build ."
You should have 3 folders : InvoiceGui, InvoiceCli and InvoiceLib. You will find corresponding executable files in InvoiceGui and InvoiceCli folders. You can launch them directly to run the Graphical user interface or the command line respectively.

## Platforms

InvoiceManager is built around Qt Framework, and as such should run on all platforms that are supported by Qt : Windows, Mac and Linux.
Please note however that as of today, it has been tested only on Linux.

This program with all its components are released according to the terms of the GNU GPLv3 license. The details about this license are provided in the LICENSE file.

## Creating templates

A template is an HTML file with placeholder fields that will be replaced by the invoice data.
Here are the fields that are available :
- {ID}
- {DATE}
- {USER-COMPANY-NAME}
- {USER-COMPANY-ADDRESS}
- {USER-COMPANY-EMAIL}
- {USER-COMPANY-PHONE}
- {CLIENT-NAME}
- {CLIENT-ADDRESS}
- {CLIENT-EMAIL}
- {CLIENT-PHONE}
- {INVOICE-DETAILS}
- {INVOICE-TOTAL}
- {CURRENCY}
- {NOTES}

Their names are self explanatory. There are, however, some fields that have a special behaviour :
- {USER-COMPANY-ADDRESS} and {CLIENT-ADDRESS} : The address is a multiline property. In order to allow it to be also represented in a multiline form in the invoice, they need to be enclosed with &lt;p&gt;&lt;/p&gt; tags. What will be replaced is &lt;p&gt;{ADRESS}&lt;/p&gt;, and each line will be enclosed in &lt;p&gt;&lt;/p&gt; tags.
- {INVOICE-DETAILS} : Details are meant to be represented in a table structure, they need to be enclosed with &lt;tr&gt;&lt;/tr&gt; tags. What will be replaced is &lt;tr&gt;{INVOICE-DETAILS}&lt;/tr&gt;, with each detail being enclosed in &lt;tr&gt;&lt;/tr&gt; tags, and each cell in it being in &lt;td&gt;&lt;/td&gt; tags.
