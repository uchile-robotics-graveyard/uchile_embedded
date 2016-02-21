/* NOTES:
* Code uses the Peripheral library support available with MCC18 Compiler
* Code Tested on:
* PicDem2+ demo board with PIC18F4685 controller
*
* signal to be compared are applied to CxIN+ & CxIN- and output is tapped at CxOUT
**********************************************************************/
#define USE_OR_MASKS
#include <p18cxxx.h>
#include "i2c.h"
unsigned char I2C_Send[21] = "MICROCHIP:I2C_SLAVE" ;
unsigned char I2C_Recv[21];
//************ I2C SLAVE ****************************************
void main(void)
{
unsigned char sync_mode=0, slew=0, add1,status,temp,w,length=0;
for(w=0;w<20;w++)
I2C_Recv[w]=0;
CloseI2C(); //close i2c if was operating earlier
//---INITIALISE THE I2C MODULE FOR MASTER MODE WITH 100KHz ---
sync_mode = SLAVE_7;
slew = SLEW_OFF;
OpenI2C(sync_mode,slew);
SSPADD = 0xA2; //initialze slave address
//**** Read the address sent by master from buffer ***
while(DataRdyI2C()==0); //WAIT UNTILL THE DATA IS TRANSMITTED FROM master
temp = ReadI2C();
//**** Data reception from master by slave ***
do
{
while(DataRdyI2C()==0); //WAIT UNTILL THE DATA IS TRANSMITTED FROM master
I2C_Recv[length++]=getcI2C(); // save byte received
}
while(length!=20);
//*** write sequence from slave ***
while(SSPSTATbits.S!=1); //wait untill STOP CONDITION
//*** Read the address sent by master from buffer ***
while(DataRdyI2C()==0); //WAIT UNTILL THE DATA IS TRANSMITTED FROM master
temp = ReadI2C();
//*** Slave transmission ***
if(SSPSTAT & 0x04) //check if master is ready for reception
while(putsI2C(I2C_Send)); // send the data to master
//---TERMINATE COMMUNICATION FROM MASTER SIDE---
CloseI2C(); //close I2C module
while(1); //End of program
}