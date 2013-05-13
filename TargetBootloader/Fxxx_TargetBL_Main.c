//-----------------------------------------------------------------------------
// Fxxx_TargetBL_Main.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// The main routine for the Target Bootloader Firmware.
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
#include "F33x_CRC.h"
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SBIT(BL_Override_Pin, SFR_P0, 7);
SEGMENT_VARIABLE( Page_Buf[TGT_FLASH_PAGE_SIZE], U8, SEG_XDATA);

U8 data SRC_Response;
U16 data SRC_Page_CRC;
U8 data rx_buf[32];
U8 Last_Error = 0;

#define APP_MODE        0
#define BOOTLOADER_MODE 1

// Bit masks for the RSTSRC SFR
#define PORSF  0x02
#define FERROR 0x40

void SRC_Validate_Response(U8 response, U8 command);


//=============================================================================
// Main Routine
//=============================================================================
void main(void)
{
    U8 device_mode = BOOTLOADER_MODE;
	U8 code* codeptr;
	U16 data page_addr = 0;
    //---------------------------------------
    // Check the bootloader consition.
    //---------------------------------------
    codeptr = (U8 code*)(APP_FW_SIG3_ADDR);
   // The Signature (in Flash) should be valid to allow application FW execution.
   // This is written at the end of the bootloading process by the bootloader.
    if((codeptr[0] == SIG_BYTE3) &&(codeptr[1] == SIG_BYTE2) &&(codeptr[2] == SIG_BYTE1) && (codeptr[3] == SIG_BYTE0))
    {
   		device_mode = APP_MODE;
    }
    if ((!BL_Override_Pin) || (((RSTSRC & PORSF) == 0) && ((RSTSRC & FERROR)
            != 0)))
    {
        device_mode = BOOTLOADER_MODE;
    }


    if (device_mode == APP_MODE)
    {
        // If not in BL Override, jump to application
        START_APPLICATION();
    }

    //-------------------------------------------
    // ** BL Mode ** Initialize MCU and Variables
    //-------------------------------------------
    Device_Init();

    //-------------------------------------------
    // Main Loop
    //-------------------------------------------
	while(1)
	{
		while(SRC_Disp_TGT_Info() != SRC_RSP_OK);
		SRC_Response = SRC_Get_Info();
        SRC_Validate_Response(SRC_Response, SRC_CMD_GET_INFO);
        if (Last_Error != 0)
	        goto error;
		Flash_Key0 = rx_buf[5];
		Flash_Key1 = rx_buf[6];
		while(1)
		{
			SRC_Response = SRC_Get_Page_Info();
			SRC_Page_CRC = rx_buf[4] | (rx_buf[5] << 8);
			page_addr = rx_buf[1] | (rx_buf[2] << 8);
            SRC_Validate_Response(SRC_Response, SRC_CMD_GET_PAGE_INFO);
            if (Last_Error != 0)
	            break;
	        // Exit this loop if no more pages are available from source
    	    if (SRC_Response == SRC_RSP_DATA_END) 
	    	    break;
            SRC_Response = SRC_Get_Page(Page_Buf);
            SRC_Validate_Response(SRC_Response, SRC_CMD_GET_PAGE); // this will check for CRC match
            if (Last_Error != 0)
	            break;
			TGT_Erase_Page(page_addr);
			TGT_Write_Flash(Page_Buf, page_addr);

		}
        // Set flash keys to 0
        Flash_Key0 = 0;
        Flash_Key1 = 0;
error:
        if (Last_Error != 0)
        {
            SRC_Response = SRC_Disp_Info_Code(Last_Error);
            Last_Error = 0;
        }
        RSTSRC = 0x12; // Initiate software reset with vdd monitor enabled
	}
}

//-----------------------------------------------------------------------------
// SRC_Validate_Response
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
void SRC_Validate_Response(U8 response, U8 command)
{
    U16 buf_crc;

    if ((response != SRC_RSP_OK) && (response != SRC_RSP_DATA_END))
    {
	    Last_Error = ERR_SRC_UNEXPECTED_RSP;
		return;
    }
    switch (command)
    {
        case SRC_CMD_GET_INFO:
        case SRC_CMD_GET_PAGE_INFO:
            break;
        case SRC_CMD_GET_PAGE:
            buf_crc = Get_Buf_CRC(Page_Buf, TGT_FLASH_PAGE_SIZE);
            if (buf_crc != SRC_Page_CRC)
            {
	            Last_Error = ERR_SRC_CRC_MISMATCH;
            }
            break;
        default:
            break;
    }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
