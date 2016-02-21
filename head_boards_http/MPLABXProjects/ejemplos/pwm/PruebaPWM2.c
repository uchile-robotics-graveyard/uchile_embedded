#define USE_OR_MASKS

#pragma config PLLCFG = ON  // ??
#pragma config DEBUG= OFF


/**************** Init variables **********************************************/
// Function prototypes for the high priority ISRs
void highPriorityISR(void);

// Function prototype for the SSP ISR
void SSPISR(void);

// This is the code for at the high priority vector
#pragma code high_vector=0x08
void interrupt_at_high_vector(void) { _asm GOTO highPriorityISR _endasm }
#pragma code

// The actual high priority ISR
#pragma interrupt highPriorityISR
void highPriorityISR() {
    if (PIR1bits.SSP1IF) {        // Check for SSP interrupt
        SSPISR();            // It is an SSP interrupt, call the SSP ISR
        PIR1bits.SSP1IF = 0;  // Clear the interrupt flag
    }
    return;
}

// This is the actual SSP ISR
void SSPISR(void) {
	static int index =0;
	static unsigned char x[6];
	unsigned int data, motor_id;
	if((SSP1STATbits.BF == 1) && (SSP1STATbits.D_A == 0) && (SSP1STATbits.R_W == 0))		//buffer full, write and an address
	{
		motor_id = ReadI2C1();//SSP1BUF;
		x[index]=motor_id;index++;

		while(SSPSTATbits.BF == 0)	//wait for next char
		{
			
		}
		motor_id = ReadI2C1();//SSP1BUF;
		x[index]=motor_id;	index++;

		while(SSPSTATbits.BF == 0)	//wait for next char
		{
			
		}
		data=0;
		data = ReadI2C1();//SSP1BUF;
		x[index]=data;index++;
		
		//----Transf data-------
		//data = 96*data/256 + 40;
		data*=96;
		data/=255;
		data+=40;
		//----------------------
		if(motor_id<10)
		{
			duty_objec[motor_id]=data;
			step[motor_id]=3;
		}else if(motor_id<20)
		{
			duty_objec[motor_id-10]=data;
			duty_cycle[motor_id-10]=data;
		}	
		
	}
}


void main(void)
{

/**************** Init variables **********************************************/
	char period=0x00;
	unsigned char outputconfig=0,outputmode=0,config=0;
	unsigned char i=0, j=0;
	INT16 test=0;
	unsigned char value;
/**************** Init OSC ****************************************************/
	OSCCONbits.IRCF = 0b011; //1MHz //0b100; //2MHZ // 0b111
	OSCCONbits.SCS  = 0b10;	 //OSC interno // 0b11
	OSCTUNEbits.PLLEN = 1;   //PLL enable
	OSCTUNEbits.TUN = 0;	 //Calibracion centrada.
	
	T2CONbits.T2CKPS = 0b11; //T2 prescaler em 16
/****************** Init i2c **************************************************/
	
	//SSP1CON1 = 0x36;		//enable I2C, release clock, slave mode
	//SSP1CON1bits.SSPM = 0b0110;
	SSP1CON1 = 0x36;
	SSP1ADD = 0x80; //SSPADD contains I2C device address in SLAVE mode
	SSP1STAT = 0;
	RCONbits.IPEN = 1;          // Enable interrupt priorities
	//INTCON &= 0x3f;             // Globally enable interrupts
	INTCONbits.PEIE = 1;          //enable interrupts
	INTCONbits.GIE = 1;          //enable interrupts
	PIE1bits.SSP1IE = 1;         // Enable SSP interrupt
	IPR1bits.SSP1IP = 1;         // Set SSP interrupt priority to high

/****************** Init PWM **************************************************/
	for(i=0;i<10;i++)
	{
		duty_cycle[i]=0x0058;
		duty_objec[i]=0x0058;
		step[i]=3;
	}	

//----ECCP3-------
SetDCPWM9(duty_cycle[0]);//9);//M0
SetDCPWM8(duty_cycle[1]);//8);//M1
SetDCPWM7(duty_cycle[2]);//7);//M2
SetDCPWM6(duty_cycle[3]);//6);//M3
SetDCEPWM2(duty_cycle[4]);//2);//M4
SetDCEPWM3(duty_cycle[5]);//3);//M5
SetDCPWM4(duty_cycle[6]);//4);//M6
SetDCPWM5(duty_cycle[7]);//5);//M7
SetDCPWM10(duty_cycle[8]);//10);//M8
SetDCEPWM1(duty_cycle[9]);//1);//M9
//----Configure pwm ----

//-------PERIOD---------
	period =0xFF; //periodo 128.4 us
	outputconfig = SINGLE_OUT; // FULL_OUT_FWD;//SINGLE_OUT ;
	outputmode = PWM_MODE_1;

//------ CCP9 ------------- S1 M0
	OpenPWM9(period, CCP_9_SEL_TMR12); 
	SetDCPWM9(duty_cycle[0]);
//------ CCP8 ------------- S2
	OpenPWM8(period, CCP_8_SEL_TMR12); 
	SetDCPWM8(duty_cycle[1]);
//------ CCP7 ------------- S3
	OpenPWM7(period, CCP_7_SEL_TMR12); 
	SetDCPWM7(duty_cycle[2]);
//------ CCP6 ------------- S4
	OpenPWM6(period, CCP_6_SEL_TMR12); 
	SetDCPWM6(duty_cycle[3]);
//------ ECCP2 ------------ S5
	OpenEPWM2(period, ECCP_2_SEL_TMR12); 
	SetDCEPWM2(duty_cycle[4]);
	SetOutputEPWM2( outputconfig, outputmode);
//------ ECCP3 ------------ S6
	OpenEPWM3( period, ECCP_3_SEL_TMR12); //Configure PWM module and initialize PWM period
	SetDCEPWM3(duty_cycle[5]); //set the duty cycle
	SetOutputEPWM3( outputconfig, outputmode); //output PWM in respective modes
//------ CCP4 ------------- S7
	OpenPWM4(period, CCP_4_SEL_TMR12); 
	SetDCPWM4(duty_cycle[6]);
//------ CCP5 ------------- S8
	OpenPWM5(period, CCP_5_SEL_TMR12); 
	SetDCPWM5(duty_cycle[7]);

//------ CCP10 ------------- S9
	OpenPWM10(period, CCP_10_SEL_TMR12); 
	SetDCPWM10(duty_cycle[8]);
//------ ECCP1 ------------ S10
	OpenEPWM1(period, ECCP_1_SEL_TMR12); 
	SetDCEPWM1(duty_cycle[9]);
	SetOutputEPWM1( outputconfig, outputmode);

	while(1)
	{
		for(i=0;i<10;i++)
		{
			test=(INT16)(duty_objec[i])- (INT16)(duty_cycle[i]);
			if((duty_cycle[i]==duty_objec[i]))
				continue;
			if(duty_cycle[i]<duty_objec[i])
			{
				if(duty_objec[i]-duty_cycle[i]>step[i])
					duty_cycle[i]+=step[i];
				else
					duty_cycle[i]=duty_objec[i];
			}else{
				if(duty_cycle[i]-duty_objec[i]>step[i])
					duty_cycle[i]-=step[i];
				else
					duty_cycle[i]=duty_objec[i];
			}	
		}
			
		SetDCPWM9(duty_cycle[0]);//M0
		SetDCPWM8(duty_cycle[1]);//M1
		SetDCPWM7(duty_cycle[2]);//M2
		SetDCPWM6(duty_cycle[3]);//M3
		SetDCEPWM2(duty_cycle[4]);//M4
		SetDCEPWM3(duty_cycle[5]);//M5
		SetDCPWM4(duty_cycle[6]);//M6
		SetDCPWM5(duty_cycle[7]);//M7
		SetDCPWM10(duty_cycle[8]);//M8
		SetDCEPWM1(duty_cycle[9]);//M9
		for(j=0;j<3;j++)
			for(i=0;i<255;i++)
				Nop();
	} 
}
