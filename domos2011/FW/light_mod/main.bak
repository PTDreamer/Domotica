
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
#include "dimming_functions.c"
#include "struct_and_enums.c"
#include "datapoints.c"
//////////////////////////////////////////
//apagar
int32 clock;

//dimmer constants
static int TimeBase=0;
static int32 reg=39;
//dimmer end

struct switches msw;
#include "switch_functions.c"

//////////////////////////////////////////////////////////////////////////

int1 flag=0;//mains not detected flag
#int_TIMER0
void  TIMER0_isr(void) 
{
   ++clock;
   if (bit_test (clock, 1)) read_inputs();
}

#int_TIMER1
void  TIMER1_isr(void) 
{
   flag = 1;
}

#int_EXT
void  EXT_isr(void) 
{

  // set_timer1 (0) ;
         portc=0xFF;
         portd=0xFF;
         if (organizado) 
         {
            organizado=0;
            if (actmat) 
            {
               actmat=0;
               pointer=delays1;
            }
            else
            {
               actmat=1;
               pointer=delays2;
            }
         }
         vez=0;
         CCP_1=matrizluz[fpointer(0,0)];
         mnumluzes=fpointer(N_LUZES,0);
         set_timer1(0);
}

#int_RDA
void  RDA_isr(void) 
{
}

#int_TBE
void  TBE_isr(void) 
{
}

#int_BUSCOL
void  BUSCOL_isr(void) 
{
}

#int_OSCF
void  OSCF_isr(void) 
{
}

#int_canrx0
void canrx0_int ( ) 
{
   canReceive ();
   // TODO: add CAN recieve code here
}

#int_canrx1
void canrx1_int ( ) 
{
   canReceive ();
   // TODO: add CAN recieve code here
}

#int_cantx0
void cantx0_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_cantx1
void cantx1_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_cantx2
void cantx2_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_canirx
void canirx_int ( ) 
{
   // TODO: add CAN IRX handling code here
}

#int_canerr
void canerr_int ( ) 
{
   // TODO: add CAN error handling code here
}

#int_CCP1
void CCP1_isr(void) 
{
   if(mnumluzes!=0)
   {
   output_low (LED) ;
      int16 auxccp=fpointer(vez,1);
      portc=(portc & MAKE8(auxccp,1));
      portd=(portd & MAKE8(auxccp,0));
      ++vez;
      --mnumluzes;
      CCP_1=matrizluz[fpointer(vez,0)];
   }
}
void main()
{
   clock = 0;
   can_init () ;
   can_set_mode (CAN_OP_CONFIG) ;
   
   BRGCON1.brp = 4;
   BRGCON1.sjw = 0;
   BRGCON2.prseg = 2;
   BRGCON2.seg1ph = 5;
   BRGCON2.sam = FALSE;
   BRGCON2.seg2phts = FALSE;
   BRGCON3.seg2ph = 5;
   BRGCON3.wakfil = TRUE;
   can_set_mode (CAN_OP_NORMAL) ;
   
   enable_interrupts (int_canrx0);
   enable_interrupts (int_canrx1);
   enable_interrupts (int_cantx0);
   enable_interrupts (int_cantx1);
   enable_interrupts (int_cantx2);
   enable_interrupts (int_canirx);
   enable_interrupts (int_canerr);
   setup_adc_ports (NO_ANALOGS | VSS_VDD) ;
   setup_adc (ADC_CLOCK_DIV_2 | ADC_TAD_MUL_0) ;
   setup_psp (PSP_DISABLED) ;
   setup_spi (SPI_SS_DISABLED) ;
   //setup_wdt (WDT_ON) ;
   setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_4); //div16 1.6us thick 104ms overflow
   
   //setup_timer_1 (T1_INTERNAL | T1_DIV_BY_2); //div2 0.2us thick overflow 13.1ms
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_COMPARE_INT);
   enable_interrupts(INT_CCP1);
   ext_int_edge(l_to_h);
   
   setup_timer_2 (T2_DISABLED, 0, 1) ;
   setup_timer_3 (T3_DISABLED | T3_DIV_BY_1) ;
   
  // setup_ccp1 (CCP_OFF) ;
   setup_comparator (NC_NC_NC_NC) ;
   setup_vref (FALSE) ;
   
   enable_interrupts (int_TIMER0) ;
   enable_interrupts (int_EXT) ;
   //enable_interrupts (int_TBE) ;
   //enable_interrupts (int_BUSCOL) ;
   //enable_interrupts (int_CANIRX) ;
   //enable_interrupts (int_CANERR) ;
   //enable_interrupts (int_CANTX2) ;
   //enable_interrupts (int_CANTX1) ;
   //enable_interrupts (int_CANTX0) ;
   //enable_interrupts (int_CANRX1) ;
   //enable_interrupts (int_CANRX0) ;
   //enable_interrupts (int_OSCF) ;
  
   //TODO: User Code
   struct switches array[NUMBER_OF_SWITCHES];
   msw.previous_state=button_depressed;
   msw.outstate=off;

   trisc=0x00; //tudo saidas
   trisd=0x00; //tudo saidas
   actmat=0;
   pointer=delays1;
   org();
   pointer=delays2;
   actmat=1;
   org();
   
   test();
   
   enable_interrupts (GLOBAL) ;
   while(true)
   {
   delay_ms (200) ;
   if(mydevices.myinputs[0].device.dim_level.needs_update)
   {
      printf("dim_level %d\n\r",mydevices.myinputs[0].device.dim_level.value);
      mydevices.myinputs[0].device.dim_level.needs_update=false;
   }
   if(mydevices.myinputs[0].device.on.needs_update)
   {
      printf("on %d\n\r",mydevices.myinputs[0].device.on.value);
      mydevices.myinputs[0].device.on.needs_update=false;
   }
   if(mydevices.myinputs[0].device.off.needs_update)
   {
      printf("off %d\n\r",mydevices.myinputs[0].device.off.value);
      mydevices.myinputs[0].device.off.needs_update=false;
   }
}
   while(organizado==0){}
   for(temp=0;temp<17;++temp)
   {
      printf("%lu <-> %lu\n\r",fpointer(temp,0),fpointer(temp,1));
   }
    int16 auxccp=fpointer(0,1);
      printf("%lu <-> %u <-> %u\n\r",auxccp,MAKE8(auxccp,1),MAKE8(auxccp,0));
   for (;; )
   {   
      
      output_high (LED) ;
       delay_ms (1000) ;
      
     // output_low (LED) ;
      
      
      delay_ms (200) ;
      //   printf("%LX\n\r",temp);
     // printf (" %d %lX\n\r", msw.dim_level.value,porta) ;
   }
}


