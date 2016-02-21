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

void testLeds();


void main()
{
setConnectionSocket(); // avisa si conectó o no

   string emotion;
   
   cin << "Emotion is:";
   cin >> emotion;

if(emotion=="test")
   testLeds();

}

void testLeds()
{
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
   for(int i=0;i<96;i++)
      buf[i]=0;
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
