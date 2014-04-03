//g++ hello.cpp -lGalil
#include "Galil.h"   //vector string Galil
#include <iostream>  //cout
#include <sstream>   //ostringstream istringstream
#include <fstream>
#include <unistd.h>
#include <math.h>

#include "controller.h"

using namespace std; //cout ostringstream vector string

Galil * galil_connection = NULL;

bool setup() {
  //TODO: make setable variable
  galil_connection = new Galil("/dev/ttyUSB0 115200");
  galil_connection->command("ST; SH; MT2,2,2; ST; AC 90000,,90000; DC 90000,,90000; JG0,0,0; BG ABC;");
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

  //Home C axis
  galil_connection->command("ST; LD 3,3,3; CN,1; SPC=22000; HM C; BG AC;");
  sleep(5);
  //galil_connection->command("DEA=0; DEC=0; LD0,0,0; JG0,0,0; BG ABC;");

  //Home A axis
  galil_connection->command("ST; LD 3,3,3; CN,-1; SPA=22000; HM A; BG A;");
  sleep(45);
  galil_connection->command("ST; DEC=0; JG0,0,0; BG ABC;");

  galil_connection->command("JG60000,0,0;");
  sleep(3);
  galil_connection->command("ST; JG0,0,0;");
  sleep(1);
  galil_connection->command("DEA=0"); //Zero A

  galil_connection->command("BG C; JG0,0,-6000;");
  sleep(1);
  galil_connection->command("ST; JG0,0,0;");

  //Rehome the stupid C
 /* galil_connection->command("ST; LD 3,3,3; CN,1; SPC=22000; HM C; BG C;");
  sleep(3);
  galil_connection->command("JG0,0,2000;");
  sleep(3);*/
  galil_connection->command("JG0,0,0; LD0,0,0;");

  //Don't scare people
  sleep(3);

  galil_connection->command("BG ABC;");

  return true;
}

double read_axis1()
{
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_TPC");
}

double read_axis2()
{
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_TPA");
}

double read_axis3()
{
  if(!galil_connection)
    return 0;
  return galil_connection->commandValue("MG_TPB");
}

axis_readings get_readings() {
  axis_readings out;
  out.a = read_axis2();
  out.b = read_axis3();
  out.c = read_axis1();
  return out;
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

void write_axes(double a, double b, double c)
{
  if(!galil_connection)
    return;
   
  ostringstream convert;
  convert<<"BG ABC; JG"
         <<((int) floor(a))<<","
         <<((int) floor(b))<<","
         <<((int) floor(c));
  
  string out = convert.str();

  //cout<<out<<endl;

  galil_connection->command(out);
}

int stringToInt (std::string s) {
   int d;
   std::stringstream ss(s);
   ss >> d;
   return d;
}

int main()
{
  try {
    setup();
    returnToHome();

    cout<<"Homing complete"<<std::endl;
    
    string s;
    std::vector<controller_command> commands;
    commands.clear();
    ifstream commandFile ("traj.csv");
	  std::string line;
    while(std::getline(commandFile,line)) {
      std::string a, b ,c;
      int ai, bi, ci;
      std::stringstream  lineStream(line);
      std::string cell;
      std::getline(lineStream,a,',');
      std::getline(lineStream,b,',');
      std::getline(lineStream,c,',');
      controller_command command;
      command.a = stringToInt(a); 
      command.b = stringToInt(b); 
      command.c = stringToInt(c); 
      commands.push_back(command);
    }

    cout<<"Running"<<endl;

    for(int i = 0; i < commands.size(); i++) {
      controller_command command = commands[i];
      axis_readings readings = get_readings();
      controller_output output = controller(readings, command);
      write_axes(output.a,0,output.c);
      usleep(10000);
    }       

    galil_connection->command("ST"); //ST = Stop

  }
  catch(string e) {
    cout << e;
  }

}

