/*
 * Copyright (C) 2018 Kevin Mc Gee <>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for
 * more details.
 */
/////////////// Shell command for packet buffer statistics(wrapper) /////////////////
/////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "net/gnrc/netif.h"
#include "net/gnrc.h"

int _printBufferStatistics(int argc, char **argv){
	(void) argc;
    	(void) argv; //stops warning-error of unused variables

	//gnrc_pktbuf_stats();
	return 0;//
}
