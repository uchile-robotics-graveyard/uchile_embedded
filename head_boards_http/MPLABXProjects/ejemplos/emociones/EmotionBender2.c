#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define MAXHOSTNAME 256
using namespace std;

//funciones
void frame(char array[], int delay); //(arreglo de 96, partiendo desde 0)
void resetArray(char array[]);
void testLeds();
//Emociones
void sadness(int n, int r); //n=nivel; r=repetición 
void anger(int n, int r);
void happiness(int n, int r);
void shamed(int n, int r);
void surprise(int n, int r);
void standby(int n, int r);

//declaracion de variables
int socketHandle;
char face[96];

//funciones auxiliares
void frame(char array[], int delay) // envia el frame actual y resetea el arreglo a cero
{
  send(socketHandle, array, strlen(array), 0);  //envia el arreglo a los leds
  sleep(1);
}
void resetArray(char array[]) //nueva cara: reset
{
  for(int j=0;j<96;j++) //resetea el arreglo para iniciar la emocion
  array[j]=0;
}

void testLeds()
{
  int rc = 0;  // Actual number of bytes read by function read()
  char buf[512];
  int j=0;
  while(1)
  {
   j=++j%3;
   
      //strcpy(buf,"Message to send");
   for(int i=0;i<96;i++)
   {
      if(i%3==j)
            buf[i]=255;
      else
         buf[i]=1;
   }
   buf[96]=0;
   send(socketHandle, buf, strlen(buf), 0);
   cout << "Receive answer" << endl;
   resetArray(buf);
   /*rc=0
   while(rc<96)
   {
         rc += recv(socketHandle, buf+rc, 512, 0);
   }*/
   rc=0;
   while(!rc)
      rc=recv(socketHandle, buf+rc, 512, 0);
      buf[rc]= (char) NULL;        // Null terminate string

      cout << "Number of bytes read: " << rc << endl;
      cout << "Received: " << buf << endl;
    for(int i=0;i<strlen(buf);i++)
      cout << (unsigned int)(unsigned char)buf[i] << ", ";
   cout << endl;
   //sleep(1);
   //return 0;
  }
}



// emociones
void anger(int n, int r)      
{
 int w=15;
 int j;
 resetArray(face); 
 if(n==1)               //anger 1
 {
  while(r>0,r--)
  {
   for(j=0;j<6;j++)      //frame 1
   {
   w=w+5;
      face[2]=w;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[95]=w;
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;
   frame(face,100);
   }  
   for(j=0;j<6;j++)      //frame 2
   {
   w=w-5;
      face[2]=w;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[95]=w;
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;
   frame(face,100);
   }
   resetArray(face); 
  }
 } //fin if(n==1)
 if(n==2)               //anger 2
 {
  while(r>0,r--)
  {
   for(j=0;j<2;j++)      //frame 1
   {
   w=w+10;
      face[2]=w;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[95]=w;
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;
   frame(face,100);
   }
   for(j=0;j<2;j++)      //frame 2
   {
   w=w-10;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;     //cambio de led
      face[38]=w;
      face[59]=w;
      face[95]=0;
      face[2]=0;
   frame(face,100);
   }
   resetArray(face);
  }
 } //fin if(n==2)
 if(n==3)               //anger 3
 {
  while(r>0,r--)
  {
   for(j=0;j<7;j++)      //frame 1
   {
   w=w+5;
      face[2]=w;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[95]=w;
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;
   frame(face,100);
   }
   for(j=0;j<7;j++)      //frame 2
   {
   w=w-5;
      face[2]=w;
      face[50]=w;
      face[53]=w;
      face[56]=w;
      face[41]=w;
      face[44]=w;
      face[47]=w;
      face[95]=w;   
      face[26]=w;
      face[29]=w;
      face[68]=w;
      face[71]=w;
   frame(face,100);
   }
   resetArray(face);
   for(j=0;j<7;j++)      //frame 3
   {
   w=w+5; 
      face[5]=w;
      face[8]=w;
      face[11]=w;
      face[86]=w;
      face[89]=w;
      face[92]=w;
      face[59]=w;
      face[62]=w;
      face[65]=w;
      face[32]=w;
      face[35]=w;
      face[38]=w;
   frame(face,100);
   }
   for(j=0;j<7;j++)      //frame 4
   {
   w=w-5;
      face[5]=w;
      face[8]=w;
      face[11]=w;
      face[86]=w;
      face[89]=w;
      face[92]=w;
      face[59]=w;
      face[62]=w;
      face[65]=w;
      face[32]=w;
      face[35]=w;
      face[38]=w;
   frame(face,100);
   }
   resetArray(face);
  }
 }  //fin if(n==3)   
} //fin void anger
void sadness(int n, int r)
 {
 int w;
 int d;
     w=50;
 resetArray(face); 
 if(n==1)     //sadness 1
 {
  while(r>0,r--)
  {
   w=30;
   face[6]=w+5;
   face[9]=w-5;
   face[87]=w+5;
   face[84]=w-5;
   
   face[24]=w+5;
   face[27]=w-5;
   face[66]=w+5;
   face[69]=w-5;
   frame(face,300);
   face[6]=w+5;
   face[9]=w-5;
   face[87]=w+5;
   face[84]=w-5;
   
   face[24]=w+5;
   face[27]=w-5;
   face[66]=w+5;
   face[69]=w-5;
   frame(face,300);
   face[6]=w+5;
   face[9]=w-5;
   face[87]=w+5;
   face[84]=w-5;
   
   face[24]=w+5;
   face[27]=w-5;
   face[66]=w+5;
   face[69]=w-5;
   frame(face,300);
    face[6]=w+5;
   face[9]=w-5;
   face[87]=w+5;
   face[84]=w-5;
   
   face[24]=w+5;
   face[27]=w-5;
   face[66]=w+5;
   face[69]=w-5;
   frame(face,300);
   w=30;
    face[6]=w-5;
   face[9]=w+5;
   face[87]=w-5;
   face[84]=w+5;
   
   face[24]=w-5;
   face[27]=w+5;
   face[66]=w-5;
   face[69]=w+5;
   frame(face,300);
     face[6]=w-5;
   face[9]=w+5;
   face[87]=w-5;
   face[84]=w+5;
   
   face[24]=w-5;
   face[27]=w+5;
   face[66]=w-5;
   face[69]=w+5;
   frame(face,300);
   face[6]=w-5;
   face[9]=w+5;
   face[87]=w-5;
   face[84]=w+5;
   
   face[24]=w-5;
   face[27]=w+5;
   face[66]=w-5;
   face[69]=w+5;
   frame(face,300);
   face[6]=w-5;
   face[9]=w+5;
   face[87]=w-5;
   face[84]=w+5;
   
   face[24]=w-5;
   face[27]=w+5;
   face[66]=w-5;
   face[69]=w+5;
   frame(face,300);
  // delay_ms(5000);
   }
   resetArray(face);
   frame(face,500);
 } //fin if(n==1)
 if(n==2)     //sadness 2 
 {
  while(r>0,r--)
  {
  d=350;
      face[0]=w;
      face[93]=w;
      face[54]=w;
      face[51]=w;
      face[48]=w;
      face[39]=w;
      face[42]=w;
      face[45]=w;
  frame(face,500); 
      face[72]=0;
      face[66]=w;
  frame(face,500);
  //delay_ms(d+100);
     face[66]=0;
     face[69]=w;
  frame(face,500);
  //delay_ms(d+100);
     face[69]=0;
     face[78]=w;
  frame(face,500);
  //delay_ms(d);
     face[78]=0;
     face[75]=w;
  frame(face,500);
  //delay_ms(d);
     face[75]=0;
     face[72]=w;
  frame(face,500); 
  //delay_ms(d);
  }
  resetArray(face);
  frame(face,1000);
 } //fin if(n==2)
 if(n==3)     //sadness 3 
 {
  while(r>0,r--)
  {
   int d; 
       d=200;
     face[54]=w;
     face[51]=w;
     face[48]=w;
     face[39]=w;
     face[42]=w;
     face[45]=w;
   frame(face,500);      
     face[21]=0;
     face[6]=w;
     face[27]=w;      
     face[72]=0;
     face[66]=w;
     face[87]=w;
   frame(face,500);
   //delay_ms(d+100);
     face[27]=0;
     face[24]=w;
     face[6]=0;
     face[9]=w;
     face[66]=0;
     face[69]=w;
     face[87]=0;
     face[84]=w;
   frame(face,500);
   //delay_ms(d+100);
     face[24]=0;
     face[9]=0;
     face[15]=w;
     face[69]=0;
     face[84]=0;
     face[78]=w;
   frame(face,300);
   //delay_ms(d);
     face[15]=0;
     face[18]=w;
     face[78]=0;
     face[75]=w;
   frame(face,300);
   //delay_ms(d);
     face[18]=0;
     face[21]=w;
     face[75]=0;
     face[72]=w;
   frame(face,300); 
   //delay_ms(d);
  }
  resetArray(face);
  frame(face,1000);
 }
} //fin sadness
void shamed(int n, int r)
{
 int w;
 int j;
     w=20;
 resetArray(face); 
 if(n==1)           //shamed 1
 {   
  while(r>0,r--)
  {
   for(j=0;j<15;j++)
   {
   w=w+10;
      face[81]=w;
      face[83]=w;
      face[12]=w;
      face[14]=w;
   frame(face,500);
   }
   for(j=0;j<15;j++)
   {
   w=w-10;
      face[81]=w;
      face[83]=w;
      face[12]=w;
      face[14]=w;
   frame(face,500);
   }
  }
  resetArray(face);
  frame(face,500);
 }
}// fin shamed
void standby(int n, int r)
{
 int w;
 int j;
 resetArray(face); 
 if(n==1)           //standby 1
 {
 while(r>0,r--)
  {
   w=10;
   for(j=0;j<5;j++)
   {
   w=w+5;
      face[91]=w;
      face[85]=w;
      face[88]=w;
      face[67]=w;
      face[70]=w;
      face[7]=w;
      face[10]=w;
      face[25]=w;
      face[28]=w;
   frame(face,300);
   }
   for(j=0;j<3;j++)
   {
   w=30;
      face[59]=w;
      face[58]=w/2;
      face[56]=w;
      face[55]=w/2;
      face[53]=w;
      face[52]=w/2;
      face[50]=w;
      face[49]=w/2;
      
      face[38]=w;
      face[37]=w/2;
      face[41]=w;
      face[40]=w/2;
      face[44]=w;
      face[43]=w/2;
      face[47]=w;
      face[46]=w/2;
   frame(face,700);
   //delay_ms(500);
   w=0;
      face[59]=w;
      face[58]=w/2;
      face[56]=w;
      face[55]=w/2;
      face[53]=w;
      face[52]=w/2;
      face[50]=w;
      face[49]=w/2;
      
      face[38]=w;
      face[37]=w/2;
      face[41]=w;
      face[40]=w/2;
      face[44]=w;
      face[43]=w/2;
      face[47]=w;
      face[46]=w/2;
   frame(face,700);
   //delay_ms(500);
   }
   w=40;
   for(j=0;j<5;j++)
   {
   w=w-5;
      face[91]=w;
      face[85]=w;
      face[88]=w;
      face[67]=w;
      face[70]=w;
      face[7]=w;
      face[10]=w;
      face[25]=w;
      face[28]=w;;
   frame(face,300);
   }
   for(j=0;j<3;j++)
   {
   w=30;
      face[59]=w;
      face[58]=w/2;
      face[56]=w;
      face[55]=w/2;
      face[53]=w;
      face[52]=w/2;
      face[50]=w;
      face[49]=w/2;
      
      face[38]=w;
      face[37]=w/2;
      face[41]=w;
      face[40]=w/2;
      face[44]=w;
      face[43]=w/2;
      face[47]=w;
      face[46]=w/2;
   frame(face,700);
   //delay_ms(500);
   w=0;
      face[59]=w;
      face[58]=w/2;
      face[56]=w;
      face[55]=w/2;
      face[53]=w;
      face[52]=w/2;
      face[50]=w;
      face[49]=w/2;
      
      face[38]=w;
      face[37]=w/2;
      face[41]=w;
      face[40]=w/2;
      face[44]=w;
      face[43]=w/2;
      face[47]=w;
      face[46]=w/2;
   frame(face,700);
   //delay_ms(500);
   }
  }
  resetArray(face);
  frame(face,500);
 }
}// fin standby
void surprise(int n, int r)
{
 int w;
 int j;
     w=10;
 resetArray(face); 
 if(n==1)           //sorprise 1
 {
 while(r>0,r--)
  {
  for(j=0;j<2;j++)
   {
      face[3*j+1]=w;
      face[3*j+2]=w;
      face[3*j+4]=w+10;
      face[3*j+5]=w+10;
      face[3*j+7]=w+20;
      face[3*j+8]=w+20;
      
      face[94-3*j]=w;
      face[95-3*j]=w;
      face[91-3*j]=w+10;
      face[92-3*j]=w+10;
      face[88-3*j]=w+20;
      face[89-3*j]=w+20;
   frame(face,300);
   resetArray(face);
   }
      face[7]=w;
      face[8]=w;
      face[10]=w+10;
      face[11]=w+10;
      face[25]=w+20;
      face[26]=w+20;
      
      face[88]=w;
      face[89]=w;
      face[85]=w+10;
      face[86]=w+10;
      face[70]=w+20;
      face[71]=w+20;
   frame(face,300);
   resetArray(face);
      face[10]=w;
      face[11]=w;
      face[25]=w+10;
      face[26]=w+10;
      face[28]=w+20;
      face[29]=w+20;
      
      face[85]=w;
      face[86]=w;
      face[70]=w+10;
      face[71]=w+10;
      face[67]=w+20;
      face[68]=w+20;
   frame(face,300);
   resetArray(face);
   for(j=8;j<14;j++)
   {
      face[3*j+1]=w;
      face[3*j+2]=w;
      face[3*j+4]=w+10;
      face[3*j+5]=w+10;
      face[3*j+7]=w+20;
      face[3*j+8]=w+20;
      
      face[94-3*j]=w;
      face[95-3*j]=w;
      face[91-3*j]=w+10;
      face[92-3*j]=w+10;
      face[88-3*j]=w+20;
      face[89-3*j]=w+20;
   frame(face,300);
   resetArray(face);
   }
      face[43]=w;
      face[44]=w;
      face[46]=w+10;
      face[47]=w+10;
      face[1]=w+20;
      face[2]=w+20;
      
      face[52]=w;
      face[53]=w;
      face[49]=w+10;
      face[50]=w+10;
      face[94]=w+20;
      face[95]=w+20;
   frame(face,300);
   resetArray(face);
      face[46]=w;
      face[47]=w;
      face[1]=w+10;
      face[2]=w+10;
      face[4]=w+20;
      face[5]=w+20;
      
      face[49]=w;
      face[50]=w;
      face[94]=w+10;
      face[95]=w+10;
      face[92]=w+20;
      face[91]=w+20;
   frame(face,300);
   resetArray(face);
  }
  frame(face,300);
 }
}// fin surprise
void happiness(int n, int r)            
{
 int w;
 int j;
     w=20;
 resetArray(face); 
 if(n==1)           //happy 1
 {
 while(r>0,r--)
  {
   face[55]=w;
   face[58]=w;
   face[52]=w;
   face[43]=w;
   face[37]=w;
   face[40]=w;
   face[49]=w;
   face[46]=w;
  for(j=0;j<2;j++)
   {
   face[94]=0;
   face[1]=0;
   face[61]=w;
   face[34]=w;
   frame(face,400);
   face[94]=w;
   face[1]=w;
   face[61]=0;
   face[34]=0;
   frame(face,400);
   }
  }
 }
 if(n==2)           //happy 2
 {
 while(r>0,r--)
  {
   face[55]=w;
   face[58]=w;
   face[52]=w;
   face[43]=w;
   face[37]=w;
   face[40]=w;
   face[49]=w;
   face[46]=w;
  for(j=0;j<2;j++)
   {
   face[94]=0;
   face[1]=0;
   face[91]=0;
   face[4]=0;
   face[61]=w;
   face[34]=w;
   face[64]=w;
   face[31]=w;
   frame(face,400);
   face[94]=w;
   face[1]=w;
   face[91]=w;
   face[4]=w;
   face[61]=0;
   face[34]=0;
   face[64]=0;
   face[31]=0;
   frame(face,400);
   }
  }
 }
 if(n==3)           //happy 3
 {
 while(r>0,r--)
  {
      face[49]=w;
      face[52]=w;
      face[55]=w;
      face[58]=w;
      face[37]=w;
      face[40]=w;
      face[43]=w;
      face[46]=w;
  for(j=0;j<4;j++)
   {
   face[94-3*j]=w;
   face[61+3*j]=w;
   face[34-3*j]=w;
   face[1+3*j]=w;
   frame(face,400);
   }
   for(j=0;j<4;j++)
   {
   face[85+3*j]=0;
   face[70-3*j]=0;
   face[25+3*j]=0;
   face[10-3*j]=0;
   frame(face,400);
   }
  }
 }
}//fin happiness

