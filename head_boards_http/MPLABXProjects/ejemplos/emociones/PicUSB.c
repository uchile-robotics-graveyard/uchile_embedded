

#include <18F4550.h>
#DEVICE ADC=8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
#use delay(clock=48000000)

#define USB_HID_DEVICE     FALSE             //deshabilitamos el uso de las directivas HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK   //turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    1                 //size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    3                 //size to allocate for the rx endpoint 1 buffer

//Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include <pic18_usb.h>
#include <PicUSB.h>         //Configuración del USB y los descriptores para este dispositivo
#include <usb.c>           //handles usb setup tokens and get descriptor reports

#define MAX(A,B) ((A)>(B))? ((A):(B))
#define MIN(A,B) ((A)<(B))? ((A):(B))

//Declaración puerto serial

#define RAND_MAX 10
#include <stdlib.h>
#include <usb_bootloader.h>
#define LAT_B PIN_D5

//funciones
void emocion(int arreglo[], int delay); //(arreglo de 96)
void resetear(int arreglo[]);
//Emociones
void sadness(int n, int r); //n=nivel; r=repetición 
void anger(int n, int r);
void happiness(int n, int r);
void shamed(int n, int r);
void surprise(int n, int r);
void standby(int n, int r);

//declaracion de variables
int cara[96];

//CICLO PRINCIPAL
void main(void)
{
   output_high(PIN_D4);     //en_b
   output_low(PIN_B0);    //rst_b 
   setup_spi(spi_master |spi_l_to_h |spi_clk_div_16 ); // inicializa el SPI, definido como master, low to high, 16 bit
  
   output_high(LAT_B); // pin correspondiente al LAT_B
   delay_us(1);
   output_low(LAT_B);
   
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_1, 8, 1);
   set_pwm1_duty(4);
   
   output_high(PIN_D6);    //sel_bk 
//pruebas

resetear(cara);
//emocion(cara,500);
//anger(1,5);
//delay_ms(1000);
//anger(2,5);
//delay_ms(1000);
//anger(3,5);
//resetear(cara);
//emocion(cara,500);

//delay_ms(2000);
//sadness(1,3);
//delay_ms(1000);
//sadness(2,7);
//delay_ms(1000);
//sadness(3,7);
//delay_ms(1000);
//shamed(1,5);
//delay_ms(1500);
//resetear(cara);
//standby(1,3);
//delay_ms(1500);
//resetear(cara);
//surprise(1,5);
//delay_ms(1500);
//resetear(cara);
//happiness(1,5);
//delay_ms(1500);
//resetear(cara);
//happiness(2,5);
//delay_ms(1500);
//resetear(cara);
happiness(3,5);

} //fin main
//funciones auxiliares
void emocion(int arreglo[], int delay)
{
   int i; 
   for(i=0;i<96;i++)
   {
      spi_write(arreglo[i]);
   }  
   output_high(LAT_B); // pin correspondiente al LAT_B
   delay_us(1000);
   output_low(LAT_B);
   output_low(PIN_D4);     //en_b
   delay_ms(delay);  //delay en segundos
}
void resetear(int arreglo[])
{
   int j; 
    for(j=0;j<96;j++) //resetea el arreglo para iniciar la emocion
    {
       arreglo[j]=0;
    }
}
//funciones: emociones
void anger(int n, int r)      
{
 int w;
 int j;
     w=15;
 resetear(cara); 
 if(n==1)               //anger 1
 {
  while(r>0,r--)
  {
   for(j=0;j<6;j++)      //frame 1
   {
   w=w+5;
      cara[2]=w;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[95]=w;
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;
   emocion(cara,100);
   }  
   for(j=0;j<6;j++)      //frame 2
   {
   w=w-5;
      cara[2]=w;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[95]=w;
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;
   emocion(cara,100);
   }
   resetear(cara); 
  }
 } //fin if(n==1)
 if(n==2)               //anger 2
 {
  while(r>0,r--)
  {
   for(j=0;j<2;j++)      //frame 1
   {
   w=w+10;
      cara[2]=w;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[95]=w;
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;
   emocion(cara,100);
   }
   for(j=0;j<2;j++)      //frame 2
   {
   w=w-10;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;     //cambio de led
      cara[38]=w;
      cara[59]=w;
      cara[95]=0;
      cara[2]=0;
   emocion(cara,100);
   }
   resetear(cara);
  }
 } //fin if(n==2)
 if(n==3)               //anger 3
 {
  while(r>0,r--)
  {
   for(j=0;j<7;j++)      //frame 1
   {
   w=w+5;
      cara[2]=w;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[95]=w;
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;
   emocion(cara,100);
   }
   for(j=0;j<7;j++)      //frame 2
   {
   w=w-5;
      cara[2]=w;
      cara[50]=w;
      cara[53]=w;
      cara[56]=w;
      cara[41]=w;
      cara[44]=w;
      cara[47]=w;
      cara[95]=w;   
      cara[26]=w;
      cara[29]=w;
      cara[68]=w;
      cara[71]=w;
   emocion(cara,100);
   }
   resetear(cara);
   for(j=0;j<7;j++)      //frame 3
   {
   w=w+5; 
      cara[5]=w;
      cara[8]=w;
      cara[11]=w;
      cara[86]=w;
      cara[89]=w;
      cara[92]=w;
      cara[59]=w;
      cara[62]=w;
      cara[65]=w;
      cara[32]=w;
      cara[35]=w;
      cara[38]=w;
   emocion(cara,100);
   }
   for(j=0;j<7;j++)      //frame 4
   {
   w=w-5;
      cara[5]=w;
      cara[8]=w;
      cara[11]=w;
      cara[86]=w;
      cara[89]=w;
      cara[92]=w;
      cara[59]=w;
      cara[62]=w;
      cara[65]=w;
      cara[32]=w;
      cara[35]=w;
      cara[38]=w;
   emocion(cara,100);
   }
   resetear(cara);
  }
 }  //fin if(n==3)   
} //fin void anger
void sadness(int n, int r)
{
 int w;
 int d;
     w=50;
 resetear(cara); 
 if(n==1)     //sadness 1
 {
  while(r>0,r--)
  {
   w=30;
   cara[6]=w+5;
   cara[9]=w-5;
   cara[87]=w+5;
   cara[84]=w-5;
   
   cara[24]=w+5;
   cara[27]=w-5;
   cara[66]=w+5;
   cara[69]=w-5;
   emocion(cara,300);
   cara[6]=w+5;
   cara[9]=w-5;
   cara[87]=w+5;
   cara[84]=w-5;
   
   cara[24]=w+5;
   cara[27]=w-5;
   cara[66]=w+5;
   cara[69]=w-5;
   emocion(cara,300);
   cara[6]=w+5;
   cara[9]=w-5;
   cara[87]=w+5;
   cara[84]=w-5;
   
   cara[24]=w+5;
   cara[27]=w-5;
   cara[66]=w+5;
   cara[69]=w-5;
   emocion(cara,300);
    cara[6]=w+5;
   cara[9]=w-5;
   cara[87]=w+5;
   cara[84]=w-5;
   
   cara[24]=w+5;
   cara[27]=w-5;
   cara[66]=w+5;
   cara[69]=w-5;
   emocion(cara,300);
   w=30;
    cara[6]=w-5;
   cara[9]=w+5;
   cara[87]=w-5;
   cara[84]=w+5;
   
   cara[24]=w-5;
   cara[27]=w+5;
   cara[66]=w-5;
   cara[69]=w+5;
   emocion(cara,300);
     cara[6]=w-5;
   cara[9]=w+5;
   cara[87]=w-5;
   cara[84]=w+5;
   
   cara[24]=w-5;
   cara[27]=w+5;
   cara[66]=w-5;
   cara[69]=w+5;
   emocion(cara,300);
   cara[6]=w-5;
   cara[9]=w+5;
   cara[87]=w-5;
   cara[84]=w+5;
   
   cara[24]=w-5;
   cara[27]=w+5;
   cara[66]=w-5;
   cara[69]=w+5;
   emocion(cara,300);
   cara[6]=w-5;
   cara[9]=w+5;
   cara[87]=w-5;
   cara[84]=w+5;
   
   cara[24]=w-5;
   cara[27]=w+5;
   cara[66]=w-5;
   cara[69]=w+5;
   emocion(cara,300);
  // delay_ms(5000);
   }
   resetear(cara);
   emocion(cara,500);
 } //fin if(n==1)
 if(n==2)     //sadness 2 
 {
  while(r>0,r--)
  {
  d=350;
      cara[0]=w;
      cara[93]=w;
      cara[54]=w;
      cara[51]=w;
      cara[48]=w;
      cara[39]=w;
      cara[42]=w;
      cara[45]=w;
  emocion(cara,500); 
      cara[72]=0;
      cara[66]=w;
  emocion(cara,500);
  delay_ms(d+100);
     cara[66]=0;
     cara[69]=w;
  emocion(cara,500);
  delay_ms(d+100);
     cara[69]=0;
     cara[78]=w;
  emocion(cara,500);
  delay_ms(d);
     cara[78]=0;
     cara[75]=w;
  emocion(cara,500);
  delay_ms(d);
     cara[75]=0;
     cara[72]=w;
  emocion(cara,500); 
  delay_ms(d);
  }
  resetear(cara);
  emocion(cara,1000);
 } //fin if(n==2)
 if(n==3)     //sadness 3 
 {
  while(r>0,r--)
  {
   int d; 
       d=200;
     cara[54]=w;
     cara[51]=w;
     cara[48]=w;
     cara[39]=w;
     cara[42]=w;
     cara[45]=w;
   emocion(cara,500);      
     cara[21]=0;
     cara[6]=w;
     cara[27]=w;      
     cara[72]=0;
     cara[66]=w;
     cara[87]=w;
   emocion(cara,500);
   delay_ms(d+100);
     cara[27]=0;
     cara[24]=w;
     cara[6]=0;
     cara[9]=w;
     cara[66]=0;
     cara[69]=w;
     cara[87]=0;
     cara[84]=w;
   emocion(cara,500);
   delay_ms(d+100);
     cara[24]=0;
     cara[9]=0;
     cara[15]=w;
     cara[69]=0;
     cara[84]=0;
     cara[78]=w;
   emocion(cara,300);
   delay_ms(d);
     cara[15]=0;
     cara[18]=w;
     cara[78]=0;
     cara[75]=w;
   emocion(cara,300);
   delay_ms(d);
     cara[18]=0;
     cara[21]=w;
     cara[75]=0;
     cara[72]=w;
   emocion(cara,300); 
   delay_ms(d);
  }
  resetear(cara);
  emocion(cara,1000);
 }
} //fin sadness
void shamed(int n, int r)
{
 int w;
 int j;
     w=20;
 resetear(cara); 
 if(n==1)           //shamed 1
 {   
  while(r>0,r--)
  {
   //cara[53]=w;
   //cara[44]=w;
   //cara[50]=w;
   //cara[47]=w;
   //cara[2]=w;
   //cara[95]=w;
   for(j=0;j<15;j++)
   {
   w=w+10;
      cara[81]=w;
      cara[83]=w;
      cara[12]=w;
      cara[14]=w;
   emocion(cara,500);
   }
   for(j=0;j<15;j++)
   {
   w=w-10;
      cara[81]=w;
      cara[83]=w;
      cara[12]=w;
      cara[14]=w;
   emocion(cara,500);
   }
  }
  resetear(cara);
  emocion(cara,500);
 }
}// fin shamed
void standby(int n, int r)
{
 int w;
 int j;
 resetear(cara); 
 if(n==1)           //standby 1
 {
 while(r>0,r--)
  {
   w=10;
   for(j=0;j<5;j++)
   {
   w=w+5;
      cara[91]=w;
      cara[85]=w;
      cara[88]=w;
      cara[67]=w;
      cara[70]=w;
      cara[7]=w;
      cara[10]=w;
      cara[25]=w;
      cara[28]=w;
   emocion(cara,300);
   }
   for(j=0;j<3;j++)
   {
   w=30;
      cara[59]=w;
      cara[58]=w/2;
      cara[56]=w;
      cara[55]=w/2;
      cara[53]=w;
      cara[52]=w/2;
      cara[50]=w;
      cara[49]=w/2;
      
      cara[38]=w;
      cara[37]=w/2;
      cara[41]=w;
      cara[40]=w/2;
      cara[44]=w;
      cara[43]=w/2;
      cara[47]=w;
      cara[46]=w/2;
   emocion(cara,700);
   delay_ms(500);
   w=0;
      cara[59]=w;
      cara[58]=w/2;
      cara[56]=w;
      cara[55]=w/2;
      cara[53]=w;
      cara[52]=w/2;
      cara[50]=w;
      cara[49]=w/2;
      
      cara[38]=w;
      cara[37]=w/2;
      cara[41]=w;
      cara[40]=w/2;
      cara[44]=w;
      cara[43]=w/2;
      cara[47]=w;
      cara[46]=w/2;
   emocion(cara,700);
   delay_ms(500);
   }
   w=40;
   for(j=0;j<5;j++)
   {
   w=w-5;
      cara[91]=w;
      cara[85]=w;
      cara[88]=w;
      cara[67]=w;
      cara[70]=w;
      cara[7]=w;
      cara[10]=w;
      cara[25]=w;
      cara[28]=w;;
   emocion(cara,300);
   }
   for(j=0;j<3;j++)
   {
   w=30;
      cara[59]=w;
      cara[58]=w/2;
      cara[56]=w;
      cara[55]=w/2;
      cara[53]=w;
      cara[52]=w/2;
      cara[50]=w;
      cara[49]=w/2;
      
      cara[38]=w;
      cara[37]=w/2;
      cara[41]=w;
      cara[40]=w/2;
      cara[44]=w;
      cara[43]=w/2;
      cara[47]=w;
      cara[46]=w/2;
   emocion(cara,700);
   delay_ms(500);
   w=0;
      cara[59]=w;
      cara[58]=w/2;
      cara[56]=w;
      cara[55]=w/2;
      cara[53]=w;
      cara[52]=w/2;
      cara[50]=w;
      cara[49]=w/2;
      
      cara[38]=w;
      cara[37]=w/2;
      cara[41]=w;
      cara[40]=w/2;
      cara[44]=w;
      cara[43]=w/2;
      cara[47]=w;
      cara[46]=w/2;
   emocion(cara,700);
   delay_ms(500);
   }
  }
  resetear(cara);
  emocion(cara,500);
 }
}// fin standby
void surprise(int n, int r)
{
 int w;
 int j;
     w=10;
 resetear(cara); 
 if(n==1)           //sorprise 1
 {
 while(r>0,r--)
  {
  for(j=0;j<2;j++)
   {
      cara[3*j+1]=w;
      cara[3*j+2]=w;
      cara[3*j+4]=w+10;
      cara[3*j+5]=w+10;
      cara[3*j+7]=w+20;
      cara[3*j+8]=w+20;
      
      cara[94-3*j]=w;
      cara[95-3*j]=w;
      cara[91-3*j]=w+10;
      cara[92-3*j]=w+10;
      cara[88-3*j]=w+20;
      cara[89-3*j]=w+20;
   emocion(cara,300);
   resetear(cara);
   }
      cara[7]=w;
      cara[8]=w;
      cara[10]=w+10;
      cara[11]=w+10;
      cara[25]=w+20;
      cara[26]=w+20;
      
      cara[88]=w;
      cara[89]=w;
      cara[85]=w+10;
      cara[86]=w+10;
      cara[70]=w+20;
      cara[71]=w+20;
   emocion(cara,300);
   resetear(cara);
      cara[10]=w;
      cara[11]=w;
      cara[25]=w+10;
      cara[26]=w+10;
      cara[28]=w+20;
      cara[29]=w+20;
      
      cara[85]=w;
      cara[86]=w;
      cara[70]=w+10;
      cara[71]=w+10;
      cara[67]=w+20;
      cara[68]=w+20;
   emocion(cara,300);
   resetear(cara);
   for(j=8;j<14;j++)
   {
      cara[3*j+1]=w;
      cara[3*j+2]=w;
      cara[3*j+4]=w+10;
      cara[3*j+5]=w+10;
      cara[3*j+7]=w+20;
      cara[3*j+8]=w+20;
      
      cara[94-3*j]=w;
      cara[95-3*j]=w;
      cara[91-3*j]=w+10;
      cara[92-3*j]=w+10;
      cara[88-3*j]=w+20;
      cara[89-3*j]=w+20;
   emocion(cara,300);
   resetear(cara);
   }
      cara[43]=w;
      cara[44]=w;
      cara[46]=w+10;
      cara[47]=w+10;
      cara[1]=w+20;
      cara[2]=w+20;
      
      cara[52]=w;
      cara[53]=w;
      cara[49]=w+10;
      cara[50]=w+10;
      cara[94]=w+20;
      cara[95]=w+20;
   emocion(cara,300);
   resetear(cara);
      cara[46]=w;
      cara[47]=w;
      cara[1]=w+10;
      cara[2]=w+10;
      cara[4]=w+20;
      cara[5]=w+20;
      
      cara[49]=w;
      cara[50]=w;
      cara[94]=w+10;
      cara[95]=w+10;
      cara[92]=w+20;
      cara[91]=w+20;
   emocion(cara,300);
   resetear(cara);
  }
  emocion(cara,300);
 }
}// fin surprise
void happiness(int n, int r)            
{
 int w;
 int j;
     w=20;
 resetear(cara); 
 if(n==1)           //happy 1
 {
 while(r>0,r--)
  {
   cara[55]=w;
   cara[58]=w;
   cara[52]=w;
   cara[43]=w;
   cara[37]=w;
   cara[40]=w;
   cara[49]=w;
   cara[46]=w;
  for(j=0;j<2;j++)
   {
   cara[94]=0;
   cara[1]=0;
   cara[61]=w;
   cara[34]=w;
   emocion(cara,400);
   cara[94]=w;
   cara[1]=w;
   cara[61]=0;
   cara[34]=0;
   emocion(cara,400);
   }
  }
 }
 if(n==2)           //happy 2
 {
 while(r>0,r--)
  {
   cara[55]=w;
   cara[58]=w;
   cara[52]=w;
   cara[43]=w;
   cara[37]=w;
   cara[40]=w;
   cara[49]=w;
   cara[46]=w;
  for(j=0;j<2;j++)
   {
   cara[94]=0;
   cara[1]=0;
   cara[91]=0;
   cara[4]=0;
   cara[61]=w;
   cara[34]=w;
   cara[64]=w;
   cara[31]=w;
   emocion(cara,400);
   cara[94]=w;
   cara[1]=w;
   cara[91]=w;
   cara[4]=w;
   cara[61]=0;
   cara[34]=0;
   cara[64]=0;
   cara[31]=0;
   emocion(cara,400);
   }
  }
 }
 if(n==3)           //happy 3
 {
 while(r>0,r--)
  {
      cara[49]=w;
      cara[52]=w;
      cara[55]=w;
      cara[58]=w;
      cara[37]=w;
      cara[40]=w;
      cara[43]=w;
      cara[46]=w;
  for(j=0;j<4;j++)
   {
   cara[94-3*j]=w;
   cara[61+3*j]=w;
   cara[34-3*j]=w;
   cara[1+3*j]=w;
   emocion(cara,400);
   }
   for(j=0;j<4;j++)
   {
   cara[85+3*j]=0;
   cara[70-3*j]=0;
   cara[25+3*j]=0;
   cara[10-3*j]=0;
   emocion(cara,400);
   }
  }
 }
}//fin happiness

