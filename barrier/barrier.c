/**
 * Copyright (c) 2022 Dynamic IT Solutions CO., LTD
 * Serial Traffic Barrier and Traffic Light Controller
 * Suvir Kumar <suvir@dits.co.th>
 * version 0.1 updates 10/11/2022 at 12:57
 * Raspbery Pi Pico
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define LOW 0
#define HIGH 1

#define UART_0_ID uart0
#define BAUD_0_RATE 115200

#define UART_1_ID uart1
#define BAUD_1_RATE 115200

#define UART_0_TX_PIN 0
#define UART_0_RX_PIN 1

#define UART_1_TX_PIN 8
#define UART_1_RX_PIN 9

const char MenuString1[] = "\rDynamic IT Solutions Parking Barrier Controller\n";
const char MenuString2[] = "\rBy Suvir Kumar <suvir@dits.co.th>\n";
const char MenuString3[] = "\rMain Menu.\n";
const char MenuString4[] = "\n\rc Chk Comm\n\r? Clear Screen & Show Menu\n\r1 Toggle Onboard LED\n\r2 Barrier Up\n\r3 Barrier Down\n\r4 Barrier Toggle\n\r5 Barrier Position\n\r6 Red Light On\n\r7 Red Light Off\n\r8 Green Light On\n\r9 Green Light Off\n\r0 Beep\n\r\n";

const uint LED_PIN    = 25; //On Board LED (Output)
const uint ETT_PORT_0 = 16; //Barrier Up (Output Relay)
const uint ETT_PORT_1 = 17; //Barrier Down (Output Relay)
const uint ETT_PORT_2 = 18; //Push up, Push Down (Output Relay)
const uint ETT_PORT_3 = 19; //Barrier Position UP (Input Opto)
const uint ETT_PORT_4 = 20; //Barrier Position Down (Input Opto)
const uint ETT_PORT_5 = 21; //Relay Red Light 220V (Output)
const uint ETT_PORT_6 = 22; //Relay Green Light 220V (Output)
const uint ETT_PORT_7 = 28; //Beeper

const char RECVString[] = "\r\n:RX=";

const char CString[]        = "C!\n\r";
const char CLEARSCRString[] = "[2J";
const char _1String[]       = "1!\n\r";
const char _2String[]       = "2!\n\r";
const char _3String[]       = "3!\n\r";
const char _4String[]       = "4!\n\r";
const char _5String[]       = "5!\n\r";
const char _6String[]       = "6!\n\r";
const char _7String[]       = "7!\n\r";
const char _8String[]       = "8!\n\r";
const char _9String[]       = "9!\n\r";
const char _0String[]       = "0!\n\r";
const char INVString[]      = "INVALID!\n\r";
const char HeaderString[]   = "\r:TX=";
const char FooterString[]   = "!\n\r";
const char EventString[]    = "\r:EVENT=";
const char BarrierUp[]      = "$U!\n\r";
const char BarrierDown[]    = "$D!\n\r";

const uint BarrierUpCmd      = ETT_PORT_0;
const uint BarrierDownCmd    = ETT_PORT_1;
const uint BarrierPushBtnCmd = ETT_PORT_2;
const uint BarrierUpStatus   = ETT_PORT_3;
const uint BarrierDownStatus = ETT_PORT_4;
const uint TrafficLightRed   = ETT_PORT_5;
const uint TrafficLightGreen = ETT_PORT_6;
const uint Beeper            = ETT_PORT_7;

void sendBarrierPulse(uint BarrierCmd, int delayBefore, int delayAfter){
        gpio_put(BarrierCmd, HIGH);
        sleep_ms(delayBefore);
        gpio_put(BarrierCmd, LOW);
        sleep_ms(delayAfter);
	return;
}

void sendTrafficSignalOn(uint TrafficCmd){
        gpio_put(TrafficCmd, HIGH);
	return;
}

void sendTrafficSignalOff(uint TrafficCmd){
        gpio_put(TrafficCmd, LOW);
	return;
}

void showMenu(){
	uart_puts(UART_1_ID, MenuString1);
	uart_puts(UART_1_ID, MenuString2);
	uart_puts(UART_1_ID, MenuString3);
	uart_puts(UART_1_ID, MenuString4);
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

	//uart_putc_raw(UART_0_ID, 'UART_0_ID A');
	//uart_putc(UART_0_ID, 'UART_0_ID B');
	//uart_puts(UART_0_ID, "UART_0_ID Hello, UART!\n");

	showMenu();

    sleep_ms(4000);

    }



}

