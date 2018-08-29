
/*
 * Copyright (C) 2017 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    app_spectrum_scanner
 * @brief       Scanner application to find free channels
 * @{
 *
 * @file
 * @brief       Spectrum scanner application for RIOT
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 *
 * @}
 */

#include <stdint.h>
#include <math.h>

#include "fmt.h"
#include "thread.h"
#include "xtimer.h"
#include "net/ieee802154.h"
#include "net/gnrc.h"
//KMG
#include<net/gnrc/netapi.h>

/* Scanning interval */
#define INTERVAL (100U * US_PER_MS)

/**
 * @brief Measure the radio energy spectrum and print on stdout
 *
 * Algorithm description:
 *
 * The process will repeat as many measurements as possible during the
 * measurement interval, before the average power is computed. This reduces the
 * noise in the measurement and will yield a better image of what the radio
 * environment contains.
 *
 * Still, 122 measurements per second (frdm-kw41z) and 128 us per measurement
 * will only give a time coverage of about 1.5%, but because the measurements are
 * spread out over time they should still give a good representation of which
 * channels are free.
 *
 * Note that because the ED values are given in decibels, the average radio
 * power is not the same as the arithmetic mean of the ED measurements. To
 * compute the average of the dB measurements this algorithm requires both
 * logarithm and exponentiation, quite heavy operations on the kinds of CPUs
 * that RIOT targets. Increasing the CPU clock frequency may therefore reduce
 * the noise in the output, because of the more frequent energy measurements
 * possible.
 */
void spectrum_scanner(void)
{
    //kernel_pid_t ifs[GNRC_NETIF_NUMOF];
    size_t netif_numof = gnrc_netif_numof();

    /* Using expf(x) (natural exponent) gives quicker computations on Cortex-M0+,
     * compared to using powf(10, x). */
    /*
     * This was optimized by testing different combinations of expf, powf, logf, log10f:
     *
     * functions used | measurement iterations per 0.5 s on reference system (frdm-kw41z)
     * ------------------------------------------------------------------
     * expf, logf     | 64
     * powf, log10f   | 46
     * expf, log10f   | 61
     * no-op (baseline) | 83 (but the measurements are useless)
     */
	
    //uint64_t last_wakeup;
    uint16_t tmp_ch;
    uint64_t exeCount = 0;
    while(1) {
        /* Stack optimization, statically allocate this buffer */
        static float ed_average[GNRC_NETIF_NUMOF][IEEE802154_CHANNEL_MAX+1];
	exeCount++;
        memset(ed_average, 0, sizeof(ed_average));

        //last_wakeup = xtimer_now_usec64();
	//printf("last wakeup = %d",(int)last_wakeup);
        //uint64_t target = last_wakeup + INTERVAL;
        /* We spin and try to do as many measurements as possible in the
         * interval time */
		 
		/////////KMG///////////
		
		gnrc_netapi_opt_t setChannel;
		setChannel.opt = NETOPT_CHANNEL;
		setChannel.data = &tmp_ch;
		setChannel.data_len = sizeof(uint16_t);
		
		netopt_enable_t tmp;
		gnrc_netapi_opt_t invokeCCA;
		invokeCCA.opt = NETOPT_IS_CHANNEL_CLR;
		invokeCCA.data = &tmp;
		invokeCCA.data_len = sizeof(netopt_enable_t);
		
		int8_t level = 0;
		gnrc_netapi_opt_t getEnergy;
		getEnergy.opt = NETOPT_LAST_ED_LEVEL;
		getEnergy.data = &level;
		getEnergy.data_len = sizeof(int8_t);
		
		gnrc_netif_t* thisInterface = NULL;
		//unsigned int interfaceCount = gnrc_netif_numof();
        	unsigned int count = 0;
		unsigned int k = 0;
        //do {
            //for (k = 0; k < netif_numof; ++k) {
			//for(unsigned int k = 0; k < interfaceCount; ++k){	///KMG: Why did the previous code use '++k'???
                //kernel_pid_t dev = ifs[k];
				//
				thisInterface = gnrc_netif_iter(thisInterface);
                for (unsigned int ch = IEEE802154_CHANNEL_MIN; ch <= IEEE802154_CHANNEL_MAX; ++ch) {
                    tmp_ch = ch;
		    setChannel.data = &tmp_ch;
                    int res;
					//KMG: Set the channel number
                    //res = gnrc_netapi_set(dev, NETOPT_CHANNEL, 0, &tmp_ch, sizeof(uint16_t));
					/////
					res = gnrc_netif_set_from_netdev(thisInterface, &setChannel);
					////
                    if (res < 0) {
                        continue;
                    }
                    
                    /* Perform CCA to update ED level */
                    //res = gnrc_netapi_get(dev, NETOPT_IS_CHANNEL_CLR, 0, &tmp, sizeof(netopt_enable_t));
					////////
					res = gnrc_netif_get_from_netdev(thisInterface, &invokeCCA); //This was a GET in the old code => do the same???
                    if (res < 0) {
                        continue;
                    }
                    
					//KMG: Get LAST ED Result
                    //res = gnrc_netapi_get(dev, NETOPT_LAST_ED_LEVEL, 0, &level, sizeof(int8_t));
					/////
					res = gnrc_netif_get_from_netdev(thisInterface, &getEnergy);
                    if (res < 0) {
                        continue;
                    }
                    /* Convert dB to pseudo-energy before summing together the
                     * measurements. "Pseudo" because we use the natural
                     * exponential function e^x instead of computing 10^x which
                     * would be required if we needed the real measured energy.
                     * There is no need to know the real energy level because we
                     * will be converting back to dB again before printing. */
                    ed_average[k][ch] += expf((float)level / 128.f);
                }
            //}
            ++count;

            thread_yield();

        //} while(xtimer_now_usec64() < target);

        for (unsigned int k = 0; k < netif_numof; ++k) {
            print("[ Interface: ", 13);
            print_u32_dec(k);
            print(", Exe: ", 7);
            print_u64_dec(exeCount);
            //print(", ", 2);
            //print_u32_dec(count);
            print("> ", 2);
            for (unsigned int ch = IEEE802154_CHANNEL_MIN; ch <= IEEE802154_CHANNEL_MAX; ++ch) {

                /* Compute the average pseudo-energy and convert back to dB */

                ed_average[k][ch] = logf(ed_average[k][ch] / count) * 128.f;
                print_u32_dec(ch);
                print(": ", 2);
                print_float(ed_average[k][ch], 4);
                print(", ", 2);
            }
            print("]\n", 2);
        }

    }
}

int main(void)
{
    puts("RIOT scanner application");

    spectrum_scanner();
    return 0;
}

