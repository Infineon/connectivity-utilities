/*
 * Copyright 2019-2020 Cypress Semiconductor Corporation
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
 * @file
 * Cypress OS agnostic Worker Thread Utility
 */
/* To create a worker thread:
 * -------------------------
 *  Define your stack size (you probabaly want > 4k if you use printf)
 *
 * #define WORKER_THREAD_STACK_SIZE (6 * 1024)
 *
 *
 *  Define your stack (1 per worker thread).
 *    Global data - do not use stack/local variable
 *    If you do not provide a stack, one will be malloc'ed.
 *
 * MBED_ALIGN(8) uint8_t worker_thread_stack[WORKER_THREAD_STACK_SIZE];
 *
 *
 *  Define the storage structure (1 per worker thread)
 *    Global data - do not use stack/local variable
 *
 * cy_worker_thread_info_t worker_thread_info;
 *
 *
 *  Define the creation parameters (only used during create, they can be local)
 *
 * cy_worker_thread_params_t worker_thread_params =
 * {
 *     .priority   = CY_WORKER_THREAD_PRIORITY_NORMAL,  // If you set a lower priority, you mst cal osDelay() in higher threads
 *     .stack_size = WORKER_THREAD_STACK_SIZE,          // The stack size
 *     .stack      = worker_thread_stack,               // The provided Stack RAM
 *     .name       = "myWorker",                        // Thread Name (up to 16 chars)
 * };
 *
 *
 *  Define the worker function
 *
 * void my_worker_thread_function(void *arg)
 * {
 *      my_structure_t *my_data = (my_structure_t *)arg;
 *
 *      // Do your work here
 * }
 *
 *
 *  Create the worker thread
 *
 * cy_create_worker_thread( &worker_thread_params, &worker_thread_info );
 *
 *
 *  Defer work to the worker thread
 *
 * cy_defer_work(worker_thread_info, my_worker_thread_function, (void *)my_data);
 *
 *
 *  Delete the worker thread
 *
 * cy_delete_worker_thread( &worker_thread_info );
 *
 */
#ifndef CY_WORKER_THREAD_H
#define CY_WORKER_THREAD_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

#include "cyabs_rtos.h"

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************************************/
/** \addtogroup group_worker_defines *//** \{ */
/******************************************************************************/

#define CY_WORKER_VALID_TAG                    (0xAC0110CA)                 /**< Tag for validating worker thread info      */
#define CY_WORKER_MAX_NAME_LEN                 (16)                         /**< Max length of user name for worker thread  */
#define CY_WORKER_THREAD_DEFAULT_NAME          "CY Worker"                  /**< Default worker thread name                 */

#ifndef CY_WORKER_THREAD_DEFAULT_STACK_SIZE
#define CY_WORKER_THREAD_DEFAULT_STACK_SIZE    (6 * 1024)                   /**< Thread Default Stack size ( >4k needed for printf() usage) */
#endif

/** \} */
/******************************************************
 *                      Macros
 ******************************************************/

#define CY_WORKER_IS_THREAD_VALID(worker)      ( ((worker != 0) && (worker->tag == CY_WORKER_VALID_TAG) && (worker->event_queue != 0)) ? 1 : 0 )

/******************************************************
 *                   Function prototypes
 ******************************************************/

/** Worker thread function call prototype  */
typedef void (cy_do_work_func_t)(void *arg);

/******************************************************************************/
/** \addtogroup group_worker_enums *//** \{ */
/******************************************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/** Worker Thread Priority enums mapped to the OSes priorities */
typedef enum
{
    CY_WORKER_THREAD_PRIORITY_DEFAULT      = CY_RTOS_PRIORITY_BELOWNORMAL,  /**< default worker thread priority is CY_WORKER_THREAD_PRIORITY_BELOW_NORMAL */
    CY_WORKER_THREAD_PRIORITY_LOW          = CY_RTOS_PRIORITY_LOW,          /**< Lowest worker thread priority  */
    CY_WORKER_THREAD_PRIORITY_BELOW_NORMAL = CY_RTOS_PRIORITY_BELOWNORMAL,  /**< Low worker thread priority     */
    CY_WORKER_THREAD_PRIORITY_NORMAL       = CY_RTOS_PRIORITY_NORMAL,       /**< Normal worker thread priority  */
    CY_WORKER_THREAD_PRIORITY_ABOVE_NORMAL = CY_RTOS_PRIORITY_ABOVENORMAL,  /**< medium worker thread priority  */
    CY_WORKER_THREAD_PRIORITY_HIGH         = CY_RTOS_PRIORITY_HIGH,         /**< High worker thread priority    */

    CY_WORKER_THREAD_MAX_PRIORITY_LEVELS
} cy_worker_thread_priority_t;

/** \} */

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************************************/
/** \addtogroup group_worker_structures *//** \{ */
/******************************************************************************/

/** Worker Thread Parameters. */
typedef struct
{
    cy_worker_thread_priority_t     priority;   /**< Optional: Requested thread priority (0 to use default priority)            */
    uint32_t                        stack_size; /**< Optional: Size of stack for new thread (0 to use default settings)         */
    uint8_t                         *stack;     /**< Optional: pointer to stack provided (stack_size must be set if provided)   */
    const char                      *name;      /**< Optional: thread name                                                      */
} cy_worker_thread_params_t;

/** Worker Thread Information. */
typedef struct
{
    uint32_t            stack_size;                                         /**< Stack size                                 */
    unsigned char       *stack;                                             /**< Pointer to stack RAM                       */
    unsigned char       *stack_allocated;                                   /**< If != NULL, free upon thread exit          */
    void                *event_queue;                                       /**< mbedos EventQueue for this thread          */
    char                name[CY_WORKER_MAX_NAME_LEN];                       /**< Optional: Name from params                 */
    cy_thread_t         thread;                                             /**< Thread object */
    volatile uint32_t   tag;                                                /**< @ref CY_WORKER_VALID_TAG (other = invalid) */
 } cy_worker_thread_info_t;

/** \} */

/*****************************************************************************/
/**
 *
 *  @addtogroup group_worker_func
 *
 * Cypress OS agnostic Worker Thread Utility
 *
 *  @{
 */
/*****************************************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

/** Create worker thread to handle running callbacks in a separate thread.
 *
 * @param [in]  params      : pointer to requested parameters for starting worker thread
 * @param [out] new_worker  : pointer to cy_worker_thread_info_t structure to be filled when created
 *
 * @return  success : 0
 * @return  fail    : 1
 */
int cy_create_worker_thread( cy_worker_thread_params_t *params, cy_worker_thread_info_t *new_worker );

/** Delete worker thread.
 *
 * @param [in] old_worker: pointer to cy_worker_thread_info_t structure to be deleted
 *
 * @return  success : 0
 * @return  fail    : 1
 */
int cy_delete_worker_thread( cy_worker_thread_info_t *old_worker );

/** Defer Work to a Worker Thread.
 * Call the given function in the worker thread context.
 * NOTE: If the thread priority is below that of the current thread,
 *       you will want to call osDelay( msec ) in your main thread.
 *
 * @param  [in] worker_info : pointer to worker_thread used to run function
 * @param  [in] work_func   : function to run
 * @param  [in] arg         : opaque arg to be used in function call
 *
 * @return  success : 0
 * @return  fail    : 1
 */
int cy_defer_work(cy_worker_thread_info_t *worker_info, cy_do_work_func_t *work_func, void *arg);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* CY_WORKER_THREAD_H */

