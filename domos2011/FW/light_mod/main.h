#include <18F4685.h>
#device adc=8
#device high_ints=true
#FUSES WDT256                  //Watch Dog Timer uses 1:128 Postscale
#FUSES H4                       //High speed osc with HW enabled 4X PLL
#FUSES NOPROTECT                //Code not protected from reading
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES BORV45                   //Brownout reset at 2.0V
#FUSES PUT                    //No Power Up Timer
#FUSES CPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOPBADEN                   //PORTB pins are configured as analog input channels on RESET
#FUSES BBSIZ4K                  //4K words Boot Block size
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES NOLPT1OSC                  //Timer1 configured for low-power operation
#FUSES NOMCLR                     //Master Clear pin enabled
//#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(clock=40000000,RESTART_WDT)
#use rs232(baud=9600,parity=N,xmit=PIN_B5,rcv=PIN_B4,bits=8,restart_wdt)

