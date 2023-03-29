#include "main.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "rios.h"

# define LEDS_TO_FLASH 6
uint pins[LEDS_TO_FLASH] = {6,7,8,9,10,11};

int main(void) {
    stdio_init_all();

    init_pins();

    add_task(&TickFct_1, 500);
    add_task(&TickFct_2, 1000);
    add_task(&TickFct_3, 2000);
    add_task(&TickFct_4, 4000);
    add_task(&TickFct_5, 8000);
    add_task(&TickFct_6, 16000);

    start_rios(500);
}

void init_pins() {
    for (int i=0; i<LEDS_TO_FLASH; ++i) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_OUT);
    }
}

/* 
 * Alarm callback user_data must live logner than the function that setups the alarm.
 * This is why we use a pointer to the GPIO pin number stored in the pins array, as it lives forever.
 * Simply passing a pointer to the pin number in the local function's scope would cause a dangling pointer / use-after-free issue.
 */
int64_t disable_pin(alarm_id_t id, void *user_data) {
    uint *pin = (uint *)user_data;
    gpio_put(*pin, 0);
    return 0;
}

void flash_pin(int idx) {
    gpio_put(pins[idx], 1);
    add_alarm_in_ms(250, disable_pin, &pins[idx], true);
}

int TickFct_1(int state) {
    printf("Task 1!\n");
    flash_pin(0);
    return ++state;
}

int TickFct_2(int state) {
    printf("Task 2!\n");
    flash_pin(1);
    return ++state;
}

int TickFct_3(int state) {
    printf("Task 3!\n");
    flash_pin(2);
    return 0;
}

int TickFct_4(int state) {
    printf("Task 4!\n");
    flash_pin(3);
    return 0;
}

int TickFct_5(int state) {
    printf("Task 5!\n");
    flash_pin(4);
    return 0;
}

int TickFct_6(int state) {
    printf("Task 6!\n");
    flash_pin(5);
    return 0;
}
