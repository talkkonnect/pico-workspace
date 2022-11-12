/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Serial Traffic Barrier and Traffic Light Controller
 * Suvir Kumar <suvir@dits.co.th>
 * version 0.1 updates 10/11/2022 at 12:57
 * Raspbery Pi Pico
 */

#include "hardware/uart.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#define LOW 0
#define HIGH 1

const uint LED_PIN = 25;    // On Board LED (Output)
const uint ETT_PORT_0 = 16; // Barrier Up (Output Relay)
const uint ETT_PORT_1 = 17; // Barrier Down (Output Relay)
const uint ETT_PORT_2 = 18; // Push up, Push Down (Output Relay)
const uint ETT_PORT_3 = 19; // Barrier Position UP (Input Opto)
const uint ETT_PORT_4 = 20; // Barrier Position Down (Input Opto)
const uint ETT_PORT_5 = 21; // Relay Red Light 220V (Output)
const uint ETT_PORT_6 = 22; // Relay Green Light 220V (Output)
const uint ETT_PORT_7 = 28; // Beeper


const uint BarrierUpCmd = ETT_PORT_0;
const uint BarrierDownCmd = ETT_PORT_1;
const uint BarrierPushBtnCmd = ETT_PORT_2;
const uint BarrierUpSignal = ETT_PORT_3;
const uint BarrierDownSignal = ETT_PORT_4;
const uint TrafficLightRed = ETT_PORT_5;
const uint TrafficLightGreen = ETT_PORT_6;
const uint Beeper = ETT_PORT_7;

void sendBarrierPulse(uint BarrierCmd, int delayBefore, int delayAfter) {
  gpio_put(BarrierCmd, HIGH);
  sleep_ms(delayBefore);
  gpio_put(BarrierCmd, LOW);
  sleep_ms(delayAfter);
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
  printf("\r\nMain Menu.\n");
  printf("\n\rc Chk Comm\n\r? Clear Screen & Show Menu\n\r1 Toggle Onboard "
         "LED\n\r2 Barrier Up\n\r3 Barrier Down\n\r4 Barrier Toggle\n\r5 "
         "Barrier Position\n\r6 Red Light On\n\r7 Red Light Off\n\r8 Green "
         "Light On\n\r9 Green Light Off\n\r0 Beep\n\r\n");
  return;
}

int main() {
  int onboard_led = 0;
  int beeper_state = 0;

  gpio_init(LED_PIN);
  gpio_init(BarrierUpCmd);
  gpio_init(BarrierDownCmd);
  gpio_init(BarrierPushBtnCmd);
  gpio_init(BarrierUpSignal);
  gpio_init(BarrierDownSignal);
  gpio_init(TrafficLightRed);
  gpio_init(TrafficLightGreen);
  gpio_init(Beeper);

  gpio_set_dir(LED_PIN, GPIO_OUT);

  gpio_set_dir(BarrierUpCmd, GPIO_OUT);

  gpio_set_dir(BarrierDownCmd, GPIO_OUT);

  gpio_set_dir(BarrierPushBtnCmd, GPIO_OUT);

  gpio_set_dir(BarrierUpSignal, GPIO_IN);
  gpio_pull_up(BarrierUpSignal);

  gpio_set_dir(BarrierDownSignal, GPIO_IN);
  gpio_pull_up(BarrierDownSignal);

  gpio_set_dir(TrafficLightRed, GPIO_OUT);

  gpio_set_dir(TrafficLightGreen, GPIO_OUT);

  gpio_set_dir(Beeper, GPIO_OUT);

  char buffer[1024];

  stdio_init_all();
  showMenu();

  while (true) {
    scanf("%1024s", buffer);

    if (strcmp(buffer, "c") == 0) {
      printf(":COMM OK$\n");
    }
    if (strcmp(buffer, "?") == 0) {
      printf("%c%c%c%c", 0x1B, 0x5B, 0x32, 0x4A);
      showMenu();
    }
    if (strcmp(buffer, "1") == 0) {
      if (onboard_led == 0) {
        gpio_put(LED_PIN, HIGH);
        onboard_led = 1;
      } else if (onboard_led == 1) {
        gpio_put(LED_PIN, LOW);
        onboard_led = 0;
      }
    }
    if (strcmp(buffer, "2") == 0) {
      sendBarrierPulse(BarrierUpCmd, 1000, 2000);
    }
    if (strcmp(buffer, "3") == 0) {
      sendBarrierPulse(BarrierDownCmd, 1000, 2000);
    }
    if (strcmp(buffer, "4") == 0) {
      sendBarrierPulse(BarrierPushBtnCmd, 1000, 2000);
    }
    if (strcmp(buffer, "5") == 0) {
      if (gpio_get(BarrierUpSignal) == 0 && gpio_get(BarrierDownSignal) == 0) {
        printf("Barrier Undetermined State!\n\n");
      }
      if (gpio_get(BarrierUpSignal) == 0 && gpio_get(BarrierDownSignal) == 1) {
        printf("Barrier is Down!\n\n");
      }
      if (gpio_get(BarrierUpSignal) == 1 && gpio_get(BarrierDownSignal) == 0) {
        printf("Barrier is Up!\n\n");
      }
      if (gpio_get(BarrierUpSignal) == 1 && gpio_get(BarrierDownSignal) == 1) {
        printf("Barrier Undetermined State!\n\n");
      }
    }
    if (strcmp(buffer, "6") == 0) {
      sendTrafficSignalOn(TrafficLightRed);
    }
    if (strcmp(buffer, "7") == 0) {
      sendTrafficSignalOff(TrafficLightRed);
    }
    if (strcmp(buffer, "8") == 0) {
      sendTrafficSignalOn(TrafficLightGreen);
    }
    if (strcmp(buffer, "9") == 0) {
      sendTrafficSignalOff(TrafficLightGreen);
    }
    if (strcmp(buffer, "0") == 0) {
      if (beeper_state == 0) {
        gpio_put(Beeper, HIGH);
        beeper_state = 1;
      } else if (beeper_state == 1) {
        gpio_put(Beeper, LOW);
        beeper_state = 0;
      }
    }

    if (strcmp(buffer, "PROGRAM") == 0) {
      reset_usb_boot(0, 0);
    }
  }
}
