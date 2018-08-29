/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       CoAP example server application (using nanocoap)
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @}
 */
/*
	KMG: 15/08/2018: This is a modified version of the nanocoap_server example found in the RIOT-OS examples folder
	[https://github.com/RIOT-OS/RIOT/tree/master/examples/nanocoap_server]
	-> The modifications made to this file include the GPIO initialisation and callback development 
	->Note: Effort was made to streamline this code by using a common callback for all common resources, however there were run-time
		issues with that approach. Therefore the prototype code was used in the final application
*/
#include <stdio.h>

#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define COAP_INBUF_SIZE (256U)

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* import "ifconfig" shell command, used for printing addresses */
extern int _gnrc_netif_config(int argc, char **argv);

/***************************************/
//KMG valve counts:
int digitalValues[6] = {0}; //valves and door
float extruderSpeed = -1;
int secondValue[5] = {0,0,0,0,0};
//gpio_t pin5 = GPIO_PIN(PA,22);//TODO
void valve1_handler(void* argv){
	//int * data = (int*)argv;
	//*data++;
	(*((int*)argv))++;
	secondValue[0]++;
	//puts("Valve 1 opened");
}
void valve2_handler(void* argv){
	(*((int*)argv))++;
	secondValue[1]++;
	//puts("Valve 2 opened");
}
void valve3_handler(void* argv){
	(*((int*)argv))++;
	secondValue[2]++;
	//puts("Valve 3 opened");
}
void valve4_handler(void* argv){
	(*((int*)argv))++;
	secondValue[3]++;
	//puts("Valve 4 opened");
}
/***************************************/
int main(void)
{
    puts("RIOT nanocoap example application");

    /* nanocoap_server uses gnrc sock which uses gnrc which needs a msg queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

	/**************************************************/
	//GPIO stuff(initialisation + interrupts)
	gpio_t pin1 = GPIO_PIN(PA,14);//TODO
	if(gpio_init_int(pin1, GPIO_IN_PD,GPIO_RISING, (gpio_cb_t)valve1_handler,(void*)(0)) != 0){
		puts("Falied to initialise GPIO interrupt 1\n");
	}
	gpio_t pin2 = GPIO_PIN(PA,18);//TODO
	if(gpio_init_int(pin2, GPIO_IN_PD,GPIO_RISING, (gpio_cb_t)valve2_handler,(void*)(0)) != 0){
		puts("Falied to initialise GPIO interrupt 2\n");
	}
	gpio_t pin3 = GPIO_PIN(PB,23);//TODO
	if(gpio_init_int(pin3, GPIO_IN_PD,GPIO_RISING, (gpio_cb_t)valve3_handler,(void*)(0)) != 0){
		puts("Falied to initialise GPIO interrupt 3\n");
	}
	gpio_t pin4 = GPIO_PIN(PA,15);//TODO
	if(gpio_init_int(pin4, GPIO_IN_PD,GPIO_RISING, (gpio_cb_t)valve4_handler,(void*)(0)) != 0){
		puts("Falied to initialise GPIO interrupt 4\n");
	}
	
	/**************************************************/
    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    /* print network addresses */
    puts("Configured network interfaces:");
    _gnrc_netif_config(0, NULL);

    /* initialize nanocoap server instance */
    uint8_t buf[COAP_INBUF_SIZE];
    sock_udp_ep_t local = { .port=COAP_PORT, .family=AF_INET6 };
    nanocoap_server(&local, buf, sizeof(buf));
	
    /* should be never reached */
    return 0;
}
