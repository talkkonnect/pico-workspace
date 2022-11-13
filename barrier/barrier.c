/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Serial Traffic Barrier and Traffic Light Controller
 * Suvir Kumar <suvir@dits.co.th>
 * Raspbery Pi Pico with ETT GOIO Board
 */

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <string.h>

#define UART0_ID uart0
#define BAUD0_RATE 115200
#define DATA0_BITS 8
#define STOP0_BITS 1
#define PARITY0    UART_PARITY_NONE
#define UART0_TX_PIN 0
#define UART0_RX_PIN 1

#define UART1_ID uart1
#define BAUD1_RATE 115200
#define DATA1_BITS 8
#define STOP1_BITS 1
#define PARITY1    UART_PARITY_NONE
#define UART1_TX_PIN 8
#define UART1_RX_PIN 9

#define LOW 0
#define HIGH 1

const char VERSION[] = "Version 1.0 Released 13/11/2022 at 22:20 (ICT)\r\n";

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

int onboard_led = 0;
int beeper_state = 0;
int current_barrier_state = 2;
int previous_barrier_state = 2;

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
  uart_puts(UART0_ID, "Dynamic IT Solutions Parking Barrier Controller\r\n");
  uart_puts(UART0_ID, "By Suvir Kumar <suvir@dits.co.th>\r\n");
  uart_puts(UART0_ID, VERSION);
  uart_puts(UART0_ID,"\r\nMain Menu\r\n");
  uart_puts(UART0_ID,"\r\nc Chk Comm\r\n? Clear Screen & Show Menu\r\n1 Toggle Onboard "
         "LED\r\n2 Barrier Up\r\n3 Barrier Down\r\n4 Barrier Toggle\r\n5 "
         "Barrier Position\r\n6 Red Light On\r\n7 Red Light Off\r\n8 Green "
         "Light On\r\n9 Green Light Off\r\n0 Beep\r\n");

  uart_puts(UART1_ID, "Dynamic IT Solutions Parking Barrier Controller\r\n");
  uart_puts(UART1_ID, "By Suvir Kumar <suvir@dits.co.th>\r\n");
  uart_puts(UART1_ID, VERSION);
  uart_puts(UART1_ID,"\r\nMain Menu\r\n");
  uart_puts(UART1_ID,"\r\nc Chk Comm\r\n? Clear Screen & Show Menu\r\n1 Toggle Onboard "
         "LED\r\n2 Barrier Up\r\n3 Barrier Down\r\n4 Barrier Toggle\r\n5 "
         "Barrier Position\r\n6 Red Light On\r\n7 Red Light Off\r\n8 Green "
         "Light On\r\n9 Green Light Off\r\n0 Beep\r\n");
  
  return;
}

void respond(char buffer)  {
      if (buffer == 'c') {
        uart_puts(UART0_ID,"'{\"ack\":\"COM\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"COM\"}'\r\n");
      }
      if (buffer == '?') {
        uart_putc(UART0_ID, 0x1B);
        uart_putc(UART0_ID, 0x5B);
        uart_putc(UART0_ID, 0x32);
        uart_putc(UART0_ID, 0x4A);
        uart_putc(UART1_ID, 0x1B);
        uart_putc(UART1_ID, 0x5B);
        uart_putc(UART1_ID, 0x32);
        uart_putc(UART1_ID, 0x4A);
        showMenu();
      }
      if (buffer == '1') {
        if (onboard_led == 0) {
          gpio_put(LED_PIN, HIGH);
          onboard_led = 1;
          uart_puts(UART0_ID,"'{\"ack\":\"OB LED ON\"}'\r\n");
          uart_puts(UART1_ID,"'{\"ack\":\"OB LED ON\"}'\r\n");
        } else if (onboard_led == 1) {
          gpio_put(LED_PIN, LOW);
          onboard_led = 0;
          uart_puts(UART0_ID,"'{\"ack\":\"OB LED OFF\"}'\r\n");
          uart_puts(UART1_ID,"'{\"ack\":\"OB LED OFF\"}'\r\n");
        }
      }
      if (buffer == '2') {
        sendBarrierPulse(BARRIER_UP_CMD, 1000);
        uart_puts(UART0_ID,"'{\"ack\":\"BR UP\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"BR UP\"}'\r\n");
      }
      if (buffer == '3') {
        sendBarrierPulse(BARRIER_DOWN_CMD, 1000);
        uart_puts(UART0_ID,"'{\"ack\":\"BR DWN\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"BR DWN\"}'\r\n");
      }
      if (buffer == '4') {
        sendBarrierPulse(BARRIER_PUSH_BTN_CMD, 1000);
        uart_puts(UART0_ID,"'{\"ack\":\"BR PB\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"BR PB\"}'\r\n");
      }
      if (buffer == '5') {
        if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
            uart_puts(UART0_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART0_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
            uart_puts(UART1_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART1_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 0 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
            uart_puts(UART0_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART0_ID,"'{\"state\":\"BR DWN\"}'\r\n");
            uart_puts(UART1_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART1_ID,"'{\"state\":\"BR DWN\"}'\r\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 0) {
            uart_puts(UART0_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART0_ID,"'{\"state\":\"BR UP\"}'\r\n");
            uart_puts(UART1_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART1_ID,"'{\"state\":\"BR UP\"}'\r\n");
        }
        if (gpio_get(BARRIER_UP_SIGNAL) == 1 &&
            gpio_get(BARRIER_DOWN_SIGNAL) == 1) {
            uart_puts(UART0_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART0_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
            uart_puts(UART1_ID,"'{\"ack\":\"BR POS\"}'\r\n");
            uart_puts(UART1_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
        }
      }
      if (buffer == '6') {
        sendTrafficSignalOn(TRAFFIC_LIGHT_RED);
        uart_puts(UART0_ID,"'{\"ack\":\"TL RED ON\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"TL RED ON\"}'\r\n");
      }
      if (buffer == '7') {
        sendTrafficSignalOff(TRAFFIC_LIGHT_RED);
        uart_puts(UART0_ID,"'{\"ack\":\"TL RED OFF\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"TL RED OFF\"}'\r\n");
      }
      if (buffer == '8') {
        sendTrafficSignalOn(TRAFFIC_LIGHT_GREEN);
        uart_puts(UART0_ID,"'{\"ack\":\"TL GRN ON\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"TL GRN ON\"}'\r\n");
      }
      if (buffer == '9') {
        sendTrafficSignalOff(TRAFFIC_LIGHT_GREEN);
        uart_puts(UART0_ID,"'{\"ack\":\"TL GRN OFF\"}'\r\n");
        uart_puts(UART1_ID,"'{\"ack\":\"TL GRN OFF\"}'\r\n");
      }
      if (buffer == '0') {
        if (beeper_state == 0) {
          gpio_put(BEEPER, HIGH);
          beeper_state = 1;
         uart_puts(UART0_ID,"'{\"ack\":\"BEEP ON\"}'\r\n");
         uart_puts(UART1_ID,"'{\"ack\":\"BEEP ON\"}'\r\n");
        } else if (beeper_state == 1) {
          gpio_put(BEEPER, LOW);
          beeper_state = 0;
         uart_puts(UART0_ID,"'{\"ack\":\"BEEP OFF\"}'\r\n");
         uart_puts(UART1_ID,"'{\"ack\":\"BEEP OFF\"}'\r\n");
        }
      }
      if (buffer == 'U') {
         uart_puts(UART0_ID,"'{\"ack\":\"FIRMWARE UPLOAD\"}'\r\n");
         uart_puts(UART1_ID,"'{\"ack\":\"FIRMWARE UPLOAD\"}'\r\n");
        reset_usb_boot(0, 0);
      }
}

int main() {
  uart_init(UART0_ID, BAUD0_RATE);
  gpio_set_function(UART0_TX_PIN, UART0_TX_PIN);
  gpio_set_function(UART0_RX_PIN, UART0_RX_PIN);
  uart_set_hw_flow(UART0_ID, false, false);
  uart_set_format(UART0_ID, DATA0_BITS, STOP0_BITS, PARITY0);
  
  uart_init(UART1_ID, BAUD1_RATE);
  gpio_set_function(UART1_TX_PIN, UART1_TX_PIN);
  gpio_set_function(UART1_RX_PIN, UART1_RX_PIN);
  uart_set_hw_flow(UART1_ID, false, false);
  uart_set_format(UART1_ID, DATA1_BITS, STOP1_BITS, PARITY1);

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
  gpio_set_function(0, GPIO_FUNC_UART);
  gpio_set_function(1, GPIO_FUNC_UART);
  gpio_set_function(8, GPIO_FUNC_UART);
  gpio_set_function(9, GPIO_FUNC_UART);


  char buffer;
  char buffer1;

  //stdio_init_all();
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
        uart_puts(UART0_ID,"'{\"state\":\"BR DWN\"}'\r\n");
        uart_puts(UART1_ID,"'{\"state\":\"BR DWN\"}'\r\n");
      }
      if (current_barrier_state == 1) {
        uart_puts(UART0_ID,"'{\"state\":\"BR UP\"}'\r\n");
        uart_puts(UART1_ID,"'{\"state\":\"BR UP\"}'\r\n");
      }
      if (current_barrier_state == 2) {
        uart_puts(UART0_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
        uart_puts(UART1_ID,"'{\"state\":\"BR UNDEF\"}'\r\n");
      }
      previous_barrier_state = current_barrier_state;
    }

    while (uart_is_readable(uart0))  {
      buffer = uart_getc(uart0);
      respond(buffer);
    }
    while (uart_is_readable(uart1))  {
      buffer = uart_getc(uart1);
      respond(buffer);
    }

  }
  }

