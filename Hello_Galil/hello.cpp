//g++ hello.cpp -lGalil
#include "Galil.h"   //vector string Galil
#include <iostream>  //cout
#include <sstream>   //ostringstream istringstream
#include <unistd.h>
#include <math.h>
using namespace std; //cout ostringstream vector string



Galil * galil_connection = NULL;

bool setup() {
  //TODO: make setable variable
  galil_connection = new Galil("/dev/ttyUSB0 115200");
  galil_connection->command("ST; SH; MT2,2,2; ST; AC 60000,,60000; DC 60000,,60000; JG0,0,0; BG ABC;");
  return true;
}

bool homeSwitchOff() {
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_HMC");
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

    //galil_connection->command("ST"); //ST = Stop

    /*while(homeSwitchOff()) {
      write_axis1(1000);
      usleep(10000);
    }*/

    /*galil_connection->command("ST; CN,1; SPC=1000; HM C; BG C;");

    sleep(10);

    galil_connection->command("ST; JG0,0,0; BG C;");*/

    /*for(int i = 0; i < 250; i++) {
      cout << "Got: " << read_axis1() <<std::endl;
      write_axis1(-3000);
      usleep(10000);
    }*/

    for(int i = 0; i < 750; i++) {
      cout << "Got: " << read_axis1() <<std::endl;
      write_axis1(3000);
      usleep(10000);
    }

    galil_connection->command("ST"); //ST = Stop

  }
  catch(string e) {
    cout << e;
  }

}
