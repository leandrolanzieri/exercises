/*
 * Copyright (C) 2025 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

#include "net/netdev.h"
#include "net/netif.h"

#include "net/gnrc/pktbuf.h"
#include "net/gnrc/pkt.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/netif/hdr.h"

#include "saul_reg.h"
#include "phydat.h"

#include "od.h"
#include "msg.h"
#include "thread.h"

#include "ztimer.h"

/* Interval between data transmissions, in seconds */
#define SEND_INTERVAL_SEC 10

/* Size of reception message queue */
#define RX_QUEUE_SIZE 8

/* Stack for reception thread */
static char _rx_thread_stack[THREAD_STACKSIZE_DEFAULT];

/* [TASK 3.3: Message queue for reception thread] */

/**
 * @brief   Find the LoRaWAN network interface in the registry.
 * @return Pointer to the LoRaWAN network interface, or NULL if not found.
 */
static netif_t *_find_lorawan_network_interface(void);

/**
 * @brief   Join the LoRaWAN network using OTAA.
 * @param   netif  Pointer to the LoRaWAN network interface.
 *
 * This function will attempt to join the LoRaWAN network using Over-The-Air
 * Activation (OTAA). It will keep retrying until a successful join is achieved.
 */
static void _join_lorawan_network(const netif_t *netif);

/**
 * @brief   Send a LoRaWAN packet with temperature data.
 * @param   netif       Pointer to the LoRaWAN network interface.
 * @param   temperature Pointer to the temperature data to be sent.
 *
 * @retval   0 on success
 * @retval  -1 on failure
 */
static int _send_lorawan_packet(const netif_t *netif, const phydat_t *temperature);

/**
 * @brief   Print to STDOUT the received packet.
 * @param   pkt  Pointer to the received packet.
 */
static void _print_received_packet(gnrc_pktsnip_t *pkt);

static netif_t *_find_lorawan_network_interface(void)
{
    /* [TASK 2.1: implement function to identify lorawan interface here]*/
}

static void _join_lorawan_network(const netif_t *netif)
{
    assert(netif != NULL);

    /* [TASK 2.2: implement join function here ]*/
}

static int _send_lorawan_packet(const netif_t *netif, const phydat_t *temperature)
{
    assert(netif != NULL);
    assert(temperature != NULL);

    int result;
    gnrc_pktsnip_t *packet;
    gnrc_pktsnip_t *header;
    gnrc_netif_hdr_t *netif_header;
    uint8_t address = 1;
    uint8_t data[2];

    /* [TASK 2.3] implement function to send data via lorawan */
}

void *rx_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    /* [TASK 3.4: initialize the message queue] */

    while (1) {
        /* [TASK 3.5: wait until we get a message]*/
    }

    /* never reached */
    return NULL;
}

static void _print_received_packet(gnrc_pktsnip_t *pkt)
{
    assert(pkt != NULL);

    gnrc_pktsnip_t *snip = pkt;

    /* [TASK 3.7: iterate over all packet snippets] */
}

int main(void)
{
    int result;
    netif_t *netif = NULL;

    /* Sleep so that we do not miss this message while connecting */
    ztimer_sleep(ZTIMER_SEC, 3);

    /* get the on-board temperature sensor */
    saul_reg_t *temp_sensor = saul_reg_find_type(SAUL_SENSE_TEMP);
    if (!temp_sensor) {
        puts("No temperature sensor present");
        return 1;
    }

    /* find the LoRaWAN network interface and connect */
    netif = _find_lorawan_network_interface();
    if (netif == NULL) {
        puts("No LoRaWAN network interface found");
        return -1;
    }

    _join_lorawan_network(netif);

    /* [TASK 3.1: create the reception thread] */

    /* [TASK 3.2: receive LoRaWAN packets in our reception thread] */

    /* record the starting time */
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_SEC);

    while (1) {
        /* read a temperature value from the sensor */
        phydat_t temperature;
        int dimensions = saul_reg_read(temp_sensor, &temperature);
        if (dimensions < 1) {
            puts("Error reading a value from the device");
            break;
        }

        /* dump the read value to STDIO */
        phydat_dump(&temperature, dimensions);

        /* [TASK 2.3: send sensor data via LoRaWAN ] */
        puts("Sending temperature data via LoRaWAN...");
        result = _send_lorawan_packet(netif, &temperature);
        if (result != 0) {
            puts("Failed to send LoRaWAN packet");
        } else {
            printf("Sent LoRaWAN packet successfully\n");
        }

        printf("%d\n", temperature.val[0]);

        /* wait a bit */
        printf("Waiting for %d seconds...\n", SEND_INTERVAL_SEC);
        ztimer_periodic_wakeup(ZTIMER_SEC, &last_wakeup, SEND_INTERVAL_SEC);
    }

    return 0;
}
