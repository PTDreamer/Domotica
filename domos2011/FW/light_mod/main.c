
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
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
   
   
   ///////////////QUARTO GRANDE//////////////////////////////
   eeprom_on_off_init(1,2,2);//2
   unsigned int off_adr[8]={2,255,255,255,255,255,255,255};
   unsigned int on_adr[8]={1,255,255,255,255,255,255,255};
   unsigned int x_adr[8]={1,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,4);
   eeprom_on_off_init(3,4,4);
   off_adr[0]=4;
   on_adr[0]=3;
   eeprom_onOff_out_init(on_adr,off_adr,3);
   eeprom_button_init(5,6,0,true);//2
   eeprom_button_init(7,8,1,true);//2
   off_adr[0]=255;
   on_adr[0]=5;
   x_adr[0]=7;
   eeprom_shutter_out_init(on_adr,x_adr,off_adr,off_adr,11,10,1,10);
   /////////////////////////////////////////////////7
   
   
   readDevices();
   printf("inputs:%d outputs:%d %d %d\n\r",mydevices.numberOfInputs,mydevices.numberOfOutputs,((struct outputs)mydevices.myoutputs[0]).type,((struct outputs)mydevices.myoutputs[1]).type);
   
   dimmer_outputs_init();
   /*((struct light)mydevices.myoutputs[0].device).dim_value.value=50;
   ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
   ((struct light)mydevices.myoutputs[0].device).off.value=1;
   ((struct light)mydevices.myoutputs[0].device).off.needs_update=true;
 */
 
   write_outputs();
 
 interrupts_enable();
 printf("start\n\r");
   while(true){
   restart_wdt();
   if(syncError)
   {  
      ++ledErrorCounter;
      if(ledErrorCounter>1000)
      {
         output_toggle(LED);
         ledErrorCounter=0;
      }
   }
   if(kbhit())
   {
      setup_wdt(WDT_OFF);
      disable_interrupts (GLOBAL) ;
      goDebug();
   }
   
   process_outpoints();
   write_outputs();
   if(secondFlag)
   {
      secondFlag=false;
      processTimedEvents();
      if(!syncError) output_toggle(LED);
   }
  // print_inputs(false);
  
}
}

