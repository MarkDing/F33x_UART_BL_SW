//-----------------------------------------------------------------------------
// Fxxx_TargetBL_Config.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
//
//
// Target:         Any Silicon Labs C8051 MCU
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.1 / 9Mar2013 (MarkD)
//    -F330 UART Revision
// Release 1.0 / 01SEP2010 (PKC)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#ifndef _FXXX_TARGETBL_CONFIG_H_
#define _FXXX_TARGETBL_CONFIG_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS - Available to all modules
//-----------------------------------------------------------------------------

// ---------------------------------
// General Constants
// ---------------------------------
#define SYSCLK 24500000UL              // SYSCLK frequency in Hz

// LED / Timer0 - Constants:

#define LED_SLOW_RATE   2              // Rate of LED blinks per sec.
// Minimum value is 1

#define LED_FAST_RATE   10             // Rate of LED blinks per sec.
// Minimum value is 1

// Reload value for Timer0 assuming a 1:48 scaling factor for Timer0 clock
#define TIMER0_RL_SLOW_HIGH  (0xFFFF -((SYSCLK/48/LED_SLOW_RATE >> 8) & 0x00FF))
#define TIMER0_RL_SLOW_LOW   (0xFFFF -(SYSCLK/48/LED_SLOW_RATE & 0x00FF))

// Reload value for Timer0 assuming a 1:48 scaling factor for Timer0 clock
#define TIMER0_RL_FAST_HIGH  (0xFFFF -((SYSCLK/48/LED_FAST_RATE >> 8) & 0x00FF))
#define TIMER0_RL_FAST_LOW   (0xFFFF -(SYSCLK/48/LED_FAST_RATE & 0x00FF))


// ---------------------------------
// Target BL Config Options
// ---------------------------------

#define FLASH_C2_ACCESS_LOCK_BYTE  0xFF
#define TGT_BL_FW_INFOBLOCK_LENGTH 16
#define TGT_CRC_TYPE               0x01  // CRC Type = CRC-16-CCITT
#define TGT_ENCRYPTION_TYPE        0x00  // None

#define TGT_BL_FW_VERSION_LOW      0x00
#define TGT_BL_FW_VERSION_HIGH     0x01

#define TGT_BL_BUF_SIZE          40    // Size of bootloader RX buffer (bytes; should be powers of 2)
// min: 2^5 = 32; max: 2^14 = 16384; 2^15 = RESERVED.

#define TGT_BL_BUF_SIZE_CODE     0x05  // BL Buffer Size Code. 
// If TGT_BL_BUF_SIZE is 2^n, TGT_BL_BUF_SIZE_CODE is n.
// See documentation for encoding details.

//-----------------------------------------------------------------------------
// Enums, Structs, Unions and Typedefs - Available to all modules
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// "Public" Global Variables - Externs - Available to all modules
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// "Public" Function Prototypes - Externs - Available to all modules
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#endif // #ifndef _FXXX_TARGETBL_CONFIG_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
