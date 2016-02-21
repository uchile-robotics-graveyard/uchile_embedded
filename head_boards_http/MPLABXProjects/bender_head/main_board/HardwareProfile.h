 /*********************************************************************
 *
 *	Hardware specific definitions for:
 *    - PIC18F96J60
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    Compiler.h
 * Processor:       PIC18
 * Compiler:        Microchip C18 v3.36 or higher
 * Company:         Microchip Technology, Inc.
 ********************************************************************/
#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "Compiler.h"

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
        // WDT (ON): Watch Dog Timer
	#pragma config WDT=OFF

        /* FOSC2 ( ON ): Default/Reset System Clock Select bit. 
         * - OFF: clock=INTRC,
         * - ON : clock selected by FOSC1:FOSC0
         */
        #pragma config FOSC2=ON

        // FOSC (ECPLL): Oscillator Selection. (HS|EC) osc., (HSPLL|ECPLL) HS|EC-CLKO with PLL..
        #pragma config FOSC=HSPLL


        /* ETHLED( ON ): Ethernet LED Enable bit.
         * - ON : RA0/RA1 are multiplexed with LEDA/LEDB when Ethernet module is
         *        enabled and function as I/O when Ethernet is disabled.
         * - OFF: RA0/RA1 function as I/O regardless of Ethernet module status.
         */
        #pragma config ETHLED=ON

        /* CCP2MX( ON ): ECCP2 MUX bit.
         *  - ON :ECCP2/P2A is multiplexed with RC1.
         *  - OFF:ECCP2/P2A is multiplexed with:
         *         - RE7 in Microcontroller mode
         *         - RB3 in Extended Microcontroller mode
         */
        #pragma config CCP2MX=OFF
        
        /*ECCPMX( ON ): ECCP MUX bit. ECCP1(P1B/P1C) and ECCP3(P3B/P3C) outputs are multiplexed with:
         * - ON : RE6/RE5 and RE4/RE3
         * - OFF: RH7/RH6 and RH5/RH4
         */
        #pragma config ECCPMX=OFF

        /* Other configs:
         * STVR  ( ON ): Stack Overflow/Underflow Reset
         * DEBUG (OFF ): Background Debbuger - RB6,RB7 for In-Circuit Debug
         * CP0   (OFF ): Code Protection bit
         * FCMEN ( ON ): Fail-Safe Clock Monitor
         * IESO  ( ON ): Two-Speed Start-up (Internal/External Oscillator Switchover) Control
         * WDTPS(32768): Watchdog Timer Postscaler Select bits. Values: 0x0N --> 1:N
         * EASHFT( ON ): External Address Bus Shift Enable
         * MODE  ( MM ): External Memory Bus 
         *               (XM20|XM16|XM12): Extended microcontroller mode, N-Bit Address mode.
         *               (MM) Microcontroller mode. Ex. uC disabled
         * BW    ( 16 ): Data Bus Width Select bit (8|16)
         * WAIT  (OFF ): External Bus Wait Enable bit
         */

	// Automatically set Extended Instruction Set fuse based on compiler setting
        // XINST ( ON ): Extended Instruction Set
	#if defined(__EXTENDED18__)
		#pragma config XINST=ON
	#else
		#pragma config XINST=OFF
	#endif
#endif


// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()		(41666667ul)			// Hz
#define GetInstructionClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/4)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.


// Hardware I/O pin mappings

// LED DRIVER
#define LD_RST_B_TRIS	(TRISDbits.TRISD2)
#define LD_RST_B	(PORTDbits.RD2)
#define LD_SEL_BK_TRIS	(TRISDbits.TRISD1)
#define LD_SEL_BK	(PORTDbits.RD1)
#define LD_LAT_B_TRIS	(TRISDbits.TRISD0)
#define LD_LAT_B	(PORTDbits.RD0)
#define LD_GCK_TRIS	(TRISEbits.TRISE7)
#define LD_GCK		(PORTEbits.RE7)
#define LD_EN_B_TRIS	(TRISEbits.TRISE6)
#define LD_EN_B		(PORTEbits.RE6)

// Momentary push buttons
#define BUTTON0_TRIS	(TRISJbits.TRISJ4) // Ref S5
#define	BUTTON0_IO	(PORTJbits.RJ4)
#define BUTTON1_TRIS	(TRISJbits.TRISJ5) // Ref S4
#define	BUTTON1_IO	(PORTJbits.RJ5)
#define BUTTON2_TRIS	(TRISJbits.TRISJ0) // Ref S3
#define	BUTTON2_IO	(PORTJbits.RJ0)
#define BUTTON3_TRIS	(TRISJbits.TRISJ1) // Ref S2
#define	BUTTON3_IO	(PORTJbits.RJ1)

// Ethernet TPIN+/- polarity swap circuitry (PICDEM.net 2 Rev 6)
#define ETH_RX_POLARITY_SWAP_TRIS (TRISGbits.TRISG0)
#define ETH_RX_POLARITY_SWAP_IO   (LATGbits.LATG0)

// UART mapping functions for consistent API names across 8-bit and 16 or
// 32 bit compilers.  For simplicity, everything will use "UART" instead
// of USART/EUSART/etc.
#define BusyUART()        BusyUSART()
#define CloseUART()       CloseUSART()
#define ConfigIntUART(a)  ConfigIntUSART(a)
#define DataRdyUART()     DataRdyUSART()
#define OpenUART(a,b,c)   OpenUSART(a,b,c)
#define ReadUART()        ReadUSART()
#define WriteUART(a)      WriteUSART(a)
#define getsUART(a,b,c)   getsUSART(b,a)
#define putsUART(a)       putsUSART(a)
//#define getcUART()        ReadUSART()
//#define putcUART(a)       WriteUSART(a)
#define putrsUART(a)      putrsUSART((far rom char*)a)

#endif // #ifndef HARDWARE_PROFILE_H
