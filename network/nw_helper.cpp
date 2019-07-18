#include "mbed.h"

#include "lwip/ip4_addr.h"      // NOTE: LwIP specific - ip4_addr
#include "lwip/netif.h"         // NOTE: LwIP specific - for etharp_cleanup_netif()
#include "lwip/etharp.h"        // NOTE: LwIP specific - for netif_list for use in etharp_cleanup_netif() call

#include "nw_helper.h"
#include "NetworkInterface.h"
#include "WhdSTAInterface.h"
#include "ip4string.h"


/*
 * NOTE:
 * =====
 * This implementation is specific to mbed-os flavor.
 * The APIs defined in this file should be implemented for other OS/RTOS/Frameworks when it's getting ported on other frameworks.
 *
 */
static struct netif *nw_get_our_netif( WhdSTAInterface *iface )
{
    const char *our_ip_string;
    uint32_t our_ipv4addr;
    struct netif *netif;

    our_ip_string = iface->get_ip_address();
    if (our_ip_string == NULL)
    {
        return NULL;
    }

    stoip4( our_ip_string, strlen(our_ip_string), &our_ipv4addr);

    netif = netif_list;                     // NOTE: This is an LwIP global - refer but do not change
    while( netif != NULL)
    {
        if ( our_ipv4addr == netif_ip4_addr(netif)->addr )
        {
            return netif;
        }
        netif = netif->next;
    }
    return NULL;
}

static void nw_ip_status_change_handler(nw_ip_status_change_callback_t *cb, nsapi_event_t event, intptr_t val)
{
    if (event == NSAPI_EVENT_CONNECTION_STATUS_CHANGE)
    {
        (*cb->cb_func)(reinterpret_cast<nw_ip_interface_t>(cb->priv), cb->arg);
    }
}

void nw_ip_initialize_status_change_callback(nw_ip_status_change_callback_t *cb, nw_ip_status_change_callback_func_t *cb_func, void *arg)
{
    cb->cb_func = cb_func;
    cb->arg = arg;
    cb->priv = 0;
}

bool nw_ip_get_ipv4_address(nw_ip_interface_t nw_interface, nw_ip_address_t *ip_addr)
{
    NetworkInterface *iface = reinterpret_cast<NetworkInterface*>(nw_interface);

    const char *addr = iface->get_ip_address();
    if (ip_addr)
    {
        ip_addr->version = NW_IP_INVALID_IP;
        if (stoip4(addr, strlen(addr), (uint8_t *)&ip_addr->ip.v4))
        {
            ip_addr->version = NW_IP_IPV4;
            return true;
        }
    }
    return false;
}

void nw_ip_register_status_change_callback(nw_ip_interface_t nw_interface, nw_ip_status_change_callback_t *cb)
{
    NetworkInterface *iface = reinterpret_cast<NetworkInterface*>(nw_interface);

    cb->priv = iface;
    iface->add_event_listener(mbed::callback(nw_ip_status_change_handler, cb));
}

void nw_ip_unregister_status_change_callback(nw_ip_interface_t nw_interface, nw_ip_status_change_callback_t *cb)
{
    NetworkInterface *iface = reinterpret_cast<NetworkInterface*>(nw_interface);

    cb->priv = 0;
    iface->remove_event_listener(mbed::callback(nw_ip_status_change_handler, cb));
}

int nw_host_arp_cache_clear( nw_ip_interface_t iface )
{
    WhdSTAInterface *ifp =  (WhdSTAInterface *)iface;
    struct netif *netif;

    netif = nw_get_our_netif( ifp );
    if( netif != NULL)
    {
        etharp_cleanup_netif(netif);
    }

    return 0;
}

int nw_host_arp_cache_get_list( nw_ip_interface_t iface, nw_arp_cache_entry_t *list, uint32_t count, uint32_t *filled)
{
    uint8_t index;
    struct ip4_addr *ipaddr_ptr;
    struct netif *netif_ptr;
    struct eth_addr *eth_ret_ptr;
    struct netif *netif;
    WhdSTAInterface *ifp =  (WhdSTAInterface *)iface;

    if ((ifp == NULL) || (list == NULL) || (count == 0) || (filled == NULL))
    {
        return 1;
    }

    *filled = 0;

    netif = nw_get_our_netif( ifp );
    if( netif != NULL)
    {
        for (index =0 ; index < ARP_TABLE_SIZE; index++)
        {
            if (etharp_get_entry(index, &ipaddr_ptr, &netif_ptr, &eth_ret_ptr) == 1)
            {
                list[*filled].ip.version = NW_IP_IPV4;
                list[*filled].ip.ip.v4 = ipaddr_ptr->addr;
                memcpy(list[*filled].mac.mac, eth_ret_ptr->addr, sizeof(list->mac.mac));

                *filled = *filled + 1;
                if (*filled >= count)
                {
                    break;
                }
            }
            else
            {

            }
        }
        /* The query was successful */
        return 0;
    }

    return 1;
}

int nw_host_send_arp_request( nw_ip_interface_t iface, const char *ip_string)
{
    ip4_addr_t ipv4addr;
    struct netif *netif;
    WhdSTAInterface *ifp =  (WhdSTAInterface *)iface;

    stoip4(ip_string, strlen(ip_string), &ipv4addr.addr);

    netif = nw_get_our_netif( ifp );
    if( netif != NULL)
    {
        err_t err = etharp_request(netif, &ipv4addr);
        return err;
    }
    return 1;
}

uint32_t nw_get_time (void)
{
    /* Get Number of ticks per second */
    uint32_t tick_freq = osKernelGetTickFreq();

    /* Convert ticks count to time in milliseconds */
    return (osKernelGetTickCount() * (1000 / tick_freq) );
}

