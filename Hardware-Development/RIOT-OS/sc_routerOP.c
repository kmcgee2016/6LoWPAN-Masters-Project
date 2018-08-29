/*
 * Copyright (C) 2018 Kevin Mc Gee <>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for
 * more details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifdef MODULE_GNRC_ICMPV6

#include "byteorder.h"
#include "net/gnrc/icmpv6.h"
#include "net/ipv6/addr.h"
#ifdef MODULE_GNRC_IPV6_NIB
#include "net/gnrc/ipv6/nib/nc.h"
#endif
#include "net/gnrc/ipv6/hdr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"
#include "thread.h"
#include "utlist.h"
#include "xtimer.h"

extern int _netif_stats(kernel_pid_t iface, unsigned module, bool reset);
bool stopTimer;

//this fcn. will periodically print out data about the device
int _router_output(int argc, char **argv){
	stopTimer = false;
	if(argc > 1){
		//in this case, there is a command added (start or stop)
		if(strcmp(argv[1], "stop") == 0 || strcmp(argv[1], "STOP") == 0){
			//break the timer loop
			stopTimer = true;
		}else{
			//unknown entry
			printf("Sorry, command arguments are 'STOP' or 'stop'\n");
		}
	}
	for(;;){
//Recent error here on example builds? - KMG 150718
		//do the print-out here:
		//I need - (Tx/Rx stuff), (buffers), (ect.)
		//gnrc_netif_t *netif = NULL;
        //while ((netif = gnrc_netif_iter(netif))) {
           // _netif_stats(netif->pid,NETSTATS_LAYER2,false);
        //}
        //lets bet the buffer stats aswell:
        //gnrc_pktbuf_stats();
        //printf(", | \n"); //just to be sure about parsing
		
		
		if(stopTimer)break; //just to allow the O/P to be stopped
		xtimer_sleep(5);
	}
	return 0;
}
#endif
