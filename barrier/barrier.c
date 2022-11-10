/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Suvir Kumar <suvir@dits.co.th>
 * version 0.1 updates 10/11/2022 at 12:57
 */

#include "pico/stdlib.h"

int main() {
   // Barrier Controller Interface
   //up   (Output)
   //down (Output)
   //push (Output)
   //red  (Input)
   //green (Input)

    //define board pins
    const uint LED_PIN = 25;
    const uint ETT_PORT_0 = 16;
    const uint ETT_PORT_1 = 17;
    const uint ETT_PORT_2 = 18;
    const uint ETT_PORT_3 = 19;
    const uint ETT_PORT_4 = 20;
    const uint ETT_PORT_5 = 21;
    const uint ETT_PORT_6 = 22;
    const uint ETT_PORT_7 = 28;

    //initalize board pins
    gpio_init(LED_PIN);
    gpio_init(ETT_PORT_0);
    gpio_init(ETT_PORT_1);
    gpio_init(ETT_PORT_2);
    gpio_init(ETT_PORT_3);
    gpio_init(ETT_PORT_4);
    gpio_init(ETT_PORT_5);
    gpio_init(ETT_PORT_6);
    gpio_init(ETT_PORT_7);

    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(ETT_PORT_0, GPIO_OUT);
    gpio_set_dir(ETT_PORT_1, GPIO_OUT);
    gpio_set_dir(ETT_PORT_2, GPIO_OUT);
    gpio_set_dir(ETT_PORT_3, GPIO_OUT);
    gpio_set_dir(ETT_PORT_4, GPIO_OUT);
    gpio_set_dir(ETT_PORT_5, GPIO_OUT);
    gpio_set_dir(ETT_PORT_6, GPIO_OUT);
    gpio_set_dir(ETT_PORT_7, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        gpio_put(ETT_PORT_0, 1);
        sleep_ms(2000);
        gpio_put(LED_PIN, 0);
        gpio_put(ETT_PORT_0, 0);
        sleep_ms(1000);
    }
}
