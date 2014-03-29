#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct _axis_readings {
  double a;
  double b;
  double c;
} axis_readings;

typedef struct _controller_output {
  double a;
  double b;
  double c;
} controller_output;

typedef struct _controller_command {
  double a;
  double b;
  double c;
} controller_command;

controller_output controller(_axis_readings reading, controller_command command);

#endif
