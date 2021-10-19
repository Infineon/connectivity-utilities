# Connectivity Middleware Utilities

## What's Included?
Refer to the [README.md](./README.md) for a complete description of the utility libraries

## Changelog

### v3.0.3
* Added additonal enums in JSON structure
* Introduced module error base for PKCS11 integration
* Introduced ntoa and aton network utility helper functions for IPV4 and IPV6 addresses
* Documentation updates

### v3.0.2
* Renamed the error code module base for Enterprise Security library

### v3.0.1
* Introduced error code module base for HTTP Client library

#### v3.0.0
* Changes to adapt to AnyCloud's support for multiple WiFi interfaces - STA, softAP and concurrent STA+softAP mode

### v2.1.1
* Updates to support mbed-os 6.2 version

### v2.1.0
* Introduced user argument support in JSON callback function
* Introduced default output, and time functions support in cy-log

### v2.0.0
* Removed cy_worker_thread APIs which are now part of abstraction RTOS library
* Renamed header files and APIs to include 'cy' prefix
* Added MBED and LWIP component-specific implementation for nw-helper

### v1.1.2
* Removed the usage of APIs that are deprecated in Mbed OS version 5.15.0

### v1.1.1
* Added TCPIP error code for socket options
* DHCP client ARP offload fixes to the network helper utilities

### v1.1.0
* Added support for string conversion helper, logging functions, worker thread and network helper functions
* Tested with ARMmbed OS 5.14.0

### v1.0.0
* Initial release for ARMmbed OS
* Supports JSON_parser and linked list libraries

## Supported Software and Tools
The current version of the library was validated for compatibility with the following Software and Tools:

| Software and Tools                                        | Version |
| :---                                                      | :----:  |
| ModusToolbox&trade; Software Environment                  | 2.4     |
| - ModusToolbox&trade; Device Configurator                 | 3.10    |
| - ModusToolbox&trade; CapSense Configurator / Tuner tools | 4.0     |
| PSoC 6 Peripheral Driver Library (PDL)                    | 2.3.0   |
| GCC Compiler                                              | 10.3.1  |
| IAR Compiler                                              | 8.32    |
| Arm Compiler 6                                            | 6.14    |
| Mbed OS                                                   | 6.2.0   |
