
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
//#define DEBUG
#include "struct_and_enums.c"
#include "dimming_functions.c"

#include "datapoints.c"
#include "timedevents.c"
#include "debug.c"
#include "eprom.c"
//////////////////////////////////////////
//apagar
volatile unsigned int32 clock;
volatile unsigned int1 secondFlag;
volatile unsigned int1 syncError;
volatile unsigned int1 oscError=0;

//dimmer constants
static int TimeBase=0;
static int32 reg=39;
int16 ledErrorCounter;
//dimmer end

//struct switches msw;
#include "switch_functions.c"
#include "interrupt_functions.c"
#include "hw_setup.c"
//////////////////////////////////////////////////////////////////////////

void main()
{
   mydevices.numberOfInputs=0;
   mydevices.numberOfOutputs=0;
   clock = 0;
   ledErrorCounter=0;   
   hw_setup();
   //TODO: User Code
   //struct switches array[NUMBER_OF_SWITCHES];
  /*
   button_test();
   dimmer_test();
   */
   
   
   ///////////////SALA//////////////////////////////
 /*  eeprom_on_off_init(20,21,6);//2 int Grande a comecar da esquerda
   eeprom_on_off_init(22,23,0);//2 
   //eeprom_dimmer_init(40,41,42,7);//2
   eeprom_on_off_init(41,42,7);//2
   eeprom_on_off_init(26,27,3);//2
   
   
   
   ///Sanca parede grande
   unsigned int on_adr[8]={20,255,255,255,255,255,255,255};
   unsigned int off_adr[8]={21,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,3);//Sanca parede grande
   eeprom_onOff_out_init(on_adr,off_adr,5);//Sanca parede grande
   
   ///Sanca parede rosa
   unsigned int on_adrr[8]={22,255,255,255,255,255,255,255};
   unsigned int off_adrr[8]={23,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adrr,off_adrr,7);//Sanca parede grande
   on_adrr[1]=26;
   off_adrr[1]=27;
   eeprom_onOff_out_init(on_adrr,off_adrr,4);//Sanca parede grande
   
    ///VARANDA
   unsigned int dimmer_dim_adr[8]={40,255,255,255,255,255,255,255}; 
   unsigned int dimmer_on_adr[8]={41,255,255,255,255,255,255,255};
   unsigned int dimmer_off_adr[8]={42,255,255,255,255,255,255,255};
   //eeprom_dimmer_out_init(dimmer_dim_adr,dimmer_on_adr,dimmer_off_adr,6);//varanda
   eeprom_onOff_out_init(dimmer_on_adr,dimmer_off_adr,6);//varanda
   
   eeprom_button_init(28,29,4,true);//estores down
   eeprom_button_init(30,31,5,true);//estores up
   unsigned int up_adr[8]={30,255,255,255,255,255,255,255};
   unsigned int down_adr[8]={28,255,255,255,255,255,255,255};
   unsigned int x_adr[8]={255,255,255,255,255,255,255,255};
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,8,9,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,10,11,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,12,13,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,14,15,0,10);
   */
   /////////////////////////////////////////////////7
   
   ///////////////QUARTO GRANDE//////////////////////////////
   eeprom_on_off_init(1,2,2);//2
   unsigned int on_adr[8]={1,255,255,255,255,255,255,255};
   unsigned int off_adr[8]={2,10,255,255,255,255,255,255};
 
   unsigned int nill_adr[8]={255,255,255,255,255,255,255,255};  
   unsigned int ventax_off_adr[8]={16,255,255,255,255,255,255,255};

   unsigned int x_adr[8]={1,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,4);
   
   eeprom_on_off_init(3,4,4);
   
   off_adr[0]=4;
   on_adr[0]=3;
   eeprom_timer_init(off_adr,on_adr,15,16,time_off,60);
   eeprom_onOff_out_init(on_adr,off_adr,3);
   eeprom_onOff_out_init(on_adr,off_adr,2);
   eeprom_onOff_out_init(on_adr,ventax_off_adr,9);//VENTAX
   
   eeprom_on_off_init(17,18,5);//corredor
   unsigned int corredor_on_adr[8]={17,255,255,255,255,255,255,255};
   unsigned int corredor_off_adr[8]={18,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(corredor_on_adr,corredor_off_adr,1);
   eeprom_button_init(5,6,0,true);//2
   eeprom_button_init(7,8,1,true);//2
   off_adr[0]=255;
   off_adr[1]=255;
   
   on_adr[0]=5;
   x_adr[0]=7;
   eeprom_shutter_out_init(on_adr,x_adr,off_adr,off_adr,11,10,0,10);
   eeprom_on_off_init(9,10,7);
   
   /////////////////////////////////////////////////7
   
   readDevices();
#ifdef DEBUG  
    printf("inputs:%d outputs:%d %d %d\n\r",mydevices.numberOfInputs,mydevices.numberOfOutputs,((struct outputs)mydevices.myoutputs[0]).type,((struct outputs)mydevices.myoutputs[1]).type);
#endif
   dimmer_outputs_init();
   // printf("start %Lu %Lu %Lu\n\r",fpointer(N_LUZES,0),delays1[N_LUZES][0],delays2[N_LUZES][0]); 
   /*((struct light)mydevices.myoutputs[0].device).dim_value.value=50;
   ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
   ((struct light)mydevices.myoutputs[0].device).off.value=1;
   ((struct light)mydevices.myoutputs[0].device).off.needs_update=true;
 */
 
   write_outputs();
 printf("start %Lu\n\r",fpointer(N_LUZES,0)); 
 interrupts_enable();

   while(true){
   restart_wdt();
   if(syncError || oscError)
   {  
      ++ledErrorCounter;
      if(ledErrorCounter>1000)
      {
         output_toggle(LED);
         ledErrorCounter=0;
      }
   }
#ifdef DEBUG
   if(kbhit())
   {
      setup_wdt(WDT_OFF);
      disable_interrupts (GLOBAL) ;
      goDebug();
   }
#endif
   process_outpoints();
   write_outputs();
   if(secondFlag)
   {
      secondFlag=false;
      processTimedEvents();
      if(!syncError && !oscError) output_toggle(LED);
   }
  // print_inputs(false);
  
}
}

