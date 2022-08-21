#ifndef _input_h
#define _input_h
#include <Arduino.h>
#include "constants.h"
void input_setup();
bool input_up();
bool input_down();
bool input_left();
bool input_right();
bool input_fire();
void input_matrix_data();
bool input_exit();
#endif
