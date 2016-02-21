#include <p18F66K90.h>
#include <GenericTypeDefs.h>

#define USE_OR_MASKS
#include <pwm.h>

#include <i2c.h>

/*
 * PWM period = [(PR2 ) + 1] x 4 x Tosc x TMR2 prescaler
 * PWM freq = 1/[PWM period]
 * PWM x Duty cycle = (DCx<9:0>) x Tosc x TMR2 prescaler
 * PWM Resolution bits (max) = log(Fosc/Fpwm) / log(2) [bits]
 */

//#pragma config FOSC = INTIO1
//#pragma config PWRTEN = OFF
//
//#pragma config DEBUG = ON

#pragma config CCP2MX = PORTBE //ECCP2 en pin RE7
#pragma config WDTEN  = OFF    // No WatchDogTimer (WDT disabled in hardware; SWDTEN bit disabled)

// oscillator configs
#pragma config FOSC   = INTIO2
//#pragma config PLLCFG = ON    // PhaseLockedLoop Conf.Bit Ignored!
                                // prefer soft-config: OSCTUNE PLLEN.


INT16 duty_cycle[10]; // control command
INT16 duty_ref[10];   // control reference
INT16 histeresis_step = 3; // Histeresis step width: move..-H..0..+X..move

void main(void)
{
    char period=0x00;
    unsigned char outputconfig=0,outputmode=0,config=0;
    unsigned char i=0, j=0;
    INT16 dc_error=0;

    //----init oscillator----
    // OSCCON: osc. control (type, power modes, freq, status)
    OSCCONbits.IRCF = 0b011;  // 0b(111|110|101|100|011):(16|8|4|2|1)[MHz]
    OSCCONbits.SCS  = 0b10;   // use internal oscillator

    // OSCTUNE: int. osc. tuning + PLL
    //OSCTUNEbits.PLLEN = 1;    // PLL enable (requires IRCF 4-16MHz)
    OSCTUNEbits.TUN = 0;   // Run Fast RC oscillator at the calibrated frequency.

    //T2CONbits.T2CKPS = 0b11;  // Timer2 prescaler, 16

    //----init pwm----
    for(i=0;i<10;i++)
    {
        //duty_cycle = 0x0F00; // 0x0F00=3840. 0x00FF=255
        duty_cycle[i]  = 0x00FF; // 88
        duty_ref[i] = 0x00FF;    // 88
    }

    //----Configure pwm ----
    period = 0xFF; //periodo 128.4 us
    outputconfig = SINGLE_OUT; // FULL_OUT_FWD ; // SINGLE_OUT
    outputmode = PWM_MODE_1;

    //Configure PWM modules and initialize PWM period
    OpenEPWM1(period, ECCP_1_SEL_TMR12);
    OpenEPWM2(period, ECCP_2_SEL_TMR12);
    OpenEPWM3(period, ECCP_3_SEL_TMR12);
    OpenPWM4(period, CCP_4_SEL_TMR12);
    OpenPWM5(period, CCP_5_SEL_TMR12);
    OpenPWM6(period, CCP_6_SEL_TMR12);
    OpenPWM7(period, CCP_7_SEL_TMR12);
    OpenPWM8(period, CCP_8_SEL_TMR12);
    OpenPWM9(period, CCP_9_SEL_TMR12);
    OpenPWM10(period, CCP_10_SEL_TMR12);

    //-----set duty cycle and EPWM as output----
    // (ordered by servo-number)
    SetDCPWM9 (duty_cycle[0]);
    SetDCPWM8 (duty_cycle[1]);
    SetDCPWM7 (duty_cycle[2]);
    SetDCPWM6 (duty_cycle[3]);
    SetDCEPWM2(duty_cycle[4]); SetOutputEPWM2( outputconfig, outputmode);
    SetDCEPWM3(duty_cycle[5]); SetOutputEPWM3( outputconfig, outputmode);
    SetDCPWM4 (duty_cycle[6]);
    SetDCPWM5 (duty_cycle[7]);
    SetDCPWM10(duty_cycle[8]);
    SetDCEPWM1(duty_cycle[9]); SetOutputEPWM1( outputconfig, outputmode);
    

    while(1) {

        // compute control commands
        for (i=0; i<10; ++i) {

            dc_error = duty_cycle[i] - duty_ref[i];
            if( dc_error >= 0 ) {    
                if ( dc_error < histeresis_step ) {
                    duty_cycle[i] = duty_ref[i];
                } else {
                    duty_cycle[i]-=histeresis_step;
                }
            } else {
                if ( -dc_error < histeresis_step ) {
                    duty_cycle[i] = duty_ref[i];
                } else {
                    duty_cycle[i]+=histeresis_step;
                }
            }
        }

        SetDCPWM9 (duty_cycle[0]); // Servo 1
        /*SetDCPWM8 (duty_cycle[1]); // Servo 2
        SetDCPWM7 (duty_cycle[2]); // Servo 3
        SetDCPWM6 (duty_cycle[3]); // Servo 4
        SetDCEPWM2(duty_cycle[4]); // Servo 5
        SetDCEPWM3(duty_cycle[5]); // Servo 6
        SetDCPWM4 (duty_cycle[6]); // Servo 7
        SetDCPWM5 (duty_cycle[7]); // Servo 8
        SetDCPWM10(duty_cycle[8]); // Servo 9
        SetDCEPWM1(duty_cycle[9]); // Servo 10*/

        /*
        for(j=0;j<3;j++) {
            for(i=0;i<255;i++) {
                Nop();
            }
        }*/
    }

}
