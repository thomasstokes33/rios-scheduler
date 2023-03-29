#include <stdio.h>
#include "pico/stdlib.h"
#include "rios.h"

int main(void) {
    stdio_init_all();

    // add_task(&TickFct_1, 500);
    // add_task(&TickFct_2, 1000);

    // start_rios(500);
}

int TickFct_1(int state) {
    return 0;
}

int TickFct_2(int state) {
    return 0;
}
