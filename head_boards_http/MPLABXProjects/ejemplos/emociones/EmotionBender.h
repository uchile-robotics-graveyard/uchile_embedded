#ifndef EMOTION_BENDER_H_
#define EMOTION_BENDER_H_

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "socketConnection.h"
#define MAXHOSTNAME 256
#define MSGSIZE 106
using namespace std;

//functions
void frame(char * array, int delay); //(arreglo de 96, partiendo desde 0)
void resetArray(char * array);
void testLeds1();
void testLeds2();
void testMotors();

//Emotions //n=nivel; r=repetición 
void sadness(int n, int r);
void anger(int n, int r);
void happiness(int n, int r);
void shamed(int n, int r);
void surprise(int n, int r);
void standby(int n, int r);
void eyebrow();


#endif
