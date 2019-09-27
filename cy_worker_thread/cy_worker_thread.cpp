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

/**
 * @file Cypress Worker Thread Utility
 */

#include "mbed.h"
#include "cyabs_rtos.h"
#include "events/EventQueue.h"
#include "cy_worker_thread.h"


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
 *               Variables Definitions
 ******************************************************/

static const cy_thread_priority_t priority_lookup_table[CY_WORKER_THREAD_MAX_PRIORITY_LEVELS] =
{
    [CY_WORKER_THREAD_PRIORITY_DEFAULT]        = CY_RTOS_PRIORITY_BELOWNORMAL,
    [CY_WORKER_THREAD_PRIORITY_LOW]            = CY_RTOS_PRIORITY_LOW,
    [CY_WORKER_THREAD_PRIORITY_BELOW_NORMAL]   = CY_RTOS_PRIORITY_BELOWNORMAL,
    [CY_WORKER_THREAD_PRIORITY_NORMAL]         = CY_RTOS_PRIORITY_NORMAL,
    [CY_WORKER_THREAD_PRIORITY_ABOVE_NORMAL]   = CY_RTOS_PRIORITY_ABOVENORMAL,
    [CY_WORKER_THREAD_PRIORITY_HIGH]           = CY_RTOS_PRIORITY_HIGH
};

/******************************************************
 *               Internal Functions
 ******************************************************/

/** Worker Thread to dispatch the events we add to the EventQueue
 *
 * @param   arg : pointer to @ref cy_worker_thread_info_t
 *
 * @return  NA
 */

static void cy_worker_thread_func( cy_thread_arg_t arg )
{
    cy_worker_thread_info_t *worker = (cy_worker_thread_info_t*)arg;

    if ( CY_WORKER_IS_THREAD_VALID(worker) )
    {
        /* the dispatch method executes events added with the EventQueue "call()" method */
        ((EventQueue *)worker->event_queue)->dispatch();
    }

    /* We are done, mark tag as 0 */
    worker->tag = 0;

    cy_rtos_exit_thread();
}

/******************************************************
 *               External Functions
 ******************************************************/

/* Cypress Worker Thread Initialization
 * Create an EventQueue and a Thread that we use for all callbacks
 *
 * @return  success : 0
 *          failure : 1
 */

int cy_create_worker_thread( cy_worker_thread_params_t *params, cy_worker_thread_info_t *new_worker )
{
    uint8_t *mem;
    cy_rslt_t result;

    /* sanity checks */
    if ((params == NULL) || (new_worker == NULL))
    {
        printf("%s() Bad ARGs param:%p info:%p \n", __func__, params, new_worker);
        return 1;
    }

    /* if we are already initialized, don't do it twice! */
    if ( CY_WORKER_IS_THREAD_VALID(new_worker) )
    {
        printf( "%s() Already Initialized !\n", __func__);
        return 0;
    }

    if ((params->stack != NULL) && (params->stack_size == 0))
    {
        printf( "%s() Stack Params error!\n", __func__);
        return 1;
    }

    /* start with a clean structure */
    memset(new_worker, 0, sizeof(cy_worker_thread_info_t));

    /* set default values */
    if (params->name != NULL)
    {
        strncpy(new_worker->name, params->name, sizeof(new_worker->name));
    }
    else
    {
        strncpy(new_worker->name, CY_WORKER_THREAD_DEFAULT_NAME, sizeof(new_worker->name));
    }
    new_worker->name[CY_WORKER_MAX_NAME_LEN - 1] = 0;
    new_worker->stack_size = (params->stack_size != 0 ) ? params->stack_size : CY_WORKER_THREAD_DEFAULT_STACK_SIZE;
    new_worker->stack     = params->stack;
    if (new_worker->stack == NULL)
    {
        /*
         * Allocate a stack buffer and make sure it is aligned on an 8 byte bounday.
         */

        mem = (unsigned char *)malloc(new_worker->stack_size + 7);
        if (mem == NULL)
        {
            printf( "%s() Stack malloc() failure !\n", __func__);
            return 1;
        }
        new_worker->stack = (uint8_t*)(((uint32_t)mem + 7) & ~7);
        new_worker->stack_allocated = mem;
    }

    /* Create an EventQueue so we can dispatch callbacks */
    new_worker->event_queue = (void *)new EventQueue();
    if (new_worker->event_queue == NULL)
    {
        printf( "%s() Create EventQueue Failed!\n", __func__);
        goto thread_create_failed;
    }

    /*
     * Create a thread to run the EventQueue as it blocks when dispatching.
     * Set tag before creating the thread
     */

    new_worker->tag = CY_WORKER_VALID_TAG;
    result = cy_rtos_create_thread(&new_worker->thread, cy_worker_thread_func,
                                   new_worker->name, new_worker->stack, new_worker->stack_size,
                                   priority_lookup_table[params->priority], (cy_thread_arg_t)new_worker);
    if (result != CY_RSLT_SUCCESS)
    {
        /* Thread create failed */
        new_worker->tag = 0;
        printf( "%s() Thread Create Failed!\n", __func__);
        goto thread_create_failed;
    }

    return 0;

thread_create_failed:
    /* Failed */
    /* free the stack memory */
    if (new_worker->stack_allocated != NULL)
    {
        free (new_worker->stack_allocated);
    }

    /* Delete the EventQueue */
    EventQueue *queue = (EventQueue *)new_worker->event_queue;
    new_worker->event_queue = NULL;
    if (queue != NULL)
    {
        delete queue;
    }

    memset (new_worker, 0, sizeof(cy_worker_thread_info_t));
    return 1;
}

/* Worker Thread De-Initialization
 * Delete the Thread and EventQueue we created for callbacks
 */

int cy_delete_worker_thread( cy_worker_thread_info_t *old_worker )
{
    if ( CY_WORKER_IS_THREAD_VALID(old_worker) )
    {
        EventQueue *queue = (EventQueue *)old_worker->event_queue;

        /* break the dispatch so the thread exits */
        queue->break_dispatch();

        /* Wait for thread to exit */
        while( old_worker->tag == CY_WORKER_VALID_TAG )
        {
            cy_rtos_delay_milliseconds(1);
        };

        old_worker->event_queue = NULL;

        /* wait for thread to exit */
        cy_rtos_join_thread(&old_worker->thread);

        /* delete the EventQueue */
        queue->~EventQueue();

        if (old_worker->stack_allocated != NULL)
        {
            free(old_worker->stack_allocated);
        }
        memset( old_worker, 0x00, sizeof(cy_worker_thread_info_t));
        return 0;
    }

    return 1;
}

/** Defer Work to a Worker Thread.
 * Call the given function in the worker thread context.
 *
 * @return  success : 0
 * @return  fail    : 1
 */
int cy_defer_work(cy_worker_thread_info_t *worker_info, cy_do_work_func_t *work_func, void *arg)
{
   if ((worker_info != NULL) && (worker_info->event_queue != NULL))
   {
       /* Queue an event to be run by the EventQueue dispatch() method */
       ((EventQueue *)worker_info->event_queue)->call(work_func, arg);
       return 0;
   }
   return 1;
}
