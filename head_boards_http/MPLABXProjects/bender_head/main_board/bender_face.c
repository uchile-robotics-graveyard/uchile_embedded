/* 
 * File:   bender_face.c
 * Author: benderteam
 *
 * Created on 7 de marzo de 2015, 03:35 AM
 */

/*
 * This macro uniquely defines this file as the main entry point.
 * There should only be one such definition in the entire project,
 * and this file must define the AppConfig variable as described below.
 */
#define THIS_IS_STACK_APPLICATION

#include <spi.h>               // spi communication PIC-eth:Led Drivers
#include <i2c.h>               // i2c communication PIC-eth:PIC-servos
#include "TCPIP Stack/TCPIP.h" // tcp/ip communication PIC-PC


// Include functions specific to this stack application
#include "bender_face.h"

// initialize it with the default values defined in TCPIPConfig.h.
// e.g, set MyIPAddr field to the values of the MY_DEFAULT_IP_ADDR_BYTEn macros
// in TCPIPConfig.h.
// see InitAppConfig(...) function in MainDemo.c (TCPIP Demo App) to see how to
// populate this structure completely.
// see StackTsk.h for a full list of APP_CONFIG parameters
APP_CONFIG AppConfig;

BYTE AN0String[8];

// i2c variables
unsigned char value;
#define PWM_ADDR 0x80

// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);
static BOOL ProcessIO(void);
void sendMotorPosition(char motor, char pose);

//
// PIC18 Interrupt Service Routines
//
// NOTE: Several PICs, including the PIC18F4620 revision A3 have a RETFIE FAST/MOVFF bug
// The interruptlow keyword is used to work around the bug when using C18
#pragma interruptlow LowISR
void LowISR(void) { TickUpdate(); }
#pragma interruptlow HighISR
void HighISR(void) { }
#pragma code lowVector=0x18
void LowVector(void){_asm goto LowISR _endasm}
#pragma code highVector=0x8
void HighVector(void){_asm goto HighISR _endasm}
#pragma code // Return to default code section

// XXX: leds?
static char list[120]; // 96
bHeadState head_state;
//
// Main application entry point.
//
void main(void)
{
    BYTE idx;
    //unsigned char i=0; // int
    //static DWORD t = 0;
    //static DWORD dwLastIP = 0;
    //char update=0;
    //BOOL standby=TRUE;


    // - - - - INITIALIZATION - - - -
    // 1.1) hardware initialization:
    // PPS pins, oscillators, LEDs, LCDs, any SPI or PMP modules you're using to
    // control your hardware MAC/PHY chip, etc.
    initHeadState();
    InitializeBoard();
    
    // 1.2) library initialization:
    // - TickInit(): Must be called 1st. It will initialize the tick timer that manages your stack timing.
    // - Any funtion for hardware initialization (e.g, MPFSInit())
    TickInit();
    MPFSInit();

    // 1.3) library configuration:
    // - AppConfig initialization
    // - call StackInit() when done
    
    // Initialize Stack and application related NV variables into AppConfig.
    InitAppConfig();

    // Initialize core stack layers (MAC, ARP, TCP, UDP) and app modules (HTTP, ICMP, ...)
    StackInit();
    
    // 1.4) application specific firmware initialization functions

    // Led Driver Init
    /*LD_SEL_BK = 1;
    //LD_EN_B = 0;
    LD_RST_B = 1;
    for(idx=0; idx<96; idx++)
    {
        WriteSPI2(0x00);
        list[idx]=(0x00);
    }
    LD_EN_B = 0;
    */

    // PWM 66k90: I2C Init
    /*
    OpenI2C1(MASTER, SLEW_OFF);  //SLEW_OFF = 100 KHz; SLEW_ON = 400 KHz;
    IdleI2C1();     // Revisa si el modulo esta activo
    StartI2C1();    // Start

    while(1)
    {
    WriteI2C1(0xFF);
    DelayMs(1);
    value = ReadI2C1();
    }

    StopI2C1();
    */


    // - - - - MAIN LOOP - - - -
    // 2) infinite loop for handling application tasks:
    // - StackTask(): perform Stack Timed operations and handle Rx/Tx of packets
    // - StackApplications(): calls app modules (e.g, HTTP2 or ICMP queued tasks)
    //
    // Custom functions should be added at the end of this loop.
    //
    // If a task needs very long time to do its job, it must be broken
    // down into smaller pieces so that other tasks can have CPU time.
    while(1)
    {
        /*
        unsigned char i=0; // int
        if(update>=105)         //Si se recibio un mensaje completo // 95
        {                       //se actualizan los leds.
            standby=FALSE;
            for(i=0;i<96;i++)
            {
                WriteSPI2(list[i]);
            }
            for(i=96;i<106;i++)
            {
                sendMotorPosition(i-96,list[i]);
            }
            LD_LAT_B        =1;
            DelayMs(1);
            LD_LAT_B        =0;
            DelayMs(1);
            LD_EN_B         =0;
        }
        if(standby)
        {
            if(i%96==0)
                WriteSPI2(127);
            else
                WriteSPI2(0);
            LD_LAT_B        =1;
            DelayMs(1);
            LD_LAT_B        =0;
            DelayMs(50);
            i=++i%96;
        }
        */

        // stack mantention routines
        TickUpdate();

        // This task performs normal stack task including checking
        // for incoming packet, type of packet and calling
        // appropriate stack entity to process it.
        StackTask();

        // This tasks invokes each of the core stack application tasks
        StackApplications();

        // Process application specific tasks here.
        // For this demo app, this will include the Generic TCP
        // client and servers, and the SNMP, Ping, and SNMP Trap
        // demos.  Following that, we will process any IO from
        // the inputs on the board itself.
        // Any custom modules or processing you need to do should
        // go here.

        //update=BenderTCPServer(list);

        //ProcessIO();
        /*
        if(ProcessIO())
        {
            update=0;
            standby=FALSE;
        }*/
    }
    
    return;
}

// TODO
static BOOL ProcessIO(void)
{
    static BYTE i=0;
    unsigned char value;
    BOOL resp=FALSE;
    static BOOL last_state=FALSE;
    BOOL current_state=FALSE;
    if(!BUTTON0_IO)
    {
        current_state=TRUE;
        resp=TRUE;
        //Reset cara
        if(!last_state)
        {
            for(i=0;i<96;i++)
            {
                if(i%48==0)
                    WriteSPI2(255);
                else
                    WriteSPI2(0);
            }
            LD_LAT_B        =1;
            DelayMs(1);
            LD_LAT_B        =0;
            DelayMs(1);
            LD_EN_B         =0;
            i               =0;
        }else
        {
            if(i%48==0)
                WriteSPI2(255);
            else
                WriteSPI2(0);
            LD_LAT_B        =1;
            DelayMs(1);
            LD_LAT_B        =0;
            DelayMs(249);
            i=++i%48;
        }
    }
    if(!BUTTON1_IO)
    {
        for(i=0;i<10;i++)
            sendMotorPosition(i,0x00);
    }
    if(!BUTTON2_IO)
    {
        for(i=0;i<10;i++)
            sendMotorPosition(i,0xff);
    }
    last_state=current_state;
    return resp;
}

static void InitializeBoard(void)
{
    // Enable 4x/5x/96MHz PLL on PIC18F87J10, PIC18F97J60, PIC18F87J50, etc.
    OSCTUNE = 0x40;

    // Enable internal PORTB pull-ups
    INTCON2bits.RBPU = 0;

    // Enable Interrupts
    RCONbits.IPEN   = 1; // Enable interrupt priorities
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    // init IOs (Buttons are inputs)
    BUTTON0_TRIS = 1;
    BUTTON1_TRIS = 1;
    BUTTON2_TRIS = 1;
    BUTTON3_TRIS = 1;

    // Led Driver init (Control pins are outputs)
    LD_RST_B_TRIS  = 0;
    LD_SEL_BK_TRIS = 0;
    LD_LAT_B_TRIS  = 0;
    LD_GCK_TRIS    = 0;
    LD_EN_B_TRIS   = 0;
    LD_EN_B        = 1; // dont init board (board desactivated)
    LD_RST_B       = 0;

    // SPI setup
    TRISDbits.TRISD4 = 0; // SDO:Out
    TRISDbits.TRISD5 = 1; // SDI:In
    TRISDbits.TRISD6 = 0; // SCK:Out
    OpenSPI2(SPI_FOSC_4,MODE_01,0x00);

    // PWM setup
    T2CONbits.TMR2ON   = 0;
    PR2                = 0x01;
    CCPR2L             = 0x01;
    T2CONbits.T2CKPS1  = 0;
    T2CONbits.T2CKPS0  = 0;
    T2CONbits.TMR2ON   = 1;
    CCP2CONbits.P2M1   = 0;
    CCP2CONbits.P2M0   = 0;
    CCP2CONbits.DC2B1  = 0;
    CCP2CONbits.DC2B0  = 0;
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 0;
    CCP2CONbits.CCP2M0 = 0;

    // I2C setup
    //OpenI2C1(MASTER, SLEW_OFF); // SLEW_OFF = 100 KHz; SLEW_ON = 400 KHz;
    SSPADD = 0x57;
}

void sendMotorPosition(char motor, char pose)
{
    IdleI2C1();                         // Wait until the bus is idle
    StartI2C1();                        // Send START condition
    IdleI2C1();                         // Wait for the end of the START condition
    WriteI2C1( PWM_ADDR & 0xfe );  // Send address with R/W cleared for write
    IdleI2C1();                         // Wait for ACK
    DelayMs(10);
    WriteI2C1( motor );                 // Write first byte of data
    IdleI2C1();                         // Wait for ACK
    DelayMs(10);
    WriteI2C1( pose );                  // Write first byte of data
    IdleI2C1();
    DelayMs(10);
    StopI2C1();
}

int getNeckYaw() {
    return head_state.neckYaw;
}

BOOL isMouthEnabled() {
    return head_state.mouthEnabled;
}

void setNeckYaw(int yaw) {

    // set warning
    if (yaw > BENDER_NECK_MAX_YAW) {
        yaw = BENDER_NECK_MAX_YAW;
    } else if (yaw < BENDER_NECK_MIN_YAW) {
        yaw = BENDER_NECK_MIN_YAW;
    }

    //int last_yaw = head_state.neckYaw;
    (&head_state)->neckYaw = yaw;
}

BYTE* getHeadEmotionStr() {

    switch (head_state.emotion) {
        case EMOTION_SERIOUS:   { return "SERIOUS";   }
        case EMOTION_HAPPY1:    { return "HAPPY1";    }
        case EMOTION_HAPPY2:    { return "HAPPY2";    }
        case EMOTION_HAPPY3:    { return "HAPPY3";    }
        case EMOTION_SAD1:      { return "SAD1";      }
        case EMOTION_SAD2:      { return "SAD2";      }
        case EMOTION_SAD3:      { return "SAD3";      }
        case EMOTION_ANGRY1:    { return "ANGRY1";    }
        case EMOTION_ANGRY2:    { return "ANGRY2";    }
        case EMOTION_ANGRY3:    { return "ANGRY3";    }
        case EMOTION_SURPRISED: { return "SURPRISED"; }
        case EMOTION_ASHAMED:   { return "ASHAMED";   }
        case EMOTION_STANDBY:   { return "STANDBY";   }
        case EMOTION_EYEBROW:   { return "EYEBROW";   }
        case EMOTION_GREETINGS: { return "GREETINGS"; }
        case EMOTION_1313:      { return "1313";      }
        case EMOTION_EAR:       { return "EAR";       }
        case EMOTION_YES:       { return "YES";       }
        case EMOTION_NO:        { return "NO";        }
        case EMOTION_LOST:      { return "LOST";      }
        case EMOTION_RELAXED:   { return "RELAXED";   }
        case EMOTION_NORMAL:    { return "NORMAL";    }
        case EMOTION_AGITATED:  { return "AGITATED";  }
        case EMOTION_BLINK:     { return "BLINK";     }
        case EMOTION_FLIRT:     { return "FLIRT";     }
        default: {
            return "NONE";
        }
    }
}

void setEmotion(BYTE* emotion) {

    if      ( stricmppgm2ram(emotion, "SERIOUS")   == 0 ) { (&head_state)->emotion = EMOTION_SERIOUS;   }
    else if ( stricmppgm2ram(emotion, "HAPPY")     == 0 ) { (&head_state)->emotion = EMOTION_HAPPY3;    }
    else if ( stricmppgm2ram(emotion, "HAPPY1")    == 0 ) { (&head_state)->emotion = EMOTION_HAPPY1;    }
    else if ( stricmppgm2ram(emotion, "HAPPY2")    == 0 ) { (&head_state)->emotion = EMOTION_HAPPY2;    }
    else if ( stricmppgm2ram(emotion, "HAPPY3")    == 0 ) { (&head_state)->emotion = EMOTION_HAPPY3;    }
    else if ( stricmppgm2ram(emotion, "SAD")       == 0 ) { (&head_state)->emotion = EMOTION_SAD3;      }
    else if ( stricmppgm2ram(emotion, "SAD1")      == 0 ) { (&head_state)->emotion = EMOTION_SAD1;      }
    else if ( stricmppgm2ram(emotion, "SAD2")      == 0 ) { (&head_state)->emotion = EMOTION_SAD2;      }
    else if ( stricmppgm2ram(emotion, "SAD3")      == 0 ) { (&head_state)->emotion = EMOTION_SAD3;      }
    else if ( stricmppgm2ram(emotion, "ANGRY")     == 0 ) { (&head_state)->emotion = EMOTION_ANGRY3;    }
    else if ( stricmppgm2ram(emotion, "ANGRY1")    == 0 ) { (&head_state)->emotion = EMOTION_ANGRY1;    }
    else if ( stricmppgm2ram(emotion, "ANGRY2")    == 0 ) { (&head_state)->emotion = EMOTION_ANGRY2;    }
    else if ( stricmppgm2ram(emotion, "ANGRY3")    == 0 ) { (&head_state)->emotion = EMOTION_ANGRY3;    }
    else if ( stricmppgm2ram(emotion, "SURPRISE")  == 0 ) { (&head_state)->emotion = EMOTION_SURPRISED; }
    else if ( stricmppgm2ram(emotion, "SURPRISED") == 0 ) { (&head_state)->emotion = EMOTION_SURPRISED; }
    else if ( stricmppgm2ram(emotion, "SHAME")     == 0 ) { (&head_state)->emotion = EMOTION_ASHAMED;   }
    else if ( stricmppgm2ram(emotion, "ASHAMED")   == 0 ) { (&head_state)->emotion = EMOTION_ASHAMED;   }
    else if ( stricmppgm2ram(emotion, "STANDBY")   == 0 ) { (&head_state)->emotion = EMOTION_STANDBY;   }
    else if ( stricmppgm2ram(emotion, "EYEBROW")   == 0 ) { (&head_state)->emotion = EMOTION_EYEBROW;   }
    else if ( stricmppgm2ram(emotion, "GREETINGS") == 0 ) { (&head_state)->emotion = EMOTION_GREETINGS; }
    else if ( stricmppgm2ram(emotion, "1313")      == 0 ) { (&head_state)->emotion = EMOTION_1313;      }
    else if ( stricmppgm2ram(emotion, "EAR")       == 0 ) { (&head_state)->emotion = EMOTION_EAR;       }
    else if ( stricmppgm2ram(emotion, "YES")       == 0 ) { (&head_state)->emotion = EMOTION_YES;       }
    else if ( stricmppgm2ram(emotion, "NO")        == 0 ) { (&head_state)->emotion = EMOTION_NO;        }
    else if ( stricmppgm2ram(emotion, "LOST")      == 0 ) { (&head_state)->emotion = EMOTION_LOST;      }
    else if ( stricmppgm2ram(emotion, "RELAXED")   == 0 ) { (&head_state)->emotion = EMOTION_RELAXED;   }
    else if ( stricmppgm2ram(emotion, "NORMAL")    == 0 ) { (&head_state)->emotion = EMOTION_NORMAL;    }
    else if ( stricmppgm2ram(emotion, "AGITATED")  == 0 ) { (&head_state)->emotion = EMOTION_AGITATED;  }
    else if ( stricmppgm2ram(emotion, "BLINK")     == 0 ) { (&head_state)->emotion = EMOTION_BLINK;     }
    else if ( stricmppgm2ram(emotion, "FLIRT")     == 0 ) { (&head_state)->emotion = EMOTION_FLIRT;     }
    else { //NONE
        // Do nothing
        //XXX: set warning
        //(&head_state)->emotion = EMOTION_NONE; // debugging
    }
}

void setMouthState(BYTE* state) {

    if      ( stricmppgm2ram(state, "ON" ) == 0 ) { (&head_state)->mouthEnabled = TRUE;  }
    else if ( stricmppgm2ram(state, "1"  ) == 0 ) { (&head_state)->mouthEnabled = TRUE;  }
    else if ( stricmppgm2ram(state, "OFF") == 0 ) { (&head_state)->mouthEnabled = FALSE; }
    else if ( stricmppgm2ram(state, "0"  ) == 0 ) { (&head_state)->mouthEnabled = FALSE; }
    else { //NONE
        // Do nothing
        //XXX: set warning
        //(&head_state)->emotion = EMOTION_NONE; // debugging
    }
}

void initHeadState() {
    (&head_state)->emotion = EMOTION_SERIOUS;
    (&head_state)->neckYaw = 0; // degrees
    (&head_state)->mouthEnabled = FALSE;
}

/* pre condition:    MPFSInit() is already called. */
// MAC Address Serialization using a MPLAB PM3 Programmer and
// Serialized Quick Turn Programming (SQTP).
// The advantage of using SQTP for programming the MAC Address is it
// allows you to auto-increment the MAC address without recompiling
// the code for each unit.  To use SQTP, the MAC address must be fixed
// at a specific location in program memory.  Uncomment these two pragmas
// that locate the MAC address at 0x1FFF0.  Syntax below is for MPLAB C
// Compiler for PIC18 MCUs. Syntax will vary for other compilers.
//#pragma romdata MACROM=0x1FFF0
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};
//#pragma romdata

static void InitAppConfig(void)
{
    while(1)
    {
        // Start out zeroing all AppConfig bytes to ensure all fields are
        // deterministic for checksum generation
        memset((void*)&AppConfig, 0x00, sizeof(AppConfig));

        AppConfig.Flags.bIsDHCPEnabled = TRUE;
        AppConfig.Flags.bInConfigMode = TRUE;
        memcpypgm2ram((void*)&AppConfig.MyMACAddr, (ROM void*)SerializedMACAddress, sizeof(AppConfig.MyMACAddr));
//        {
//            _prog_addressT MACAddressAddress;
//            MACAddressAddress.next = 0x157F8;
//            _memcpy_p2d24((char*)&AppConfig.MyMACAddr, MACAddressAddress, sizeof(AppConfig.MyMACAddr));
//        }
        AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
        AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
        AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
        AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
        AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
        AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul  | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
        AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul  | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;

        // Load the default NetBIOS Host Name
        memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
        FormatNetBIOSName(AppConfig.NetBIOSName);

        break;
    }
}