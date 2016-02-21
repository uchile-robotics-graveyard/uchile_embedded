/* CustomHTTPApp.c
 *
 *     matias.pavez.b@gmail.com
 */
#define __CUSTOMHTTPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_HTTP2_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include "bender_face.h"        // Needed for SaveAppConfig() prototype

/* GET request handlers */
HTTP_IO_RESULT HTTPExecuteGet(void)
{
	BYTE *ptr;
	BYTE filename[20];

	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 20);

	// If its the control.html page
	if(!memcmppgm2ram(filename, "control.html", 9)) // XXX: 12 chars
	{
		// Seek out each of the available arguments
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"emotion");
		if(ptr) { setEmotion(ptr); }

		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"yaw");
		if(ptr) { setNeckYaw( atoi(ptr) ); }

                ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"mouth");
		if(ptr) { setMouthState(ptr); }
	}

        
	return HTTP_IO_DONE;
}

/*
 *  DYNAMIC VARIABLE CALLBACK FUNCTIONS
 */

void HTTPPrint_debug_string(void) {
    //TCPPutString(sktHTTP, "Please, use me wisely");
}

// - - - - Control states data - - - -

void HTTPPrintInteger(int number) {
    // only valid for integers with up to 10 digits
    BYTE string[11];
    itoa(number, string);
    TCPPutString(sktHTTP, string);
}

void HTTPPrint_yaw_max(void)
{
    HTTPPrintInteger(BENDER_NECK_MAX_YAW);
}
void HTTPPrint_yaw_min(void)
{
    HTTPPrintInteger(BENDER_NECK_MIN_YAW);
}
void HTTPPrint_yaw(void)
{
    HTTPPrintInteger(getNeckYaw());
}
void HTTPPrint_emotion(void)
{
    TCPPutROMString(sktHTTP, (ROM void*)getHeadEmotionStr() );
}
void HTTPPrint_mouth(void)
{
    if (isMouthEnabled() == TRUE) {
        TCPPutROMString(sktHTTP, "ON");
    } else {
        TCPPutROMString(sktHTTP, "OFF");
    }
}

void HTTPPrint_btn(WORD num)
{
    // Determine which button
    switch(num)
    {
        case 0:
            num = BUTTON0_IO;
            break;
        case 1:
            num = BUTTON1_IO;
            break;
        case 2:
            num = BUTTON2_IO;
            break;
        case 3:
            num = BUTTON3_IO;
            break;
        default:
        num = 0;
    }
    
    // Print the output
    if(num == 1)
        TCPPutROMString(sktHTTP, "up");
    else
        TCPPutROMString(sktHTTP, "down");
}

// - - - Board data - - - -
void HTTPPrint_builddate(void)
{
    curHTTP.callbackPos = 0x01;
    if(TCPIsPutReady(sktHTTP) < strlenpgm((ROM char*)__DATE__" "__TIME__))
        return;

    curHTTP.callbackPos = 0x00;
    TCPPutROMString(sktHTTP, (ROM void*)__DATE__" "__TIME__);
}

void HTTPPrint_version(void)
{
    TCPPutROMString(sktHTTP, (ROM void*)TCPIP_STACK_VERSION);
}

// - - - - Network data - - - -
extern APP_CONFIG AppConfig;

void HTTPPrint_config_hostname(void) {
    TCPPutString(sktHTTP, AppConfig.NetBIOSName);
    return;
}

void HTTPPrintIP(IP_ADDR ip)
{
    BYTE digits[4];
    BYTE i;

    for(i = 0; i < 4u; i++)
    {
        if(i) {
            TCPPut(sktHTTP, '.');
        }       
        uitoa(ip.v[i], digits);
        TCPPutString(sktHTTP, digits);
    }
}
void HTTPPrint_config_ip(void)     { HTTPPrintIP(AppConfig.MyIPAddr);           return; }
void HTTPPrint_config_gw(void)     { HTTPPrintIP(AppConfig.MyGateway);          return; }
void HTTPPrint_config_subnet(void) { HTTPPrintIP(AppConfig.MyMask);             return; }
void HTTPPrint_config_dns1(void)   { HTTPPrintIP(AppConfig.PrimaryDNSServer);   return; }
void HTTPPrint_config_dns2(void)   { HTTPPrintIP(AppConfig.SecondaryDNSServer); return; }

void HTTPPrint_config_mac(void)
{
    BYTE i;

    if(TCPIsPutReady(sktHTTP) < 18u)
    {//need 17 bytes to write a MAC
        curHTTP.callbackPos = 0x01;
        return;
    }

    // Write each byte
    for(i = 0; i < 6u; i++)
    {
        if(i) {
            TCPPut(sktHTTP, ':');
        }
        TCPPut(sktHTTP, btohexa_high(AppConfig.MyMACAddr.v[i]));
        TCPPut(sktHTTP, btohexa_low(AppConfig.MyMACAddr.v[i]));
    }

    // Indicate that we're done
    curHTTP.callbackPos = 0x00;
    return;
}


// - - - - other old - - - -
/*
void HTTPPrint_reboot(void) {
    // This is not so much a print function, but causes the board to reboot
    // when the configuration is changed.  If called via an AJAX call, this
    // will gracefully reset the board and bring it back online immediately
    Reset();
}

void HTTPPrint_rebootaddr(void) {
    // This is the expected address of the board upon rebooting
    TCPPutString(sktHTTP, curHTTP.data);
}
*/
/*
void HTTPPrint_status_ok(void)
{
	if(lastSuccess)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"block");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"none");
	lastSuccess = FALSE;
}

void HTTPPrint_status_fail(void)
{
	if(lastFailure)
		TCPPutROMString(sktHTTP, (ROM BYTE*)"block");
	else
		TCPPutROMString(sktHTTP, (ROM BYTE*)"none");
	lastFailure = FALSE;
}
*/
#endif

