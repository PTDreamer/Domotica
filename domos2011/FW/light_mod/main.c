
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
#include "struct_and_enums.c"
#include "dimming_functions.c"

#include "datapoints.c"
#include "timedevents.c"
//////////////////////////////////////////
//apagar
volatile unsigned int32 clock;
volatile unsigned int1 secondFlag;
volatile unsigned int1 syncError;
//dimmer constants
static int TimeBase=0;
static int32 reg=39;
//dimmer end

//struct switches msw;
#include "switch_functions.c"
#include "interrupt_functions.c"
#include "hw_setup.c"
//////////////////////////////////////////////////////////////////////////

void main()
{
   clock = 0;
      
   hw_setup();
   //TODO: User Code
   //struct switches array[NUMBER_OF_SWITCHES];
  
   button_test();
   dimmer_test();
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
   if(syncError)
      output_toggle(LED);
   process_outpoints();
   write_outputs();
   if(secondFlag)
   {
      secondFlag=false;
      processTimedEvents();
      output_toggle(LED);
   }
  // print_inputs(false);
  
}
}

