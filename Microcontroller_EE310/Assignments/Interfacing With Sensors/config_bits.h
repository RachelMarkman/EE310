#ifndef CONFIG_BITS_H
#define CONFIG_BITS_H

#include <xc.h>

#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINTOSC_1MHZ

#pragma config CLKOUTEN = OFF
#pragma config CSWEN = ON
#pragma config FCMEN = ON

#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = OFF
#pragma config IVT1WAY = OFF
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS

#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = OFF
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF

#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF

#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = LFINTOSC

#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF

#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config LVP = OFF

#pragma config CP = OFF

#define _XTAL_FREQ 1000000UL

#endif
