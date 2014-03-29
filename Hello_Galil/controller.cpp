#include "controller.h"

const double kPc = 50.f;
const double kPa = 30.f;

controller_output controller(_axis_readings reading,controller_command command) {
  controller_output output;

  double error_c = command.c - reading.c;
  output.c = kPc*error_c;

  double a_reading = reading.c - reading.a;
  double a_error = a_reading - command.a; //Hold position
  output.a = output.c + kPa*a_error;

  output.b = 0;

  return output;
}
