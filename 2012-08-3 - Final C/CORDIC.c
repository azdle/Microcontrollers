#include <p18f4550.h>
#include <math.h>

// If the data is signed, we probably ought to make cbuff a signed char.
// we'll fix this in mavg, though.
#pragma config WDT = OFF, PBADEN = OFF

// downloaded (and modified by Kia) from 
// www.execpc.com/~geezer/embed/cordic.c

#define 	AG_CONST    0.6072529350
#define	FXD(X)          ((long int)((X) * 65536.0))

typedef long int fixed; /* 16.16 fixed-point */

static const fixed Alpha[]={ FXD(45.0),
  FXD(26.565), FXD(14.0362), FXD(7.12502),
  FXD(3.57633), FXD(1.78991), FXD(0.895174),
  FXD(0.447614),FXD(0.223811), FXD(0.111906),
  FXD(0.055953),FXD(0.027977) };

void main(void){
  fixed X, Y, CurrAngle;
  unsigned i;
  float sin_c, cos_c, sin_err, cos_err;

  sin_c = sin(28.027*3.14159/180);		// 3775 cycles
  cos_c = cos(28.027*3.14159/180);		// 5157 cycles

  X=FXD(AG_CONST);  /* AG_CONST * cos(0) */
  Y=0;		    /* AG_CONST * sin(0) */
  CurrAngle=FXD(28.027);
  for(i=0; i < 12; i++){
     fixed NewX;

     if(CurrAngle > 0) {
        NewX=X - (Y >> i);
        Y+=(X >> i);
        X=NewX;
        CurrAngle -= Alpha[i]; }
     else {
        NewX=X + (Y >> i);
        Y-=(X >> i);
        X=NewX;
        CurrAngle += Alpha[i]; 
     } // if-else
  } // for (i=…
  
	// CORDIC 12 passes: 3206 cycles
	// CORDIC 23 passes: 8154 cycles


  sin_err = sin_c - Y/65536.0;
  cos_err = cos_c - X/65536.0;
} // main
