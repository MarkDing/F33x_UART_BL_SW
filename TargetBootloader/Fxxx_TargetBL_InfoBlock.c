//-----------------------------------------------------------------------------
// Fxxx_TargetBL_InfoBlock.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This should be located at the end of the target bootloader flash area using
// compiler/linker directives.
// 
//
//
// Target:         C8051Fxxx (Any Silicon Labs Flash MCU)
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0 / 01SEP2010 (PKC)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>

#include "Fxxx_Target_Config.h"
#include "Fxxx_Target_Interface.h"

#include "Fxxx_TargetBL_Config.h"
#include "Fxxx_TargetBL_Interface.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

/*****************************************
Target Bootloader InfoBlock Format
	COMMAND CODE
	infoLength,
	BLfwVer0,
	BLfwVer1,
	MCUcode,
	BLtype,
	flashPageSizeCode,
	BLbufferSizeCode,
	CRCtype,
	encryptionType,
	appFWstartAddr0,
	appFWstartAddr1,
	appFWstartAddr2,
	appFWendAddr0,
	appFWendAddr1,
	appFWendAddr2,
	CANdeviceAddr,
	appFWver0,
	appFWver1,
*****************************************/

SEGMENT_VARIABLE(TGT_BL_InfoBlock[TGT_BL_FW_INFOBLOCK_LENGTH], const U8, SEG_CODE) =
{
	SRC_CMD_DISP_TGT_INFO,
	TGT_BL_FW_INFOBLOCK_LENGTH,	
	TGT_BL_FW_VERSION_LOW,
   	TGT_BL_FW_VERSION_HIGH,
	TGT_PRODUCT_CODE,
	TGT_BL_TYPE,
	TGT_FLASH_PAGE_SIZE_CODE,
    0xFC, //((TGT_BL_PKT_SIZE_CODE << 4)|(TGT_BL_BUF_SIZE_CODE & 0x0F)),
    TGT_CRC_TYPE,
	TGT_ENCRYPTION_TYPE,
 	(APP_FW_START_ADDR & 0xFF),
	((APP_FW_START_ADDR & 0xFF00) >> 8),
	((APP_FW_START_ADDR & 0xFF0000) >> 16),
	(APP_FW_END_ADDR & 0xFF),
	((APP_FW_END_ADDR & 0xFF00) >> 8),
	((APP_FW_END_ADDR & 0xFF0000) >> 16),
	0x25,
	TGT_DEVICE_SERIAL0,
	TGT_DEVICE_SERIAL1
};

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------