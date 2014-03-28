#include "controller.h"

const double kP = 50;

double controller(double reading,double command) {
  double error = command - reading;
  return kP*error;
}
