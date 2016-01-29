//#fuses INTRC_IO,NOMCLR,NOWDT,NOPROTECT,NOLVP,PUT,BROWNOUT // Opciones de configuración
#pragma config WDTE = OFF       // NOWDT
#pragma config CPD = OFF        // NOPROTECT   data memory code protection off
#pragma config MCLRE = OFF      // NOMCLR
#pragma config LVP = OFF        // NOLVP
#pragma config FOSC = (4000000L)  // INTRC_IO   oscilador
#pragma config BOREN = ON       // BROWNOUT
#pragma config PWRTE = ON       // PUT

#include <xc.h>
#include <conio.h>
#include <pic16f628a.h>
#include "uart.h"

#define  Led0   RB0    // PIN_B0
#define  Led1   RB3    // PIN_B3
#define  Led2   RB4    // PIN_B4
#define  Led3   RB5    // PIN_B5
#define  Led4   RB6    // PIN_B6
#define  Led5   RB7    // PIN_B7

// Interfaces para compatibilizar con el compilador antiguo
#define output_high(_X) _X=1
#define output_low(_X) _X=0
#define delay(_X) __delay(_X)
#define delay_ms(_X) __delay_ms(_X)
#define delay_us(_X) __delay_us(_X)


// Variables globales
char comando[3];
char indiceComando=0;
char aux;
unsigned char refbrillo[6];
unsigned char brillo[6];
unsigned char vel[6];
unsigned char espera[6];

/*
    comandos:   255   10n   x  elige el brillo a x porcentaje ( 0 - 254 )del led n
                255   11n   x  elige la velocidad con la que cambia el brillo
                255   120   x  apaga todos los leds
*/

void analizaComando(){
   switch(comando[1])
   {
      case 100:
            refbrillo[0]=comando[2];
            break;
      case  101:
            refbrillo[1]=comando[2];
            break;
      case  102:
            refbrillo[2]=comando[2];
            break;
      case  103:
            refbrillo[3]=comando[2];
            break;
      case  104:
            refbrillo[3]=comando[2];
            break;
      case  105:
            refbrillo[5]=comando[2];
            break;

      case  110:
            vel[0]=comando[2];
            break;
      case  111:
            vel[1]=comando[2];
            break;
      case  112:
            vel[2]=comando[2];
            break;
      case  113:
            vel[3]=comando[2];
            break;
      case  114:
            vel[4]=comando[2];
            break;
      case  115:
            vel[5]=comando[2];
            break;

      case  120:
            vel[0]=0;
            vel[1]=0;
            vel[2]=0;
            vel[3]=0;
            vel[4]=0;
            vel[5]=0;
            refbrillo[0]=0;
            refbrillo[1]=0;
            refbrillo[2]=0;
            refbrillo[3]=0;
            refbrillo[4]=0;
            refbrillo[5]=0;
            brillo[0]=0;
            brillo[1]=0;
            brillo[2]=0;
            brillo[3]=0;
            brillo[4]=0;
            brillo[5]=0;
            break;

      default:
            break;


   }

   return;
}



void serial_isr()
{
   if(UART_Data_Ready())
   {
      aux=UART_Read();
      if(aux==255)
      {
         comando[0]=aux;
         indiceComando=1;
         return;
      }
      else if(indiceComando==1)
      {
         comando[1]=aux;
         indiceComando=2;
         return;
      }
      else if(indiceComando==2)
      {
         comando[2]=aux;
         indiceComando=3;
         analizaComando();
         return;
      }
   }
  return;
 }
 
  


void apagatodo()
{
   output_low(Led0);
   output_low(Led1);
   output_low(Led2);
   output_low(Led3);
   output_low(Led4);
   output_low(Led5);
}

void encendertodo()
{

   output_high(Led0);
   output_high(Led1);
   output_high(Led2);
   output_high(Led3);
   output_high(Led4);
   output_high(Led5);

}

void  main()
{
   int   i=0;
   for(i=0;i<5;++i)
   {
      vel[i]=0;
      brillo[i]=0;
      refbrillo[i]=0;
      espera[i]=0;
   }
   delay_ms(1000);
   apagatodo();
   output_high(Led0);
   delay_ms(1000);
   apagatodo();
   output_high(Led1);
   delay_ms(1000);
   apagatodo();
   output_high(Led2);
   delay_ms(1000);
   apagatodo();
   output_high(Led3);
   delay_ms(1000);
   apagatodo();
   output_high(Led4);
   delay_ms(1000);
   apagatodo();
   output_high(Led5);
   delay_ms(1000);
   apagatodo();
   delay_ms(2000);
   ei();

   // Enable interrupts
   INTCONbits.GIE = 1;
   UART_Init(BAUDRATE);


   while(1)
   {
      serial_isr();
      if(brillo[0]!=0)
         output_high(Led0);
      if(brillo[1]!=0)
         output_high(Led2);
      if(brillo[2]!=0)
         output_high(Led2);
      if(brillo[3]!=0)
         output_high(Led3);
      if(brillo[4]!=0)
         output_high(Led4);
      if(brillo[5]!=0)
         output_high(Led5);

      for(i=0;i<255;++i)
      {
         if(brillo[0]==i)
            output_low(Led0);
         if(brillo[1]==i)
            output_low(Led1);
         if(brillo[2]==i)
            output_low(Led2);
         if(brillo[3]==i)
            output_low(Led3);
         if(brillo[4]==i)
            output_low(Led4);
         if(brillo[5]==i)
            output_low(Led5);
         delay_us(1);
      }
      for(i=0;i<6;++i)
      {
         if(espera[i]!=0)
            espera[i]--;
         if(brillo[i]!=refbrillo[i])
         {
            if(espera[i]==0)
            {
               espera[i]=vel[i];
               if(brillo[i]>refbrillo[i])
               {
                  --brillo[i];
               }
               else
               {
                  ++brillo[i];
               }
            }
         }
      }
   }
}

int test()
{
  INTCONbits.GIE = 1;
  TRISB = 0xFF; //PORTB as Input
  UART_Init(9600);
  TRISB4=0;  //puerto B4 como salida
  TRISB5=0;   // puerto B5 como salida


  do {
        while(!RCIF)
        {
            RB5 = 1;
        }
        RB5 = 0;
        char data = RCREG;
        if(data==0x01) RB4 = 1;
        else RB4 = 0;
  }
  while(1);
}