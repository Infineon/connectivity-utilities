## Introduction
The connectivity utilities library is a collection of general purpose middleware utilities. Several Cypress connectivity middleware libraries shall depend on this utilities library. 
Currently, the following utilities are included:

### JSON parser
The JSON format is often used for serializing and transmitting structured data over a network connection. It is used primarily to transmit data between a server and web application, serving as an alternative to XML. JSON is JavaScript Object Notation. The JSON parser utility library provides helper functions to parse JSON objects and calls the function registered by the caller

Refer to the [JSON.h](./JSON_parser/JSON.h) for API documentation

### Linked list
This is a generic linked list library with helper functions to add, insert, delete and find nodes in a list.

Refer to the [linked_list.h](./linked_list/linked_list.h) for API documentation

### String utilities
The string utilities module is a collection of string conversion helpers to convert between integer and strings.

Refer to the [cy_string_utils.h](./cy_string/cy_string_utils.h) for API documentation

### Network helpers
This is a collection of network helper functions to fetch IPv4 address of the local device, notify IPv4 address change via callback and conversion utilities.

Refer to the [nw_helper.h](./network/nw_helper.h) for API documentation

### Logging functions
This module is a logging subsystem that allows run time control for the logging level. Log messages are passed back to the application for output. A time callback can be provided by the application for the timestamp for each output line. Log messages are mutex protected across threads so that log messages do not interrupt each other.

Refer to the [cy_log.h](./cy_log/cy_log.h) for API documenmtation

### Worker thread
This is a worker thread utility to handle dedicated functionality.

Refer to the [cy_worker_thread.h](./cy_worker_thread/cy_worker_thread.h) for API documentation

### Middleware Error codes
The utilities library includes a collection of middleware error codes for various Cypress delivered middleware libraries such as AWS IoT, HTTP server, Enterprise security etc.

Refer to [cy_result_mw.h](./cy_result_mw.h) for details

## Supported platforms
This middleware library and it's features are supported on following Cypress platforms:
* [PSoC6 WiFi-BT Prototyping Kit (CY8CPROTO-062-4343W)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wi-fi-bt-prototyping-kit-cy8cproto-062-4343w)
* [PSoC6 WiFi-BT Pioneer Kit (CY8CKIT-062-WiFi-BT)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wifi-bt-pioneer-kit-cy8ckit-062-wifi-bt)
* CY8CKIT-062S2-43012

## Dependencies
This section provides the list of dependency libraries required for this middleware library to work.
* [ARM mbed-os stack version 5.15.0](https://os.mbed.com/mbed-os/releases)

## Additional Information
* [Connectivity Utilities RELEASE.md](./RELEASE.md)
* [Connectivity Utilities API reference guide](https://cypresssemiconductorco.github.io/connectivity-utilities/api_reference_manual/html/index.html)
* [Connectivity Utilities version](./version.txt)
