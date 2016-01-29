/* 
 * File:   uart.h
 * Author: Robotica
 *
 * Created on 29 de enero de 2016, 07:55 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

// configuraciones de comunicaciones
//#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1) // Definición del RS232
#define FOSC (4000000L)
#define _XTAL_FREQ FOSC//#use delay(clock=4000000) // Velocidad del Cristal : 4 Mhz
#define FCYC (FOSC/4L)
#define BAUDRATE 9600L

char UART_Init(const long int baudrate)
{
  unsigned int x;
  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);   //SPBRG for Low Baud Rate
  if(x>255)                                       //If High Baud Rage Required
  {
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
    BRGH = 1;                                     //Setting High Baud Rate
  }
  if(x<256)
  {
    SPBRG = x;                                    //Baudrate config Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    // Output port
    TRISB1 = 1;                                   //As Prescribed in Datasheet
    TRISB2 = 1;                                   //As Prescribed in Datasheet
    CREN = 1;                                     //Enables Continuous Reception
    TXEN = 1;                                     //Enables Transmission
    return 1;                                     //Returns 1 to indicate Successful Completion
  }
  return 0;                                       //Returns 0 to indicate UART initialization failed
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}


char UART_TX_Empty()
{
  return TRMT;
}


void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write(text[i]);
}

char UART_Data_Ready()
{
  return RCIF;
}

char UART_Read()
{
  while(!RCIF);
  return RCREG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
  unsigned int i;
  for(int i=0;i<length;i++)
  Output[i] = UART_Read();
}


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

