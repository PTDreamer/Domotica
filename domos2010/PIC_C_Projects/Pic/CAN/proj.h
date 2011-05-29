#include <16F876a.h>
#device *=16
#device adc=8
#use delay(clock=20000000)
#fuses HS,noWDT,nolvp,NOBROWNOUT
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

