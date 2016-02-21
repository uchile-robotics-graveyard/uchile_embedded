/*
Este programa modifica el ángulo de 6 servos los cuales están representados en
el arreglo llamado ang[k] donde k toma valores entre 0 y 5 y ang[k] toma valores
entre 0 y 180, representando cada valor un angulo del servo (preferiblemente simpre tener
valores entre 2 y 178 ya que los extremos, por aproximaciones se escapan de los valores
permitidos por el servo)
La operación que realiza con el ángulo es ang[k]*111/180+ 37 ; operación que se obtuvo
de generar una recta entre los puntos 37 y 148 que representan el Duty cycle de un pulso
de aprox 16 [ms](especificaciones del servo) donde 37 representa ángulo 0 y 148 ángulo
180.
En la placa construida para controlar los servos se tienen las siguientes relaciones
ang[0]= CCP9 ; representado por el servo 1 de la placa
ang[1]=CCP8 ; representado por el servo 2 de la placa
ang[2]=CCP7 ; representado por el servo 3 de la placa
ang[3]=CCP6 ; representado por el servo 4 de la placa
ang[4]=CCP5 ; representado por el servo 8 de la placa
ang[5]=CCP10 ; representado por el servo 9 de la placa
LOS OTROS SERVOS DE LA PLACA NO ESTAN IMPLEMENTADOS, NO INTENTAR USAR.
*/



#include <stdio.h>
#include <stdlib.h>
#include <p18f66k90.h>
#include <math.h>
#include <i2c.h>
#include "config.h"

int divide(int dividend, int divisor);
void SetDC(int dc);
int SetDRef(int angle); //it recieves from 0° to 180°

int angulo, denom,z,y,current,answer,num, porcien=0,pordiez=0,poruno=0;
int i = 0,b,aux,j,c, centena, decena, unidad, numero, resto, dutyref[6];
int dc[6],q,comp,bn[8],angbin[15],angulo, ang[6], k; 
int dref, histeresis_step =3, dc_error=0; 

unsigned char sync_mode=0, slew=0, add1,status,temp,w,length=0;



void main(void) {


    //---init oscillator----
    // OSCCON: osc. control (type, power modes, freq, status)

    OSCCONbits.IRCF = 0b011;    // 0b(111|110|101|100|011):(16|8|4|2|1)[MHz]
    OSCCONbits.SCS = 0b10;      // use internal oscillator

    //OSCTUNE: int. osc. tuning + PLL
    //OSCTUNEbits.PLLEN = 1;    //PLL enable (requires IRCF 4-16 MHz)
    OSCTUNEbits.TUN = 0;    //Run fast RC oscillator at the calibrated frecuency

    T2CONbits.TMR2ON = 1;   //Timer2 is on


    TRISC = 0b11111001;		//Set outputs
    TRISE = 0b00000011;
    TRISG = 0b00000000;

    PR2 = 255; //In wich value of T2 it will reset


    T2CONbits.T2CKPS = 0b11; // Timer2 prescaler, (00|01|1x):(1|4|16)
    
    //PWM configurations
    CCP4CONbits.CCP4M = 0b1111;
    CCP5CONbits.CCP5M = 0b1111;
    CCP6CONbits.CCP6M = 0b1111;
    CCP7CONbits.CCP7M = 0b1111;
    CCP8CONbits.CCP8M = 0b1111;
    CCP9CONbits.CCP9M = 0b1111;
    CCP10CONbits.CCP10M = 0b1111;
    
	//PWM timer dependency (it depends on TMR 2)
    CCPTMRS1bits.C4TSEL0 = 0b00;
    CCPTMRS1bits.C5TSEL0 = 0b0;
    CCPTMRS1bits.C6TSEL0 = 0b0;
    CCPTMRS1bits.C7TSEL0 = 0b00;
    CCPTMRS2bits.C8TSEL0 = 0b00;
    CCPTMRS2bits.C9TSEL0 = 0b0;
    CCPTMRS2bits.C10TSEL0 = 0b0;

    /*for (i=0; i<6; i++){
    	dc[i]=92;
    	ang[i]=90;
    }
    */
    
    
    //---INITIALISE THE I2C MODULE FOR SLAVE MODE WITH 100KHz ---
	sync_mode = SLAVE_7;
	slew = SLEW_OFF;
	OpenI2C(sync_mode,slew);
	SSPADD = 0xA2; //initialze slave address



    while (1){
	    
	    //**** Data reception from master by slave ***
	    aux=0;
	    for (k=0; k<5; k++){
		    i=dc[k];
		    j=dutyref[k];
	    	if (i==j){
	    		aux++;
	    	}
	    }
	    if (aux==6){
	    	do
			{
				while(DataRdyI2C()==0); //WAIT UNTILL THE DATA IS TRANSMITTED FROM master
				ang[length]=getcI2C(); // save byte received
				length++;
			}
			while(length!=5);
	    }
	    
	    for (k=0 ; k<6; k++){
		    dutyref[k]=SetDRef(ang[k]);
		   } 
	    /*
	    for (k=0 ; k<6; k++){
		    i=dc[k];
		    j=dutyref[k];
		    if (j==i){
			    if (ang[k]==135){
			    	ang[k]=45;
			    	dutyref[k]=SetDRef(ang[k]);
			    	}
			    else if (ang[k]==90){
			    	ang[k]=135;
			    	dutyref[k]=SetDRef(ang[k]);
			    	}
			    else{
			    	ang[k]=90;
			    	dutyref[k]=SetDRef(ang[k]);
			    	}
		    	}
		    	
		    }
		    */
	    //dutyref[k] = SetDRef(ang[k]) does dutyref[k] = ang[k]*111/180 + 37
		
		//Soft movement of the servos	
		for (k=0;k<5;k++){
			//if(dc[k]==dutyref[k]){
			//}
			//else{
			i=dc[k];
			j=dutyref[k];
				if(i>j){
					dc_error=i-j;
					if(dc_error<histeresis_step){
						dc[k]=dutyref[k];
				
					}
					else{
						dc[k]-=histeresis_step; 
					}
				}
				if(i<j){
					dc_error=j-i;
					if(dc_error<histeresis_step){
						dc[k]=dutyref[k]; 
					}
					else { 
						dc[k]+=histeresis_step;
				
					}	
				}
				SetDC(dc[k]);	//Set the corresponding values for Duty cycle
				//}
			
		}
		
    }

}

int SetDRef(int angle)
{
	angulo=angle;
	poruno=divide(angulo,180);
	i=divide(resto,18);
	pordiez=divide(angulo,18);
	j=divide(resto,2);
	porcien=divide(angulo,18);
	porcien*=10;
	j*=11;
	aux=i+j;
	i=divide(aux,10);
	dref=poruno+pordiez;
	dref+=porcien;
	dref+=i;
	dref+=37;
	return dref;	
	
}

void SetDC(int dc)
{		//--Transform dc into a binary number and put it in an array (bn[i])
		//each value of the array is 1 bit bn[0]=LSB bn[7]=MSB
		q=dc;
		comp=dc;
		for( i=0;i<8;i++){
			bn[i]=0;
		}
		//		
		for(i=0;i<8;i++){
			q>>=1;
			q<<=1;
			if((comp-q)==1){ 
				bn[i]=1;
			}
			else{
				bn[i]=0;
			}
			q>>=1;
			comp=q;
		}
		
		//make the aux variable take the value of bn[2]*2^0+bn[3]*2^1+...+bn[7]*2^5
		aux=bn[2];
		c=0;
		for(i=3;i<8;i++){
			j=i-2;
			b = 1<<j;
			c=bn[i];	
			j=c*b;
			aux+=j; 
		}
		
		//CCPxCONbits.DCxB = 2 LSB of the Duty cycle
		//CCPRxL = 8 MSB of the duty cycle
		//k is the index of the duty cycle array wich respresents the servo that i'm controlling
		switch(k){
			case 0:
				CCP9CONbits.DC9B = bn[1]*2;
				CCP9CONbits.DC9B+=bn[0];
				CCPR9L=aux;
				break;
			case 1:
				CCP8CONbits.DC8B = bn[1]*2;
				CCP8CONbits.DC8B+=bn[0];
				CCPR8L=aux;
				break;
			case 2:
				CCP7CONbits.DC7B = bn[1]*2;
				CCP7CONbits.DC7B+=bn[0];
				CCPR7L=aux;
				break;
			case 3:
				CCP6CONbits.DC6B = bn[1]*2;
				CCP6CONbits.DC6B+=bn[0];
				CCPR6L=aux;
				break;
			case 4:
				CCP5CONbits.DC5B = bn[1]*2;
				CCP5CONbits.DC5B+=bn[0];
				CCPR5L=aux;
				break;
			case 5:
				CCP10CONbits.DC10B = bn[1]*2;
				CCP10CONbits.DC10B+=bn[0];
				CCPR10L=aux;
				break;
			default:
				break;
		}
}

int divide(int dividend, int divisor) { 

	//divides dividend/divisor, returns the answer and saves the rest of the division in the variable called "resto"

    num=dividend;
    denom=divisor;
    current = 1;
    answer=0;
     
    if(denom>num){
	    resto=num;
        return 0;
	}

    if ( denom == num){
	    resto=0; 
        return 1;
	}
    while (denom <= num) {
        denom<<= 1;
        current<<= 1; 
    }
	
    denom>>= 1;
    current>>= 1;
	

    while (current!=0) {
        if ( num >= denom) {
            num -= denom;
            answer+=current;
        }
        current>>= 1;
        denom>>= 1;
		
		
    }
    num=dividend;
    denom=divisor;
    current=answer*denom;
    resto=num-current;
		    
    return answer;
}