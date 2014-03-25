CONNECTING TO A GALIL CONTROLLER USING THE GALIL COMMUNICATIONS C++ LIBRARY WITH THE GNU C++ COMPILER

(1) Install GalilTools version 1.4.0.0 or newer.  Lite is free of charge, the full version requires a purchased password.  Either is fine for this walkthrough.
http://www.galilmc.com/support/software-downloads.php
Linux Installation Instructions:
http://www.galilmc.com/support/manuals/galiltools/introduction.html#linuxinstall

(2) Connect to your controller and then close GalilTools
http://www.galilmc.com/support/manuals/galiltools/connections.html

(3) Open a text editor window and enter the following code.

//g++ hello.cpp -lGalil
#include "Galil.h"   //vector string Galil
#include <iostream>  //cout
#include <sstream>   //ostringstream istringstream
using namespace std; //cout ostringstream vector string

int main()
{
	try
	{
		Galil g;  //Open connections dialog
		cout << g.connection() << endl; //Print connection string
		cout << g.libraryVersion() << endl; //Print Library Version
		cout << "MG TIME " << g.command("MG TIME") << endl; //Send MG TIME to controller and get response
	}
	catch(string e)
	{
		cout << e;
	}
}

(4) Save the file as hello.cpp

(5) Open a terminal window, goto the hello.cpp location

(6) Compile and run hello.cpp.  The program output should be similar to the following:
[user@localhost Desktop]$ g++ hello.cpp -lGalil
[user@localhost Desktop]$ ./a.out
10.0.51.64, DMC2182 Rev 1.0q2, 30664, IHA IHB
libGalil.a 1.4.0
MG TIME 17878816.0000

Also see:
http://www.galilmc.com/support/manuals/galiltools/library.html
