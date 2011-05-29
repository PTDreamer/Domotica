#include <16F876A.h>
#device adc=8
#device *=16
#use delay(clock=20000000)
#fuses HS,NOWDT,nolvp,NOBROWNOUT
#use rs232(baud=baudrate,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
