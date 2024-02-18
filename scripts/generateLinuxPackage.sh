#/bin/sh

Qt5Dir="/usr/lib/x86_64-linux-gnu/cmake/Qt5"
version=`cat ../VERSION`


buildApp() {
    buildDir="InvoiceManager-$version-build"
    cd ../builds

    if [ -d "$buildDir" ]; then
        echo "$buildDir already exists. Cleaning it."
        rm -Rf "$buildDir"
    fi

    mkdir "$buildDir"
    cmake -S ../ -DQt5_DIR=${Qt5Dir} -DCMAKE_BUILD_TYPE:STRING=Release -B "$buildDir"
    cmake --build "$buildDir"
}

createPackageStructure()
{
    packageName="InvoiceManager-$version"
    packageBinFolder="$packageName/usr/local/bin"
    packageControlFolder="$packageName/DEBIAN"

    if [ -d "$packageName" ]; then
        echo "$packageName already exists. Cleaning it."
        rm -Rf "$packageName"
    fi

	mkdir $packageName
	mkdir $packageName/usr
	mkdir $packageName/usr/local
	mkdir $packageBinFolder
	mkdir $packageControlFolder
}

copyFilesToStructure()
{
	cp "$buildDir/InvoiceCli/InvoiceCli" $packageBinFolder/invoiceCli
	cp "$buildDir/InvoiceGui/InvoiceGui" $packageBinFolder/invoiceGui
}

createDebianControlFile()
{
	CONTROL_FILE="$packageControlFolder/control"

	echo "Package: Invoice-Manager" >> $CONTROL_FILE
	echo "Version: $version" >> $CONTROL_FILE
	echo "Section: tasks" >> $CONTROL_FILE
	echo "Priority: optional" >> $CONTROL_FILE
	echo "Architecture: all" >> $CONTROL_FILE
	#echo "Depends: libsomethingorrather (>= 1.2.13), anotherDependency (>= 1.2.6)" >> $CONTROL_FILE
	echo "Maintainer: Mickael C. Guimarães <mickael.dacruz@gmail.com>" >> $CONTROL_FILE
	echo "Description: Invoice Manager" >> $CONTROL_FILE
	echo " A command-line tool and a Gui to create and manage Invoices" >> $CONTROL_FILE
}

generateDebPackage() {
    createPackageStructure
    copyFilesToStructure
    createDebianControlFile
    dpkg-deb --build $packageName
}



buildApp
generateDebPackage

