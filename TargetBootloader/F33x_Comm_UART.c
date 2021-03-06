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
#include "F33x_CRC.h"
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern U8 data rx_buf[32];
extern SEGMENT_VARIABLE(TGT_BL_InfoBlock[TGT_BL_FW_INFOBLOCK_LENGTH], const U8, SEG_CODE);
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
// Return Value:  Response
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Info(void)
{
    U8 data cmd = SRC_CMD_GET_INFO;

    // Command Format:
    // [0] Command
	uart_send(&cmd, 1);
    // Response:
    // [0] Return code (ACK/ERROR etc)
    // [1] InfoBlock Length
    // [2] MCU Code
    // [3] BL Type
    // [4] Flash Page Size Code
    // [5] Flash Key0
    // [6] Flash Key1
    // [7] SMB Device Address
    // [8] Application FW Start Addr Low (0)
    // [9] Application FW Start Addr (1)
    // [10] Application FW Start Addr High (2)
    // [11] Application FW End Addr Low (0)
    // [12] Application FW End Addr (1)
    // [13] Application FW End Addr High (2)
	uart_receive(rx_buf, SRC_CMD_GET_INFO_RX_SZ);

    return (rx_buf[0]);
}

//-----------------------------------------------------------------------------
// SRC_Get_Page_Info
//-----------------------------------------------------------------------------
//
// Return Value:  Reponse
// Parameters:    buf
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Page_Info(void)
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
	uart_receive(rx_buf, SRC_CMD_GET_PAGE_INFO_RX_SZ);

    return (rx_buf[0]);
}

//-----------------------------------------------------------------------------
// SRC_Get_Page
//-----------------------------------------------------------------------------
//
// Return Value:  Resposne
// Parameters:    buf
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Get_Page(U8 xdata *buf)
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
// SRC_Disp_TGT_Info
//-----------------------------------------------------------------------------
//
// Return Value:  Response
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Disp_TGT_Info(void)
{
    U8 cmd;

    // Command Format:
    // [0] Command
	uart_send(TGT_BL_InfoBlock, TGT_BL_FW_INFOBLOCK_LENGTH);

    // Response:
    // [0] Return code (ACK/ERROR etc)
	uart_receive(&cmd, SRC_CMD_DISP_TGT_INFO_RX_SZ);


    return (cmd);
}

//-----------------------------------------------------------------------------
// SRC_Disp_Info_Code
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    info_code
//
//
//
//-----------------------------------------------------------------------------
U8 SRC_Disp_Info_Code(U8 info_code)
{

    // Command Format:
    // [0] Command
    // [1] Info Code
	rx_buf[0] = SRC_CMD_DISP_INFO_CODE;
	rx_buf[1] = info_code;
	uart_send(rx_buf, 2);

    // Response:
    // [0] Return code (ACK/ERROR etc)
	uart_receive(rx_buf, SRC_CMD_DISP_INFO_CODE_RX_SZ);
    return (rx_buf[0]);
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//  uart send, count max number is 255, limited by XRAM size
void uart_send(U8 *buf, U8 count)
{
	U8 *ptr = buf;
	do{
	    SBUF0 = *ptr++;
		while(TI0 == 0);
		TI0 = 0;
	}while(--count);
}

// uart receive, count max 512, limited by XRAM size
void uart_receive(U8 *buf, U16 count)
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
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
