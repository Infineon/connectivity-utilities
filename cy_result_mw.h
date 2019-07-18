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
#include "cy_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Each middleware module has been reserved with 128 error codes
 *
 * The expectation is that all middleware modules shall define their base out of this file so
 * that the error code space can be reserved and allotted efficiently
 *
 * !!! ALWAYS ADD MODULE BASES AT THE END. DO NOT INSERT NEW MODULES IN BETWEEN EXISTING MODULES !!!
 */
#define CY_RSLT_MODULE_MW_OFFSET 128

#define CY_RSLT_MODULE_MDNS_BASE                         CY_RSLT_MODULE_MIDDLEWARE_BASE  /* MDNS */
#define CY_RSLT_MODULE_AWS_BASE                          CY_RSLT_MODULE_MDNS_BASE + CY_RSLT_MODULE_MW_OFFSET /* AWS IoT */
#define CY_RSLT_MODULE_JSON_BASE                         CY_RSLT_MODULE_AWS_BASE + CY_RSLT_MODULE_MW_OFFSET /* JSON */
#define CY_RSLT_MODULE_LINKED_LIST_BASE                  CY_RSLT_MODULE_JSON_BASE + CY_RSLT_MODULE_MW_OFFSET /* linked list */
#define CY_RSLT_MODULE_COMMAND_CONSOLE_BASE              CY_RSLT_MODULE_LINKED_LIST_BASE + CY_RSLT_MODULE_MW_OFFSET /* command console */
#define CY_RSLT_MODULE_HTTP_SERVER                       CY_RSLT_MODULE_COMMAND_CONSOLE_BASE + CY_RSLT_MODULE_MW_OFFSET /* HTTP server */
#define CY_RSLT_MODULE_WIFI_SUPPLICANT_BASE              CY_RSLT_MODULE_HTTP_SERVER + CY_RSLT_MODULE_MW_OFFSET /* wifi supplicant */
#define CY_RSLT_MODULE_TCPIP_BASE                        CY_RSLT_MODULE_WIFI_SUPPLICANT_BASE + CY_RSLT_MODULE_MW_OFFSET /* TCPIP */
#define CY_RSLT_MODULE_MW_BASE                           CY_RSLT_MODULE_TCPIP_BASE + CY_RSLT_MODULE_MW_OFFSET
#define CY_RSLT_MODULE_TLS_BASE                          CY_RSLT_MODULE_MW_BASE + CY_RSLT_MODULE_MW_OFFSET


#define CY_RSLT_MW_ERROR                      ( CY_RSLT_MODULE_MW_BASE + 1 )
#define CY_RSLT_MW_TIMEOUT                    ( CY_RSLT_MODULE_MW_BASE + 2 )
#define CY_RSLT_MW_BADARG                     ( CY_RSLT_MODULE_MW_BASE + 3 )
#define CY_RSLT_MW_OUT_OF_HEAP_SPACE          ( CY_RSLT_MODULE_MW_BASE + 4 )
#define CY_RSLT_MW_PENDNG                     ( CY_RSLT_MODULE_MW_BASE + 5 )
#define CY_RSLT_MW_UNSUPPORTED                ( CY_RSLT_MODULE_MW_BASE + 6 )
#define CY_RSLT_MW_BUFFER_UNAVAIL_TEMPORARILY ( CY_RSLT_MODULE_MW_BASE + 7 )


#define CY_RSLT_TCPIP_ERROR                   ( CY_RSLT_MODULE_TCPIP_BASE + 1 )
#define CY_RSLT_TCPIP_TIMEOUT                 ( CY_RSLT_MODULE_TCPIP_BASE + 2 )
#define CY_RSLT_TCPIP_ERROR_NO_MEMORY         ( CY_RSLT_MODULE_TCPIP_BASE + 3 )
#define CY_RSLT_TCPIP_ERROR_SOCKET_OPEN       ( CY_RSLT_MODULE_TCPIP_BASE + 4 )
#define CY_RSLT_TCPIP_ERROR_SOCKET_BIND       ( CY_RSLT_MODULE_TCPIP_BASE + 5 )
#define CY_RSLT_TCPIP_ERROR_SOCKET_LISTEN     ( CY_RSLT_MODULE_TCPIP_BASE + 6 )
#define CY_RSLT_TCPIP_ERROR_SOCKET_ACCEPT     ( CY_RSLT_MODULE_TCPIP_BASE + 7 )
#define CY_RSLT_TCPIP_ERROR_TLS_OPERATION     ( CY_RSLT_MODULE_TCPIP_BASE + 8 )
#define CY_RSLT_TCPIP_ERROR_NO_MORE_SOCKET    ( CY_RSLT_MODULE_TCPIP_BASE + 9 )
#define CY_RSLT_TCPIP_ERROR_SEND              ( CY_RSLT_MODULE_TCPIP_BASE + 10)
#define CY_RSLT_TCPIP_ERROR_RECEIVE           ( CY_RSLT_MODULE_TCPIP_BASE + 11)


/* Various TLS error codes */
#define CY_RSLT_MODULE_TLS_ERROR              ( CY_RSLT_MODULE_TLS_BASE + 1 )
#define CY_RSLT_MODULE_TLS_TIMEOUT            ( CY_RSLT_MODULE_TLS_BASE + 2 )
#define CY_RSLT_MODULE_TLS_BADARG             ( CY_RSLT_MODULE_TLS_BASE + 3 )
#define CY_RSLT_MODULE_TLS_OUT_OF_HEAP_SPACE  ( CY_RSLT_MODULE_TLS_BASE + 4 )
#define CY_RSLT_MODULE_TLS_BAD_INPUT_DATA     ( CY_RSLT_MODULE_TLS_BASE + 5 )
#define CY_RSLT_MODULE_TLS_PARSE_KEY          ( CY_RSLT_MODULE_TLS_BASE + 6 )
#define CY_RSLT_MODULE_TLS_PARSE_CERTIFICATE  ( CY_RSLT_MODULE_TLS_BASE + 7 )
#define CY_RSLT_MODULE_TLS_UNSUPPORTED        ( CY_RSLT_MODULE_TLS_BASE + 8 )
#define CY_RSLT_MODULE_TLS_HANDSHAKE_FAILURE  ( CY_RSLT_MODULE_TLS_BASE + 9 )



#ifdef __cplusplus
} /*extern "C" */
#endif        

