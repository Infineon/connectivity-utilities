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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "cy_result_mw.h"
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/*
 * Results returned by linked list library
 */
#define CY_RSLT_MODULE_LINKED_LIST_ERR_CODE_START          (0)
#define CY_RSLT_LINKED_LIST_ERROR_BASE                     CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_LINKED_LIST_BASE, CY_RSLT_MODULE_LINKED_LIST_ERR_CODE_START)

#define CY_RSLT_LINKED_LIST_BADARG                         ((cy_rslt_t)(CY_RSLT_LINKED_LIST_ERROR_BASE + 1))
#define CY_RSLT_LINKED_LIST_NOT_FOUND                      ((cy_rslt_t)(CY_RSLT_LINKED_LIST_ERROR_BASE + 2))

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct linked_list_node cy_linked_list_node_t;

/******************************************************
 *                    Structures
 ******************************************************/

struct linked_list_node
{
    void*               data;
    cy_linked_list_node_t* next;
    cy_linked_list_node_t* prev;
};

typedef struct
{
    uint32_t            count;
    cy_linked_list_node_t* front;
    cy_linked_list_node_t* rear;
} cy_linked_list_t;

typedef bool (*cy_linked_list_compare_callback_t)( cy_linked_list_node_t* node_to_compare, void* user_data );

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

cy_rslt_t cy_linked_list_init( cy_linked_list_t* list );

cy_rslt_t cy_linked_list_deinit( cy_linked_list_t* list );

cy_rslt_t cy_linked_list_get_count( cy_linked_list_t* list, uint32_t* count );

cy_rslt_t cy_linked_list_set_node_data( cy_linked_list_node_t* node, const void* data );

cy_rslt_t cy_linked_list_get_front_node( cy_linked_list_t* list, cy_linked_list_node_t** front_node );

cy_rslt_t cy_linked_list_get_rear_node( cy_linked_list_t* list, cy_linked_list_node_t** rear_node );

cy_rslt_t cy_linked_list_find_node( cy_linked_list_t* list, cy_linked_list_compare_callback_t callback, void* user_data, cy_linked_list_node_t** node_found );

cy_rslt_t cy_linked_list_insert_node_at_front( cy_linked_list_t* list, cy_linked_list_node_t* node );

cy_rslt_t cy_linked_list_insert_node_at_rear( cy_linked_list_t* list, cy_linked_list_node_t* node );

cy_rslt_t cy_linked_list_insert_node_before( cy_linked_list_t* list, cy_linked_list_node_t* reference_node, cy_linked_list_node_t* node_to_insert );

cy_rslt_t cy_linked_list_insert_node_after( cy_linked_list_t* list, cy_linked_list_node_t* reference_node, cy_linked_list_node_t* node_to_insert );

cy_rslt_t cy_linked_list_remove_node( cy_linked_list_t* list, cy_linked_list_node_t* node );

cy_rslt_t cy_linked_list_remove_node_from_front( cy_linked_list_t* list, cy_linked_list_node_t** removed_node );

cy_rslt_t cy_linked_list_remove_node_from_rear( cy_linked_list_t* list, cy_linked_list_node_t** removed_node );

#ifdef __cplusplus
} /* extern "C" */
#endif
