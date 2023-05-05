#include <stdio.h>
#include "pico/stdlib.h"
#include "rios.h"
#include "pico/cyw43_arch.h"
#define PINS
int TickFct_1(int a);
int TickFct_2(int a);
int TickFct_3(int a);
int64_t disablePin(alarm_id_t id, void *user_data);
int flash(int a);

int toggle=0;
#define numLeds 2
uint leds[numLeds] = {9,12};

int setupPins() {
    for (int i =0;  i<numLeds ; i++) {
        gpio_init(leds[i]);
        gpio_set_dir(leds[i],GPIO_OUT);
    }
}

int main(void) {
    stdio_init_all();
    setupPins();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    
    add_task(&TickFct_1, 500);
    add_task(&TickFct_2, 1000);

    start_rios(1000);
}

int TickFct_1(int state) {
    if (toggle) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    } else{
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    }
    toggle=!toggle;  
    return state++;
}

int TickFct_2(int state) {
    printf("pin 9");
    flash(0);
    return state++;
}

int TickFct_3(int state) {
    printf("pin 12");
    flash(1);
    return state++;
}

int flash(int index) {
    gpio_put(leds[index],1);
    add_alarm_in_ms(250,&disablePin,&leds[index],true);
    //sleep didn't work.
}

int64_t disablePin(alarm_id_t id, void *user_data) {
    printf("here");
    uint *pin = (uint*) user_data;
    gpio_put(*pin,0);
    return 0;
}
