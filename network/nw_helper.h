/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>


#if defined(__cplusplus)
extern "C" {
#endif

#define cy_assert( error_string, assertion )         do { (void)(assertion); } while(0)

typedef uintptr_t nw_ip_interface_t;

typedef void (nw_ip_status_change_callback_func_t)(nw_ip_interface_t iface, void *arg);

typedef struct nw_ip_status_change_callback
{
    nw_ip_status_change_callback_func_t *cb_func;
    void *arg;
    void *priv;
} nw_ip_status_change_callback_t;

typedef enum nw_ip_version
{
    NW_IP_IPV4 = 4,
    NW_IP_IPV6 = 6,
    NW_IP_INVALID_IP = (-1),
} nw_ip_version_t;

/*
 * Network interface type
 */
typedef enum
{
    CY_NW_INF_TYPE_WIFI = 0,
    CY_NW_INF_TYPE_ETH
} cy_network_interface_type_t;

typedef struct nw_ip_address
{
    nw_ip_version_t version;

    union
    {
        uint32_t v4;
        uint32_t v6[4];
    } ip;
} nw_ip_address_t;

typedef void* cy_network_interface_object_t;

/** MAC Address info */
typedef struct nw_ip_mac
{
    uint8_t    mac[6];              /**< MAC address                */
} nw_ip_mac_t;

/** ARP Cache Entry info */
typedef struct nw_arp_cache_entry
{
    nw_ip_address_t    ip;         /**< IP address                 */
    nw_ip_mac_t        mac;        /**< MAC address                */
} nw_arp_cache_entry_t;

/*
 * Network interface info structure
 */
typedef struct
{
    cy_network_interface_type_t     type;
    cy_network_interface_object_t   object;                             /* Pointer to the network interface object */
} cy_network_interface_t;

/** Initialize status change callback
 *
 * Initialize @ref nw_ip_status_change_callback_t instead of
 * directly manipulating the callback struct.
 */
void nw_ip_initialize_status_change_callback(nw_ip_status_change_callback_t *, nw_ip_status_change_callback_func_t *, void *arg);

/** Retrieves the IPv4 address for an interface
 *
 * Retrieves the IPv4 address for an interface (AP or STA) if it
 * exists.
 */
bool nw_ip_get_ipv4_address(nw_ip_interface_t, nw_ip_address_t *);

/** Registers a callback function that gets called when the IP address has changed
 *
 * Registers a callback function that gets called when the IP address has changed
 */
void nw_ip_register_status_change_callback(nw_ip_interface_t, nw_ip_status_change_callback_t *);

/** Un-registers a callback function that gets called when the IP address has changed
 */
void nw_ip_unregister_status_change_callback(nw_ip_interface_t, nw_ip_status_change_callback_t *);

/** Clear Host Network Stack ARP Cache
 * NOTE: in LwIP, we need the netif (NetworkInterface) to do things, we can find using wifi interface.
 *
 * @param [in]  iface   - WiFi Interface to act on
 *
 *  @return 0 - success
 *          1 - fail
 */
int nw_host_arp_cache_clear( nw_ip_interface_t iface );

/** Get Host Network Stack ARP Cache List
 * NOTE: in LwIP, we need the netif (NetworkInterface) to do things, we can find using wifi interface.
 *
 * @param [in]  iface   - WiFi Interface to act on
 * @param [out] list    - Pointer to nw_arp_cache_entry_t array
 * @param [in]  count   - Number of entries available in list
 * @param [out] filled  - Number of entries filled
 *
 *  @return 0 - success
 *          1 - fail
 */
int nw_host_arp_cache_get_list( nw_ip_interface_t iface, nw_arp_cache_entry_t *list, uint32_t count, uint32_t *filled );

/** Send ARP Request
 * NOTE: in LwIP, we need the netif (NetworkInterface) to do things, we can find using wifi interface.
 *
 * @param [in]  iface       - WiFi Interface to act on
 * @param [in]  ip_string   - Address to ping (ex: "192.168.1.1")
 *
 * @return 0 - success
 *         1 - failed to send ARP request
 */
int nw_host_send_arp_request( nw_ip_interface_t iface, const char *ip_string );

/* GET time in milliseconds
 * @return time in milliseconds
 * */
uint32_t nw_get_time (void);

#if defined(__cplusplus)
}
#endif
