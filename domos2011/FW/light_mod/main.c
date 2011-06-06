
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
#include "struct_and_enums.c"
#include "dimming_functions.c"

#include "datapoints.c"
//////////////////////////////////////////
//apagar
volatile unsigned int32 clock;

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
   ((struct light)mydevices.myoutputs[0].device).dim_value.value=50;
   ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
   ((struct light)mydevices.myoutputs[0].device).on.value=1;
   ((struct light)mydevices.myoutputs[0].device).on.needs_update=true;
 
   write_outputs();
   interrupts_enable();
   while(true){
   process_outpoints();
   write_outputs();
  // print_inputs(false);
   };
    

   while(organizado==0){}
   for(temp=0;temp<17;++temp)
   {
      printf("%lu <-> %lu\n\r",fpointer(temp,0),fpointer(temp,1));
   }
    int16 auxccp=fpointer(0,1);
      printf("%lu <-> %u <-> %u\n\r",auxccp,MAKE8(auxccp,1),MAKE8(auxccp,0));

}


