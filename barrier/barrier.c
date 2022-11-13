/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Serial Traffic Barrier and Traffic Light Controller
 * Suvir Kumar <suvir@dits.co.th>
 * version 0.2 updates 12/11/2022 at 22:49
 * Raspbery Pi Pico with ETT GOIO Board
 */

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <string.h>

#define LOW 0
#define HIGH 1

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1


const char VERSION[] = "Version 0.4 Released 13/11/2022";

const uint LED_PIN = 25;    // Pico On Board LED (Output LED)
const uint ETT_PORT_0 = 16; // Barrier Up (Output Relay)
const uint ETT_PORT_1 = 17; // Barrier Down (Output Relay)
const uint ETT_PORT_2 = 18; // Push up, Push Down (Output Relay)
const uint ETT_PORT_3 = 19; // Barrier Position UP (Input Opto)
const uint ETT_PORT_4 = 20; // Barrier Position Down (Input Opto)
const uint ETT_PORT_5 = 21; // Red Light 220V (Output Relay)
const uint ETT_PORT_6 = 22; // Green Light 220V (Output Relay)
const uint ETT_PORT_7 = 28; // Beeper (Output Beeper)

const uint BARRIER_UP_CMD = ETT_PORT_0;
const uint BARRIER_DOWN_CMD = ETT_PORT_1;
const uint BARRIER_PUSH_BTN_CMD = ETT_PORT_2;
const uint BARRIER_UP_SIGNAL = ETT_PORT_3;
const uint BARRIER_DOWN_SIGNAL = ETT_PORT_4;
const uint TRAFFIC_LIGHT_RED = ETT_PORT_5;
const uint TRAFFIC_LIGHT_GREEN = ETT_PORT_6;
const uint BEEPER = ETT_PORT_7;

void sendBarrierPulse(uint BarrierCmd, int delayBefore) {
  gpio_put(BarrierCmd, HIGH);
  sleep_ms(delayBefore);
  gpio_put(BarrierCmd, LOW);
  return;
}

void sendTrafficSignalOn(uint TrafficCmd) {
  gpio_put(TrafficCmd, HIGH);
  return;
}

void sendTrafficSignalOff(uint TrafficCmd) {
  gpio_put(TrafficCmd, LOW);
  return;
}

void showMenu(void) {
  printf("\rDynamic IT Solutions Parking Barrier Controller\n");
  printf("\rBy Suvir Kumar <suvir@dits.co.th>\n");
  printf("%s\n", VERSION);
  printf("\r\nMain Menu\n");
  printf("\n\rc Chk Comm\n\r? Clear Screen & Show Menu\n\r1 Toggle Onboard "
         "LED\n\r2 Barrier Up\n\r3 Barrier Down\n\r4 Barrier Toggle\n\r5 "
         "Barrier Position\n\r6 Red Light On\n\r7 Red Light Off\n\r8 Green "
         "Light On\n\r9 Green Light Off\n\r0 Beep\n\r\n");
  return;
}

int main() {
  int onboard_led = 0;
  int beeper_state = 0;
  int current_barrier_state = 2;
  int previous_barrier_state = 2;

  gpio_init(LED_PIN);
  gpio_init(BARRIER_UP_CMD);
  gpio_init(BARRIER_DOWN_CMD);
  gpio_init(BARRIER_PUSH_BTN_CMD);
  gpio_init(BARRIER_UP_SIGNAL);
  gpio_init(BARRIER_DOWN_SIGNAL);
  gpio_init(TRAFFIC_LIGHT_RED);
  gpio_init(TRAFFIC_LIGHT_GREEN);
  gpio_init(BEEPER);

  gpio_set_dir(LED_PIN, GPIO_OUT);

  gpio_set_dir(BARRIER_UP_CMD, GPIO_OUT);

  gpio_set_dir(BARRIER_DOWN_CMD, GPIO_OUT);

  gpio_set_dir(BARRIER_PUSH_BTN_CMD, GPIO_OUT);

  gpio_set_dir(BARRIER_UP_SIGNAL, GPIO_IN);
  gpio_pull_up(BARRIER_UP_SIGNAL);

  gpio_set_dir(BARRIER_DOWN_SIGNAL, GPIO_IN);
  gpio_pull_up(BARRIER_DOWN_SIGNAL);

  gpio_set_dir(TRAFFIC_LIGHT_RED, GPIO_OUT);

  gpio_set_dir(TRAFFIC_LIGHT_GREEN, GPIO_OUT);

  gpio_set_dir(BEEPER, GPIO_OUT);

  // char buffer[1024];
  char buffer;

  stdio_init_all();
  showMenu();

  while (true) {

    if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
        gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
      	current_barrier_state = 2;
    }
    if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
        gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
      	current_barrier_state = 0;
    }
    if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
        gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
      	current_barrier_state = 1;
    }
    if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
        gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
      	current_barrier_state = 2;
    }

    if (current_barrier_state != previous_barrier_state) {
      if (current_barrier_state == 0) {
        printf("'{\"state\":\"BR DWN\"}'\n");
      }
      if (current_barrier_state == 1) {
        printf("'{\"state\":\"BR UP\"}'\n");
      }
      if (current_barrier_state == 2) {
        printf("'{\"state\":\"BR UNDEF\"}'\n");
      }
      previous_barrier_state = current_barrier_state;
    }

    while (uart_is_readable(uart0)) {
      buffer = getchar();

      if (buffer == 'c') {
        printf("'{\"ack\":\"COM\"}'\n");
      }
      if (buffer == '?') {
        printf("%c%c%c%c", 0x1B, 0x5B, 0x32, 0x4A);
        showMenu();
      }
      if (buffer == '1') {
        if (onboard_led == 0) {
          gpio_put(LED_PIN, HIGH);
          onboard_led = 1;
          printf("'{\"ack\":\"OB LED ON\"}'\n");
        } else if (onboard_led == 1) {
          gpio_put(LED_PIN, LOW);
          onboard_led = 0;
          printf("'{\"ack\":\"OB LED OFF\"}'\n");
        }
      }
      if (buffer == '2') {
        sendBarrierPulse(BARRIER_UP_CMD, 1000);
        printf("'{\"ack\":\"BR UP\"}'\n");
      }
      if (buffer == '3') {
        sendBarrierPulse(BARRIER_DOWN_CMD, 1000);
        printf("'{\"ack\":\"BR DWN\"}'\n");
      }
      if (buffer == '4') {
        sendBarrierPulse(BARRIER_PUSH_BTN_CMD, 1000);
        printf("'{\"ack\":\"BR PB\"}'\n");
      }
      if (buffer == '5') {
        if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
            printf("'{\"ack\":\"BR POS\"}'\n");
            printf("'{\"state\":\"BR UNDEF\"}'\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
            printf("'{\"ack\":\"BR POS\"}'\n");
            printf("'{\"state\":\"BR DWN\"}'\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
            printf("'{\"ack\":\"BR POS\"}'\n");
            printf("'{\"state\":\"BR UP\"}'\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
            printf("'{\"ack\":\"BR POS\"}'\n");
            printf("'{\"state\":\"BR UNDEF\"}'\n");
        }
      }
      if (buffer == '6') {
        sendTrafficSignalOn(TRAFFIC_LIGHT_RED);
        printf("'{\"ack\":\"TL RED ON\"}'\n");
      }
      if (buffer == '7') {
        sendTrafficSignalOff(TRAFFIC_LIGHT_RED);
        printf("'{\"ack\":\"TL RED OFF\"}'\n");
      }
      if (buffer == '8') {
        sendTrafficSignalOn(TRAFFIC_LIGHT_GREEN);
        printf("'{\"ack\":\"TL GRN ON\"}'\n");
      }
      if (buffer == '9') {
        sendTrafficSignalOff(TRAFFIC_LIGHT_GREEN);
        printf("'{\"ack\":\"TL GRN OFF\"}'\n");
      }
      if (buffer == '0') {
        if (beeper_state == 0) {
          gpio_put(BEEPER, HIGH);
          beeper_state = 1;
         printf("'{\"ack\":\"BEEP ON\"}'\n");
        } else if (beeper_state == 1) {
          gpio_put(BEEPER, LOW);
          beeper_state = 0;
         printf("'{\"ack\":\"BEEP OFF\"}'\n");
        }
      }
      if (buffer == 'U') {
         printf("'{\"ack\":\"FIRMWARE UPLOAD\"}'\n");
        reset_usb_boot(0, 0);
      }
    }
  }
}
