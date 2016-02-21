#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "EmotionBender.h"
#define MAXHOSTNAME 256
using namespace std;

//Auxiliary functions
void frame(char * array, int delay)
{
	//for(int i=96;i<MSGSIZE;i++)
	//array[i]=128;
	sendBuf(array,MSGSIZE);
	sleep(delay);
	//cout << "frame sent" << endl;
}
void resetArray(char * array)
{
	for(int j=0;j<MSGSIZE;j++)
	{
		array[j]=0;       //leds
		if(j > 95)
			array[j]=128; //motors
	}
}
void testLeds1()
{
	char face[MSGSIZE];
	resetArray(face);
	int j=0;
	for(int i=4; i>0;i--)
	{
		j=++j%3;
		for(int i=0;i<96;i++)
		{
			if(i%3==j)
				face[i]=255;
			else
				face[i]=0;
		}
		frame(face,3);
		resetArray(face);
	}
}
void testLeds2()
{
	char face[MSGSIZE];
	resetArray(face);

	for(int j=0;j<96;j++)
	{
		face[j]=255;
		frame(face,3);
		resetArray(face);
	}
}
void testMotors()
{
	char face[MSGSIZE];
	resetArray(face);

	for(int i=96;i<MSGSIZE;i++)
	{
		face[i]=255;
		frame(face,2);
	}
}

// Emotions
void anger(int n, int r)      
{
	char face[MSGSIZE];
	resetArray(face);
	int w=20; //intensidad del led

	if(n==0)
	{
		face[50]=w;
		face[53]=w;
		face[56]=w;
		face[59]=w;
		face[62]=w;
		face[65]=w;

		face[32]=w;
		face[35]=w;
		face[38]=w;
		face[41]=w;
		face[44]=w;
		face[47]=w;

		//motors
		face[95+9]=192;   //motor 9
		frame(face,1);
	}

	if(n==1)               //anger 1
	{
		for(;r>0;r--)
		{
			for(int j=0;j<10;j++)      //frame 1
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
				frame(face,0);
			}
			for(int j=0;j<10;j++)      //frame 2
			{
				w=w-10;
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
				frame(face,0);
			}
			resetArray(face);
		}
	} //fin if(n==1)
	if(n==2)               //anger 2
	{
		for(;r>0;r--)
		{
			for(int j=0;j<2;j++)      //frame 1
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
				frame(face,1);
			}
			for(int j=0;j<2;j++)      //frame 2
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
				frame(face,1);
			}
			resetArray(face);
		}
	} //fin if(n==2)
	if(n==3)               //anger 3
	{
		for(;r>0;r--)
		{
			for(int j=0;j<7;j++)      //frame 1
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
				frame(face,1);
			}
			for(int j=0;j<7;j++)      //frame 2
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
				frame(face,1);
			}
			resetArray(face);
			for(int j=0;j<7;j++)      //frame 3
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
				frame(face,1);
			}
			for(int j=0;j<7;j++)      //frame 4
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
				frame(face,1);
			}
			resetArray(face);
		}
	}  //fin if(n==3)
} //fin void anger
void sadness(int n, int r)
{
	char face[MSGSIZE];
	resetArray(face);
	int w=20;

	if(n==0)
	{
		face[6]=w;
		face[9]=w;
		face[24]=w;
		face[27]=w;
		face[30]=w;
		face[33]=w;


		face[87]=w;
		face[84]=w;
		face[60]=w;
		face[63]=w;
		face[66]=w;
		face[69]=w;

		//motors
		face[95+9]=64;   //motor 9
		frame(face,1);

	}

	if(n==1)     //sadness 1
	{
		for(;r>0;r--)
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
			frame(face,0);
			face[6]=w+5;
			face[9]=w-5;
			face[87]=w+5;
			face[84]=w-5;

			face[24]=w+5;
			face[27]=w-5;
			face[66]=w+5;
			face[69]=w-5;
			frame(face,0);
			face[6]=w+5;
			face[9]=w-5;
			face[87]=w+5;
			face[84]=w-5;

			face[24]=w+5;
			face[27]=w-5;
			face[66]=w+5;
			face[69]=w-5;
			frame(face,0);
			face[6]=w+5;
			face[9]=w-5;
			face[87]=w+5;
			face[84]=w-5;

			face[24]=w+5;
			face[27]=w-5;
			face[66]=w+5;
			face[69]=w-5;
			frame(face,0);
			w=30;
			face[6]=w-5;
			face[9]=w+5;
			face[87]=w-5;
			face[84]=w+5;

			face[24]=w-5;
			face[27]=w+5;
			face[66]=w-5;
			face[69]=w+5;
			frame(face,0);
			face[6]=w-5;
			face[9]=w+5;
			face[87]=w-5;
			face[84]=w+5;

			face[24]=w-5;
			face[27]=w+5;
			face[66]=w-5;
			face[69]=w+5;
			frame(face,0);
			face[6]=w-5;
			face[9]=w+5;
			face[87]=w-5;
			face[84]=w+5;

			face[24]=w-5;
			face[27]=w+5;
			face[66]=w-5;
			face[69]=w+5;
			frame(face,0);
			face[6]=w-5;
			face[9]=w+5;
			face[87]=w-5;
			face[84]=w+5;

			face[24]=w-5;
			face[27]=w+5;
			face[66]=w-5;
			face[69]=w+5;
			frame(face,0);
			// delay_ms(5000);
		}
		resetArray(face);
		frame(face,1);
	} //fin if(n==1)
	if(n==2)     //sadness 2
	{
		for(;r>0;r--)
		{
			face[0]=w;
			face[93]=w;
			face[54]=w;
			face[51]=w;
			face[48]=w;
			face[39]=w;
			face[42]=w;
			face[45]=w;
			frame(face,0);
			face[72]=1;
			face[66]=w;
			frame(face,0);
			//delay_ms(d+100);
			face[66]=1;
			face[69]=w;
			frame(face,0);
			//delay_ms(d+100);
			face[69]=1;
			face[78]=w;
			frame(face,0);
			//delay_ms(d);
			face[78]=1;
			face[75]=w;
			frame(face,0);
			//delay_ms(d);
			face[75]=1;
			face[72]=w;
			frame(face,0);
			//delay_ms(d);
		}
		resetArray(face);
		frame(face,0);
	} //fin if(n==2)
	if(n==3)     //sadness 3
	{
		for(;r>0;r--)
		{
			face[54]=w;
			face[51]=w;
			face[48]=w;
			face[39]=w;
			face[42]=w;
			face[45]=w;
			frame(face,1);
			face[21]=0;
			face[6]=w;
			face[27]=w;
			face[72]=0;
			face[66]=w;
			face[87]=w;
			frame(face,1);
			//delay_ms(d+100);
			face[27]=0;
			face[24]=w;
			face[6]=0;
			face[9]=w;
			face[66]=0;
			face[69]=w;
			face[87]=0;
			face[84]=w;
			frame(face,1);
			//delay_ms(d+100);
			face[24]=0;
			face[9]=0;
			face[15]=w;
			face[69]=0;
			face[84]=0;
			face[78]=w;
			frame(face,1);
			//delay_ms(d);
			face[15]=0;
			face[18]=w;
			face[78]=0;
			face[75]=w;
			frame(face,1);
			//delay_ms(d);
			face[18]=0;
			face[21]=w;
			face[75]=0;
			face[72]=w;
			frame(face,1);
			//delay_ms(d);
		}
		resetArray(face);
		frame(face,1);
	}
} //fin sadness
void shamed(int n, int r)
{
	char face[MSGSIZE];
	resetArray(face);
	int w=20;

	if(n==0)
	{
		face[0]=w;
		face[3]=w;
		face[30]=w;
		face[33]=w;

		face[90]=w;
		face[93]=w;
		face[60]=w;
		face[63]=w;

		face[81]=w+100;
		face[83]=w+100;
		face[12]=w+100;
		face[14]=w+100;

		//motors
		face[95+9]=64;   //motor 9
		frame(face,1);
	}

	if(n==1)           //shamed 1
	{
		for(;r>0;r--)
		{
			for(int j=0;j<15;j++)
			{
				w=w+10;
				face[81]=w;
				face[83]=w;
				face[12]=w;
				face[14]=w;
				frame(face,1);
			}
			for(int j=0;j<15;j++)
			{
				w=w-10;
				face[81]=w;
				face[83]=w;
				face[12]=w;
				face[14]=w;
				frame(face,1);
			}
		}
		resetArray(face);
		frame(face,1);
	}
}// fin shamed
void standby(int n, int r)
{
	char face[MSGSIZE];
	resetArray(face);
	int w=10;

	if(n==0)
	{
		//red
		face[95]=w;
		face[92]=w;
		face[89]=w;
		face[68]=w;
		face[65]=w;
		face[62]=w;

		face[2]=w;
		face[5]=w;
		face[8]=w;
		face[29]=w;
		face[32]=w;
		face[35]=w;

		//green
		w=20;
		face[94]=w;
		face[91]=w;
		face[88]=w;
		face[67]=w;
		face[64]=w;
		face[61]=w;

		face[1]=w;
		face[4]=w;
		face[7]=w;
		face[28]=w;
		face[31]=w;
		face[34]=w;
		frame(face,1);

	}

	if(n==1)           //standby 1
	{
		for(;r>0;r--)
		{
			w=10;
			for(int j=0;j<5;j++)
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
				frame(face,1);
			}
			for(int j=0;j<3;j++)
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
			for(int j=0;j<5;j++)
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
			for(int j=0;j<3;j++)
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
	char face[MSGSIZE];
	resetArray(face);
	int w=10;
	if(n==1)           //sorprise 1
	{
		for(;r>0;r--)
		{
			for(int j=0;j<2;j++)
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
				frame(face,0);
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
			frame(face,0);
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
			frame(face,0);
			resetArray(face);

			for(int j=8;j<14;j++)
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
				frame(face,0);
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
			frame(face,0);
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
			frame(face,0);
			resetArray(face);
		}
		frame(face,1);
	}
}// fin surprise
void happiness(int n, int r)            
{
	char face[MSGSIZE];
	resetArray(face);
	int w=20;

	if(n==0)
	{
		for(int j=49; j<62; j=j+3)
			face[j]=w;
		face[94]=w;

		for(int j=34; j<47; j=j+3)
			face[j]=w;
		face[1]=w;

		frame(face,1);
	}

	if(n==1)           //happy 1
	{
		for(;r>0;r--)
		{
			face[55]=w;
			face[58]=w;
			face[52]=w;
			face[43]=w;
			face[37]=w;
			face[40]=w;
			face[49]=w;
			face[46]=w;
			for(int j=0;j<2;j++)
			{
				face[94]=0;
				face[1]=0;
				face[61]=w;
				face[34]=w;
				frame(face,2);
				face[94]=w;
				face[1]=w;
				face[61]=0;
				face[34]=0;
				frame(face,2);
			}
		}
	}
	if(n==2)           //happy 2
	{
		for(;r>0;r--)
		{
			face[55]=w;
			face[58]=w;
			face[52]=w;
			face[43]=w;
			face[37]=w;
			face[40]=w;
			face[49]=w;
			face[46]=w;
			for(int j=0;j<2;j++)
			{
				face[94]=0;
				face[1]=0;
				face[91]=0;
				face[4]=0;
				face[61]=w;
				face[34]=w;
				face[64]=w;
				face[31]=w;
				frame(face,2);
				face[94]=w;
				face[1]=w;
				face[91]=w;
				face[4]=w;
				face[61]=0;
				face[34]=0;
				face[64]=0;
				face[31]=0;
				frame(face,2);
			}
		}
	}
	if(n==3)           //happy 3
	{
		for(;r>0;r--)
		{
			face[49]=w;
			face[52]=w;
			face[55]=w;
			face[58]=w;
			face[37]=w;
			face[40]=w;
			face[43]=w;
			face[46]=w;
			for(int j=0;j<4;j++)
			{
				face[94-3*j]=w;
				face[61+3*j]=w;
				face[34-3*j]=w;
				face[1+3*j]=w;
				frame(face,2);
			}
			for(int j=0;j<4;j++)
			{
				face[85+3*j]=0;
				face[70-3*j]=0;
				face[25+3*j]=0;
				face[10-3*j]=0;
				frame(face,2);
			}
		}
	}
}//fin happiness

void eyebrow()
{
	char face[MSGSIZE];
	resetArray(face);

	for(int j=5;j>0;j--)
	{
		face[95+9] = 128;
		frame(face,2);
		face[95+9] = 0;
		frame(face,2);
	}


	frame(face,2);
}
