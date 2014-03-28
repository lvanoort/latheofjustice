//g++ hello.cpp -lGalil
#include "Galil.h"   //vector string Galil
#include <iostream>  //cout
#include <sstream>   //ostringstream istringstream
#include <unistd.h>
#include <math.h>

#include "controller.h"

using namespace std; //cout ostringstream vector string

Galil * galil_connection = NULL;

bool setup() {
  //TODO: make setable variable
  galil_connection = new Galil("/dev/ttyUSB0 115200");
  galil_connection->command("ST; SH;MT2,2,2; ST; AC 60000,,60000; DC 60000,,60000; JG0,0,0; BG ABC;");
  return true;
}

bool homeSwitchOff() {
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_HMC");
}

bool returnToHome() {
  if(!galil_connection)
    return false;

  galil_connection->command("ST; LD 3,3,3; CN,1; SPC=22000; HM C; BG C;");
  sleep(5);

  galil_connection->command("DEC=0; LD0,0,0; JG0,0,0; BG C;");
  return true;
}

double read_axis1()
{
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_TPC");
}

void write_axis1(double val)
{
  if(!galil_connection)
    return;
   
  int num = floor(val);
  ostringstream convert;
  convert<<num;
  
  string out = convert.str();
  galil_connection->command("JG,,"+out+";");
}

int main()
{
  try {
    setup();
    returnToHome();


    for(int i = 0; i < 600; i++) {
      double reading = read_axis1();
      cout << reading <<std::endl;
      write_axis1(controller(reading, -1500));
      usleep(10000);
    }

    galil_connection->command("ST"); //ST = Stop

  }
  catch(string e) {
    cout << e;
  }

}
