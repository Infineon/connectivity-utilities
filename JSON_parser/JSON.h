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
#include "cy_result_mw.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/*
 * Results returned by JSON library
 */
#define CY_RSLT_MODULE_JSON_ERR_CODE_START          (0)
#define CY_RSLT_JSON_ERROR_BASE                     CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_JSON_BASE, CY_RSLT_MODULE_JSON_ERR_CODE_START)

#define CY_RSLT_JSON_GENERIC_ERROR                  ((cy_rslt_t)(CY_RSLT_JSON_ERROR_BASE + 1))


/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    JSON_STRING_TYPE,
    JSON_NUMBER_TYPE,
    JSON_VALUE_TYPE,
    JSON_ARRAY_TYPE,
    JSON_OBJECT_TYPE,
    JSON_BOOLEAN_TYPE,
    JSON_NULL_TYPE,
    UNKNOWN_JSON_TYPE
} cy_JSON_type_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/
#define OBJECT_START_TOKEN        '{'
#define OBJECT_END_TOKEN          '}'

#define ARRAY_START_TOKEN         '['
#define ARRAY_END_TOKEN           ']'

#define STRING_TOKEN              '"'

#define START_OF_VALUE            ':'

#define COMMA_SEPARATOR           ','

#define ESCAPE_TOKEN              '\\'

#define TRUE_TOKEN                't'

#define FALSE_TOKEN               'f'

#define NULL_TOKEN                'n'
/******************************************************
 *                    Structures
 ******************************************************/
typedef struct cy_JSON_object {

    char*               object_string;
    uint8_t             object_string_length;
    cy_JSON_type_t      value_type;
    char*               value;
    uint16_t            value_length;
    struct cy_JSON_object* parent_object;
} cy_JSON_object_t;

#define JSON_IS_NOT_ESC_CHAR( ch ) ( ( ch != '\b' )  &&  \
                                     ( ch != '\f' ) &&  \
                                     ( ch != '\n' ) &&  \
                                     ( ch != '\r' ) &&  \
                                     ( ch != '\t' ) &&  \
                                     ( ch != '\"' ) &&  \
                                     ( ch != '\\' ) )

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
typedef cy_rslt_t (*cy_JSON_callback_t)( cy_JSON_object_t* json_object );

/** Register callback to be used by JSON parser
 *
 * @param[in] json_callback                  Callback to be called when JSON parser encounters an objects value. The callback will
 *                                           return the cy_JSON_object_type, giving the object string name,value type the object
 *                                           and the value.
 *
 * @return @ref cy_rslt_t
 */
cy_rslt_t cy_JSON_parser_register_callback( cy_JSON_callback_t json_callback );

/** Returns the current callback function registered with by JSON parser
 *
 * @return @ref cy_JSON_callback_t
 */
cy_JSON_callback_t cy_JSON_parser_get_callback( void );

/** Parse JSON input string.
 *  This function will parse the JSON input string through a single parse, calling a callback whenever it encounters milestones
 *  an object, passing in object name, json value type, and a value (if value is string or number )
 *
 * @param[in] json_input   : JSON input array

 * @param[in] input_length : Length of JSON input
 *
 * @return @ref cy_rslt_t
 *
 * @note: Currently escape values are not supported.
 */
cy_rslt_t cy_JSON_parser( const char* json_input, uint32_t input_length );


#ifdef __cplusplus
} /*extern "C" */
#endif
