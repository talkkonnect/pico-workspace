/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Serial Traffic Barrier and Traffic Light Controller
 * Suvir Kumar <suvir@dits.co.th>
 * version 0.1 updates 10/11/2022 at 12:57
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_0_ID uart0
#define BAUD_0_RATE 115200

#define UART_1_ID uart1
#define BAUD_1_RATE 115200

#define UART_0_TX_PIN 0
#define UART_0_RX_PIN 1

#define UART_1_TX_PIN 8
#define UART_1_RX_PIN 9


const uint LED_PIN    = 25; //On Board LED (Output)
const uint ETT_PORT_0 = 16; //Barrier Up (Output Relay)
const uint ETT_PORT_1 = 17; //Barrier Down (Output Relay)
const uint ETT_PORT_2 = 18; //Push up, Push Down (Output Relay)
const uint ETT_PORT_3 = 19; //Barrier Position UP (Input Opto)
const uint ETT_PORT_4 = 20; //Barrier Position Down (Input Opto)
const uint ETT_PORT_5 = 21; //Relay Red Light 220V (Output)
const uint ETT_PORT_6 = 22; //Relay Green Light 220V (Output)
const uint ETT_PORT_7 = 28; //Beeper

const uint BarrierUpCmd      = ETT_PORT_0;
const uint BarrierDownCmd    = ETT_PORT_1;
const uint BarrierPushBtnCmd = ETT_PORT_2;
const uint BarrierUpStatus   = ETT_PORT_3;
const uint BarrierDownStatus = ETT_PORT_4;
const uint TrafficLightRed   = ETT_PORT_5;
const uint TrafficLightGreen = ETT_PORT_6;
const uint Beeper            = ETT_PORT_7;

void sendBarrierPulse(uint BarrierCmd, int delayBefore, int delayAfter){
        gpio_put(BarrierCmd, 1);
        sleep_ms(delayBefore);
        gpio_put(BarrierCmd, 0);
        sleep_ms(delayAfter);
	return;
}

void sendTrafficSignalOn(uint TrafficCmd){
        gpio_put(TrafficCmd, 1);
	return;
}

void sendTrafficSignalOff(uint TrafficCmd){
        gpio_put(TrafficCmd, 0);
	return;
}

int main() {
	gpio_init(LED_PIN);
	gpio_init(BarrierUpCmd);
	gpio_init(BarrierDownCmd);
	gpio_init(BarrierPushBtnCmd);
	gpio_init(BarrierUpStatus);
	gpio_init(BarrierDownStatus);
	gpio_init(TrafficLightRed);
	gpio_init(TrafficLightGreen);
	gpio_init(Beeper);

	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_set_dir(BarrierUpCmd, GPIO_OUT);
	gpio_set_dir(BarrierDownCmd, GPIO_OUT);
	gpio_set_dir(BarrierPushBtnCmd, GPIO_OUT);
	gpio_set_dir(BarrierUpStatus, GPIO_IN);
	gpio_set_dir(BarrierDownStatus, GPIO_IN);
	gpio_set_dir(TrafficLightRed, GPIO_OUT);
 	gpio_set_dir(TrafficLightGreen, GPIO_OUT);
	gpio_set_dir(Beeper, GPIO_OUT);

	uart_init(UART_0_ID, BAUD_0_RATE);
	gpio_set_function(UART_0_TX_PIN, GPIO_FUNC_UART);
    	gpio_set_function(UART_0_RX_PIN, GPIO_FUNC_UART);

	uart_init(UART_1_ID, BAUD_1_RATE);
	gpio_set_function(UART_1_TX_PIN, GPIO_FUNC_UART);
    	gpio_set_function(UART_1_RX_PIN, GPIO_FUNC_UART);

    while (true) {
        sleep_ms(4000);

	sendBarrierPulse(BarrierUpCmd, 1000,2000);
	//sendBarrierPulse(BarrierDownCmd, 1000,2000);
	//sendBarrierPulse(BarrierPushBtnCmd, 1000,2000);
	//sendTrafficSignalOn(TrafficLightRed);
	//sendTrafficSignalOff(TrafficLightRed);
	//sendTrafficSignalOn(TrafficLightGreen);
	//sendTrafficSignalOff(TrafficLightGreen);

//	uart_putc_raw(UART_0_ID, 'UART_0_ID A');
//  	uart_putc(UART_0_ID, 'UART_0_ID B');
//	uart_puts(UART_0_ID, "UART_0_ID Hello, UART!\n");

	uart_putc_raw(UART_1_ID, 'UART_1_ID A');
    	uart_putc(UART_1_ID, 'UART_1_ID B');
    	uart_puts(UART_1_ID, "UART_1_ID Hello, UART!\n");
        sleep_ms(4000);
    }
}

