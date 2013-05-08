//-----------------------------------------------------------------------------
// Fxxx_Target_Interface.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fxxx_TargetBL_Interface.c - DO NOT change this file name.
// xxx => Leave as-is; this means the file is compatible with all C8051F devices.
//
// Content: Publishes the interface used by the bootloader framework. 
// No changes necessary.
//
// Target:         Any Silicon Labs C8051 MCU
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.1 / 9May2013 (MarkD)
//    -F330 UART Revision
// Release 1.0 / 01SEP2010 (PKC)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#ifndef _FXXX_TARGET_INTERFACE_H_
#define _FXXX_TARGET_INTERFACE_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS - Available to all modules
//-----------------------------------------------------------------------------

#define TGT_CMD_FIXED_LENGTH        8

// -----------------------------------  
// Bootloader Version (BCD encoding)
// -----------------------------------
#define TGT_BL_VER                  0x10

// Signature Bytes
#define SIG_BYTE0             0x3D
#define SIG_BYTE1             0xC2
#define SIG_BYTE2             0xA5
#define SIG_BYTE3             0x1B

// Data Source Commands
#define SRC_CMD_GET_INFO         0x80
#define SRC_CMD_GET_PAGE_INFO    0x81
#define SRC_CMD_GET_PAGE         0x82
#define SRC_CMD_DISP_TGT_INFO    0x83
#define SRC_CMD_DISP_INFO_CODE   0x84

// Data Source Response Sizes
#define SRC_CMD_GET_INFO_RX_SZ      14
#define SRC_CMD_GET_PAGE_INFO_RX_SZ 6
#define SRC_CMD_GET_PAGE_RX_SZ      (TGT_FLASH_PAGE_SIZE)
#define SRC_CMD_DISP_TGT_INFO_RX_SZ 1
#define SRC_CMD_DISP_INFO_CODE_RX_SZ 1

// Data Source Response Codes
#define SRC_RSP_OK               0x70
#define SRC_RSP_ERROR            0x71
#define SRC_RSP_DATA_END         0x72
#define SRC_RSP_UNKNOWN_CMD      0x73

// ---------------------------------
// Target BL Command Codes
// ---------------------------------
#define TGT_CMD_RESET_MCU           0x00
#define TGT_CMD_GET_VERSION         0x01
#define TGT_CMD_ERASE_FLASH_PAGE    0x02
#define TGT_CMD_WRITE_FLASH_BYTES   0x03
#define TGT_CMD_READ_FLASH_BYTES    0x04
#define TGT_CMD_ENTER_BL_MODE       0x05
// ---------------------------------
// Target BL Response Codes
// ---------------------------------
#define TGT_RSP_OK                  0x00 // RSP_OK should always be 0
#define TGT_RSP_PARAMETER_INVALID   0x01
#define TGT_RSP_UNSUPPORTED_CMD     0x02
#define TGT_RSP_BL_MODE             0x03
#define TGT_RSP_ERROR               0x80


//---------------------------------------------
// Error Codes
//---------------------------------------------
#define ERR_TGT_INFO_MISMATCH    0x01
#define ERR_TGT_BL_MODE          0x02
#define ERR_TGT_UNEXPECTED_RSP   0x03
#define ERR_TGT_CRC_MISMATCH     0x04

#define ERR_SRC_INFO_MISMATCH    0x10
#define ERR_SRC_CRC_MISMATCH     0x11
#define ERR_SRC_UNEXPECTED_RSP   0x12

#define ERR_TGT_SRC_INFO_MISMATCH   0x20
#define ERR_NUM_PAGES_MISMATCH      0x21
//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#endif // #ifndef _FXXX_TARGET_INTERFACE_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
