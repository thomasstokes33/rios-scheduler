#ifndef MAIN_H
# define MAIN_H

#include "pico/stdlib.h"

int TickFct_1(int state);
int TickFct_2(int state);
int TickFct_3(int state);
int TickFct_4(int state);
int TickFct_5(int state);
int TickFct_6(int state);

void init_pins();
void flash_pin(int idx);

#endif