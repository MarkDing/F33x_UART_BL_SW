//-----------------------------------------------------------------------------
// F33x_Comm_UART.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fnnn_Comm_CommType.c
// nnn => Fill-in with the MCU family part number.
// CommType => Fill-in Comm interface name.
// Examples: F33x_Comm_SMB.c, F33x_Comm_UART.c
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


//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------

//=============================================================================
// Function Definitions
//=============================================================================
//-----------------------------------------------------------------------------
// SRC_Get_Info
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Info(U8 *buf)
{
    U8 cmd = SRC_CMD_GET_INFO;

    // Command Format:
    // [0] Command
	uart_send(&cmd, 1);
    // Response:
    // [0] Return code (ACK/ERROR etc)
    // [1] InfoBlock Length
    // [2] MCU Code
    // [3] BL Type
    // [4] Flash Page Size Code
    // [5] Application FW Version Low (0)
    // [6] Application FW Version High (1)
    // [7] SMB Device Address
    // [8] Application FW Start Addr Low (0)
    // [9] Application FW Start Addr (1)
    // [10] Application FW Start Addr High (2)
    // [11] Application FW End Addr Low (0)
    // [12] Application FW End Addr (1)
    // [13] Application FW End Addr High (2)
	uart_receive(buf, SRC_CMD_GET_INFO_RX_SZ);

    return (buf[0]);
}


//-----------------------------------------------------------------------------
// SRC_Get_Page_Info
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Page_Info(U8* buf)
{
	U8 cmd = SRC_CMD_GET_PAGE_INFO;

    // Command Format:
    // [0] Command
	uart_send(&cmd, 1);

    // Response:
    // [0] Return code (ACK/ERROR etc)
    // [1] Page address byte 0 {or Num Pages, if return code = SRC_RSP_DATA_END}
    // [2] Page address byte 1
    // [3] Page address byte 2 <-- don't care for MCUs 64k and smaller
    // [4] Page CRC byte 0
    // [5] Page CRC byte 1
    // Note: If Num Pages is provided, add 1 to get actual num pages
	uart_receive(buf, SRC_CMD_GET_PAGE_INFO_RX_SZ);

    return (buf[0]);
}

//-----------------------------------------------------------------------------
// SRC_Get_Page
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Page(U8 *buf)
{
	U8 cmd = SRC_CMD_GET_PAGE;
    // Command Format:
    // [0] Command
	uart_send(&cmd, 1);

    // Response:
    // [0] Return code (ACK/ERROR etc)
    // [1..512] Page data
    // [513] Return code (ACK/ERROR etc)
	uart_receive(&cmd, 1);
	uart_receive(buf, SRC_CMD_GET_PAGE_RX_SZ);
	uart_receive(&cmd, 1);

    return (cmd);
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//  uart send, count max number is 512, limited by XRAM size
U8 uart_send(U8 *buf, U16 count)
{
	U8 *ptr = buf;
	do{
	    SBUF0 = *ptr++;
		while(TI0 == 0);
		TI0 = 0;
	}while(count--);
	return 0;
}

// uart receive, count max 512, limited by XRAM size
U8 uart_receive(U8 *buf, U16 count)
{
	U8 * ptr = buf;
	do{
		if(RI0 == 1)
		{
			*ptr++ = SBUF0;
			RI0 = 0;
			count--;
		}
	}while(count);

	return 0;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
