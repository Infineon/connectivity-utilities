/*
 * Copyright 2019-2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 * 
 * Linked list helper library
 */

#include "string.h"
#include "cy_linked_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

cy_rslt_t cy_linked_list_init( cy_linked_list_t* list )
{
    if ( list == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    memset( list, 0, sizeof( *list ) );
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_deinit( cy_linked_list_t* list )
{
    cy_linked_list_node_t* current;

    if ( list == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    /* Traverse through all nodes and detach them */
    current = list->front;

    while ( current != NULL )
    {
        cy_linked_list_node_t* next = current->next;

        /* Detach node from the list */
        current->prev = NULL;
        current->next = NULL;

        /* Move to the next node */
        current = next;
    }

    memset( list, 0, sizeof( *list ) );
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_get_count( cy_linked_list_t* list, uint32_t* count )
{
    if ( list == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    *count = list->count;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_set_node_data( cy_linked_list_node_t* node, const void* data )
{
    if ( node == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    node->data = (void*)data;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_get_front_node( cy_linked_list_t* list, cy_linked_list_node_t** front_node )
{
    if ( list == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        *front_node = NULL;
        return CY_RSLT_LINKED_LIST_NOT_FOUND;
    }

    *front_node = list->front;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_get_rear_node( cy_linked_list_t* list, cy_linked_list_node_t** rear_node )
{
    if ( list == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        *rear_node = NULL;
        return CY_RSLT_LINKED_LIST_NOT_FOUND;
    }

    *rear_node = list->rear;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_find_node( cy_linked_list_t* list, cy_linked_list_compare_callback_t callback, void* user_data, cy_linked_list_node_t** node_found )
{
    cy_linked_list_node_t* current;

    if ( list == NULL || callback == NULL || node_found == NULL || list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    current = list->front;

    while ( current != NULL )
    {
        if ( callback( current, user_data ) == true )
        {
            *node_found = current;
            return CY_RSLT_SUCCESS;
        }

        current = current->next;
    }

    return CY_RSLT_LINKED_LIST_NOT_FOUND;
}

cy_rslt_t cy_linked_list_insert_node_at_front( cy_linked_list_t* list, cy_linked_list_node_t* node )
{
    if ( list == NULL || node == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        list->front = node;
        list->rear  = node;
        node->prev  = NULL;
        node->next  = NULL;
    }
    else
    {
        node->prev        = NULL;
        node->next        = list->front;
        list->front->prev = node;
        list->front       = node;
    }

    list->count++;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_insert_node_at_rear( cy_linked_list_t* list, cy_linked_list_node_t* node )
{
    if ( list == NULL || node == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        list->front = node;
        list->rear  = node;
        node->prev  = NULL;
        node->next  = NULL;
    }
    else
    {
        node->next       = NULL;
        node->prev       = list->rear;
        list->rear->next = node;
        list->rear       = node;
    }

    list->count++;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_insert_node_before( cy_linked_list_t* list, cy_linked_list_node_t* reference_node, cy_linked_list_node_t* node_to_insert )
{
    /* WARNING: User must make sure that reference_node is in the list */
    if ( list == NULL || reference_node == NULL || node_to_insert == NULL || list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_NOT_FOUND;
    }

    if ( reference_node == list->front )
    {
        return cy_linked_list_insert_node_at_front( list, node_to_insert );
    }
    else
    {
        node_to_insert->prev       = reference_node->prev;
        node_to_insert->prev->next = node_to_insert;
        node_to_insert->next       = reference_node;
        reference_node->prev       = node_to_insert;

        list->count++;

        return CY_RSLT_SUCCESS;
    }
}

cy_rslt_t cy_linked_list_insert_node_after( cy_linked_list_t* list, cy_linked_list_node_t* reference_node, cy_linked_list_node_t* node_to_insert )
{
    /* WARNING: User must make sure that reference_node is in the list */
    if ( list == NULL || reference_node == NULL || node_to_insert == NULL || list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( reference_node == list->rear )
    {
        return cy_linked_list_insert_node_at_rear( list, node_to_insert );
    }
    else
    {
        node_to_insert->prev       = reference_node;
        node_to_insert->next       = reference_node->next;
        reference_node->next->prev = node_to_insert;
        reference_node->next       = node_to_insert;

        list->count++;

        return CY_RSLT_SUCCESS;
    }
}

cy_rslt_t cy_linked_list_remove_node( cy_linked_list_t* list, cy_linked_list_node_t* node )
{
    /* WARNING: User must make sure that node to remove is in the list */
    if ( list == NULL || node == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_NOT_FOUND;
    }

    if ( list->count == 1 )
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else if ( node == list->front )
    {
        cy_linked_list_node_t* removed_node;

        return cy_linked_list_remove_node_from_front( list, &removed_node );
    }
    else if ( node == list->rear )
    {
        cy_linked_list_node_t* removed_node;

        return cy_linked_list_remove_node_from_rear( list, &removed_node );
    }
    else
    {
        /* Check for possible 'null' pointer exception, when cy_linked_list_remove_node() is called with the same 'node' for more than once */
        if (node->prev == NULL || node->next == NULL)
        {
            return CY_RSLT_LINKED_LIST_BADARG;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    /* Make sure that detach node does not point to some arbitrary memory location */
    node->prev = NULL;
    node->next = NULL;

    list->count--;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_remove_node_from_front( cy_linked_list_t* list, cy_linked_list_node_t** removed_node )
{
    if ( list == NULL || removed_node == NULL || list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    *removed_node = list->front;

    if ( list->count == 1 )
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else
    {
        list->front       = list->front->next;
        list->front->prev = NULL;
    }

    /* Make sure node does not point to some arbitrary memory location */
    (*removed_node)->prev = NULL;
    (*removed_node)->next = NULL;

    list->count--;

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_linked_list_remove_node_from_rear( cy_linked_list_t* list, cy_linked_list_node_t** removed_node )
{
    if ( list == NULL || removed_node == NULL )
    {
        return CY_RSLT_LINKED_LIST_BADARG;
    }

    if ( list->count == 0 )
    {
        return CY_RSLT_LINKED_LIST_NOT_FOUND;
    }

    *removed_node = list->rear;

    if ( list->count == 1 )
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else
    {
        list->rear       = list->rear->prev;
        list->rear->next = NULL;
    }

    /* Make sure node does not point to some arbitrary memory location */
    (*removed_node)->prev = NULL;
    (*removed_node)->next = NULL;

    list->count--;

    return CY_RSLT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
