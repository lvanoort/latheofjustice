#include "controller.h"

const double kPc = 50.f;
const double kPa = 0.f;

controller_output controller(_axis_readings reading,double command) {
  controller_output output;

  double error_c = command - reading.c;
  output.c = kPc*error_c;

  double a_reading = reading.a - reading.c;
  double a_error = a_reading - 0; //Hold position
  output.a = output.c + kPa*a_error;

  output.b = 0;

  return output;
}
