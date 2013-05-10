//-----------------------------------------------------------------------------
// Fxxx_TargetBL_Commands.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// 
//
// How To Test:
//
// 
//
//
// Target:         C8051Fxxx (Any Silicon Labs Flash MCU)
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
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include "Fxxx_SFR_Defs_Wrapper.h"     // This header file will include the
// real MCU register definition file

#include "Fxxx_Target_Config.h"
#include "Fxxx_Target_Interface.h"

#include "Fxxx_TargetBL_Config.h"
#include "Fxxx_TargetBL_Interface.h"

#include "Fxxx_BL129_UART_Interface.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

U8 Flash_Key0 = 0xA5;
U8 Flash_Key1 = 0xF1;

//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------


//=============================================================================
// Function Definitions
//=============================================================================


//-----------------------------------------------------------------------------
// TGT_Erase_Page
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Erases one page of flash.
//-----------------------------------------------------------------------------
void TGT_Erase_Page(U16 Addr)
{
    PSCTL |= 0x03;
    FLASH_Modify(Addr, 0x00);
}

//-----------------------------------------------------------------------------
// TGT_Write_Flash
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Writes 512 bytes of flash.
//-----------------------------------------------------------------------------
void TGT_Write_Flash(U8 *buf, U16 addr)
{
	U8 * ptr = buf;
	U16 count = TGT_FLASH_PAGE_SIZE;
    // Setup for flash operation
    while (count--)
    {
        PSCTL |= 0x01;
        FLASH_Modify(addr, *ptr++);
        addr ++;
    }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
