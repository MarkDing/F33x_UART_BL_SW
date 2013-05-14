//-----------------------------------------------------------------------------
// Fnnn_TargetBL_DevSpecific.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fnnn_TargetBL_DevSpecific.c
// nnn => Fill-in with the MCU family part number.
// Examples: F50x_TargetBL_DevSpecific.c, F33x_TargetBL_DevSpecific.c
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

// ------------------------------------------
// *** Device-Specific *** 
// Change this as necessary based on MCU
// ------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------
void OSCILLATOR_Init(void);
void PORT_Init(void);
void UART0_Init(void);

//=============================================================================
// Function Definitions
//=============================================================================
void Disable_Watchdog(void)
{
    PCA0MD &= ~0x40; // Disable Watchdog
}

void Device_Init(void)
{
	Disable_Watchdog();
    OSCILLATOR_Init(); // Configure system clock
    PORT_Init(); // Initialize crossbar
    UART0_Init(); // Init UART0 & Timer1 (Data source comm)
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency.
// Also enables the Missing Clock Detector.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init(void)
{

    OSCICN = 0x83; // Set system clock to 24.5 MHz
    VDM0CN = 0x80; // Enable VDD monitor and early warning
    RSTSRC = 0x02; // Enable VDD monitor as a reset source
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART0 TX
// P0.5   digital   open-drain   UART0 RX
//
// P1.3   digital  push-pull     LED
// P0.7   digital  open-drain    SW

//-----------------------------------------------------------------------------
void PORT_Init(void)
{
    // Initialize port I/O
    P0MDOUT = 0x10; // Enable UART0 TX as push-pull output
    P1MDOUT = 0x08; // P1.3 (LED) is push-pull
    P0SKIP = 0x03;
    XBR0 = 0x01; // Enable UART0
    XBR1 = 0x40; // Enable crossbar
}


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init(void)
{
    SCON0 = 0x10; // SCON0: 8-bit variable bit rate
    //        level of STOP bit is ignored
    //        RX enabled
    //        ninth bits are zeros
    //        clear RI0 and TI0 bits
    if (SYSCLK / BAUDRATE / 2 / 256 < 1)
    {
        TH1 = -(SYSCLK / BAUDRATE / 2); // 0x96
        CKCON &= ~0x0B; // T1M = 1; SCA1:0 = xx
        CKCON |= 0x08;
    }
    else if (SYSCLK / BAUDRATE / 2 / 256 < 4)
    {
        TH1 = -(SYSCLK / BAUDRATE / 2 / 4);
        CKCON &= ~0x0B; // T1M = 0; SCA1:0 = 01
        CKCON |= 0x01;
    }
    else if (SYSCLK / BAUDRATE / 2 / 256 < 12)
    {
        TH1 = -(SYSCLK / BAUDRATE / 2 / 12);
        CKCON &= ~0x0B; // T1M = 0; SCA1:0 = 00
    }
    else
    {
        TH1 = -(SYSCLK / BAUDRATE / 2 / 48);
        CKCON &= ~0x0B; // T1M = 0; SCA1:0 = 10
        CKCON |= 0x02;
    }

    TL1 = TH1; // init Timer1
    TMOD &= ~0xf0; // TMOD: timer 1 in 8-bit autoreload
    TMOD |= 0x20;
    TR1 = 1; // START Timer1
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
