#include <stdio.h>
#include <stdlib.h>
#include <p18f96j60.h>
#include <math.h>
#include <i2c.h>
//#include "config.h"

unsigned char sync_mode=0, slew=0, add1,w,data,status,length;
unsigned char ang[6];
int i;

void main(void)
{
	
//CONFIG2L=0b00000100;
OSCCONbits.SCS=0b0;

status = 1;


add1=0xA2; //address of the device (slave) under communication


//---INITIALISE THE I2C MODULE FOR MASTER MODE WITH 100KHz ---

sync_mode = MASTER;
slew = SLEW_OFF;

OpenI2C(sync_mode,slew);

SSPADD=0x0A; //400kHz Baud clock(9) @8MHz

for (i=0; i<5; i++){
    	ang[i]=90;
    }

//check for bus idle condition in multi master communication
IdleI2C();    

//---START I2C---
StartI2C();

while(1){
	while(status!=0){
		status = WriteI2C( add1 | 0x00 ); //write the address of slave
		if(status == -1) //check if bus collision happened
		{
			data = SSPBUF; //upon bus collision detection clear the buffer,
			SSPCON1bits.WCOL=0; // clear the bus collision status bit
		}
	}
	
	//***WRITE THE THE DATA TO BE SENT FOR SLAVE***
	for (i=0;i<5;i++){
		while(WriteI2C(ang[i])!=0); //write a byte of data to be transmitted to slave
	}
	
	//---TERMINATE COMMUNICATION FROM MASTER SIDE---
	IdleI2C();
	
	//---RESTART I2C COMMUNICATION---
	RestartI2C();
	IdleI2C();
	
	//***write the address of the device for communication***
	data = SSPBUF; //read any previous stored content in buffer to clear buffer
	
	for (i=0 ; i<6; i++){
		if (ang[i]==135){
			ang[i]=45;
		  	}
		else if (ang[i]==90){
			ang[i]=135;
		 	}
    	else{
		   	ang[i]=90;
		  	}
	}
		    	
		    

}

}