int1 array_contains(struct data_point_in* array,unsigned int value)
{
   int x;
   for (x = 0; x < 8; ++x)
   {
      // printf ("array[ % d] = % u for value % u\n\r", x, ( (struct data_point_in *) array) - > adress[x], value);
      // delay_ms (100) ;
      if ( ( (struct data_point_in *) array)->adress[x] == value) return true;
   }

   return false;
}

void process_inpoints(unsigned int datapoint, unsigned int value)//processa os datapoints de entrada que estao associados a saidas reais
{
   int x;
   for (x = 0; x < mydevices.numberOfOutputs; ++x)
   {
      switch (( (struct outputs) mydevices.myoutputs[x]) .type)
      {
         case dimmer:
         // printf ("process datapoint % d with value % d\n", datapoint, value);
         if (array_contains ( & ( (struct light) mydevices.myoutputs[x].device) .dim_value, datapoint))
         {
          //  printf("DIM");
            ( (struct light) mydevices.myoutputs[x].device) .dim_value.value = value;
            ( (struct light) mydevices.myoutputs[x].device) .dim_value.needs_update = true;
         }

         else if (array_contains ( & ( (struct light) mydevices.myoutputs[x].device) .on, datapoint))
         {
        // printf("DIM ON");
            ( (struct light) mydevices.myoutputs[x].device) .on.value = value;
            ( (struct light) mydevices.myoutputs[x].device) .on.needs_update = true;
         }

         else if (array_contains ( & ( (struct light) mydevices.myoutputs[x].device) .off, datapoint))
         {
            ( (struct light) mydevices.myoutputs[x].device).off.value = value;
            ( (struct light) mydevices.myoutputs[x].device).off.needs_update = true;
         }

         break;
         case _on_off:
         if (array_contains ( & ( (struct oNoFF) mydevices.myoutputs[x].device) .on, datapoint))
         {
           // printf("OF %d\n\r",x);
            ( (struct oNoFF) mydevices.myoutputs[x].device).on.value = value;
            ( (struct oNoFF) mydevices.myoutputs[x].device).on.needs_update = true;
         }

         else if (array_contains ( & ( (struct oNoFF) mydevices.myoutputs[x].device) .off, datapoint))
         {
            ( (struct oNoFF) mydevices.myoutputs[x].device) .off.value = value;
            ( (struct oNoFF) mydevices.myoutputs[x].device) .off.needs_update = true;
         }

         break;
         case shutter:
         if (array_contains ( & ( (struct shutter) mydevices.myoutputs[x].device) .up, datapoint))
         {
            ( (struct shutter) mydevices.myoutputs[x].device) .up.value = value;
            ( (struct shutter) mydevices.myoutputs[x].device) .up.needs_update = true;
         }

         else if (array_contains ( & ( (struct shutter) mydevices.myoutputs[x].device) .down, datapoint))
         {
            ( (struct shutter) mydevices.myoutputs[x].device) .down.value = value;
            ( (struct shutter) mydevices.myoutputs[x].device) .down.needs_update = true;
         }

         else if (array_contains ( & ( (struct shutter) mydevices.myoutputs[x].device) .fullup, datapoint))
         {
            ( (struct shutter) mydevices.myoutputs[x].device) .fullup.value = value;
            ( (struct shutter) mydevices.myoutputs[x].device) .fullup.needs_update = true;
         }

         else if (array_contains ( & ( (struct shutter) mydevices.myoutputs[x].device) .fulldown, datapoint))
         {
            ( (struct shutter) mydevices.myoutputs[x].device) .fulldown.value = value;
            ( (struct shutter) mydevices.myoutputs[x].device) .fulldown.needs_update = true;
         }

         break;
         default:
         break;
      }
   }
    for (x = 0; x < mydevices.numberOfInputs; ++x)//special cases (timer
   {
      switch (( (struct inputs) mydevices.myinputs[x]) .type)
      {
        
         
         case timer:
          if(value==1)
          {
         if (array_contains ( & ( (struct timer) mydevices.myinputs[x].device).start, datapoint))
         {//printf("AKI1");
            ( (struct timer) mydevices.myinputs[x].device).timer=0;
            ( (struct timer) mydevices.myinputs[x].device).active=true;
         }

         else if (array_contains ( & ( (struct timer) mydevices.myinputs[x].device).stop, datapoint))
         {//printf("AKI2");
            ( (struct timer) mydevices.myinputs[x].device).timer=0;
            ( (struct timer) mydevices.myinputs[x].device).active=false;
         }
         }
         break;
      }
   }
         
}

void process_outpoint_answer(unsigned int adress,unsigned int value)
{  
  /* disable_interrupts (GLOBAL) ;
   delay_ms(100);
   enable_interrupts (GLOBAL) ;*/
   process_inpoints (adress, value); //internal processing
   //TODO CAN Sending of values
}

void process_outpoints()
{
   int x;
   for (x = 0; x < mydevices.numberOfInputs; ++x)
   {
      switch (( (struct inputs) mydevices.myinputs[x]) .type)
      {
         case dimmer_switch:
         if (( (struct dimmer_switch) mydevices.myinputs[x].device) .dim_level.needs_update)
         {
            process_outpoint_answer (( (struct dimmer_switch) mydevices.myinputs[x].device) .dim_level.adress, ( (struct dimmer_switch) mydevices.myinputs[x].device) .dim_level.value);
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .dim_level.needs_update = false;
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .dim_level.value = 0;
         }

         if (( (struct dimmer_switch) mydevices.myinputs[x].device) .on.needs_update)
         {
            process_outpoint_answer (( (struct dimmer_switch) mydevices.myinputs[x].device) .on.adress, ( (struct dimmer_switch) mydevices.myinputs[x].device) .on.value);
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .on.needs_update = false;
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .on.value = 0;
         }

         if (( (struct dimmer_switch) mydevices.myinputs[x].device) .off.needs_update)
         {
            process_outpoint_answer (( (struct dimmer_switch) mydevices.myinputs[x].device) .off.adress, ( (struct dimmer_switch) mydevices.myinputs[x].device) .off.value);
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .off.needs_update = false;
            ( (struct dimmer_switch) mydevices.myinputs[x].device) .off.value = 0;
         }

         break;
         case button_switch:
         if (( (struct button_switch) mydevices.myinputs[x].device) .on.needs_update)
         {
            process_outpoint_answer (( (struct button_switch) mydevices.myinputs[x].device) .on.adress, ( (struct button_switch) mydevices.myinputs[x].device) .on.value);
            ( (struct button_switch) mydevices.myinputs[x].device) .on.needs_update = false;
            ( (struct button_switch) mydevices.myinputs[x].device) .on.value = 0;
         }

         if (( (struct button_switch) mydevices.myinputs[x].device) .off.needs_update)
         {
            process_outpoint_answer (( (struct button_switch) mydevices.myinputs[x].device) .off.adress, ( (struct button_switch) mydevices.myinputs[x].device) .off.value);
            ( (struct button_switch) mydevices.myinputs[x].device) .off.needs_update = false;
            ( (struct button_switch) mydevices.myinputs[x].device) .off.value = 0;
         }

         break;
         case on_off_switch:
         if (( (struct on_off_switch) mydevices.myinputs[x].device) .on.needs_update)
         {
           // printf("on needs update");
            process_outpoint_answer (( (struct on_off_switch) mydevices.myinputs[x].device) .on.adress, ( (struct on_off_switch) mydevices.myinputs[x].device) .on.value);
            ( (struct on_off_switch) mydevices.myinputs[x].device) .on.needs_update = false;
            ( (struct on_off_switch) mydevices.myinputs[x].device) .on.value = 0;
         }

         if (( (struct on_off_switch) mydevices.myinputs[x].device) .off.needs_update)
         {
            process_outpoint_answer (( (struct on_off_switch) mydevices.myinputs[x].device) .off.adress, ( (struct on_off_switch) mydevices.myinputs[x].device) .off.value);
            ( (struct on_off_switch) mydevices.myinputs[x].device) .off.needs_update = false;
            ( (struct on_off_switch) mydevices.myinputs[x].device) .off.value = 0;
         }

         break;
         case timer:
          if (( (struct timer) mydevices.myinputs[x].device).on.needs_update)
         {
            process_outpoint_answer (( (struct timer) mydevices.myinputs[x].device).on.adress, ( (struct timer) mydevices.myinputs[x].device) .on.value);
            ( (struct timer) mydevices.myinputs[x].device) .on.needs_update = false;
            ( (struct timer) mydevices.myinputs[x].device) .on.value = 0;
         }
         if (( (struct timer) mydevices.myinputs[x].device) .off.needs_update)
         {
            process_outpoint_answer (( (struct timer) mydevices.myinputs[x].device) .off.adress, ( (struct timer) mydevices.myinputs[x].device) .off.value);
            ( (struct timer) mydevices.myinputs[x].device) .off.needs_update = false;
            ( (struct timer) mydevices.myinputs[x].device) .off.value = 0;
         }
         break;
         default:
         break;
      }
   }
}


