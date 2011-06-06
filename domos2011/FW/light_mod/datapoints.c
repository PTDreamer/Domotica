int1 array_contains(struct data_point_in* array,int & value)
{
   int x;
   for(x=0;x<8;++x)
   {
      if(((struct data_point_in*) array)->adress[x]==value) return true;
    //  printf("array[%u]=%u\n\r",x,&(array));
      ++array;
   }
   return false;
}

void process_inpoints(unsigned int &datapoint, unsigned int &value)//processa os datapoints de entrada que estao associados a saidas reais
{
   int x;
   for(x=0;x<mydevices.numberOfOutputs;++x)
   {
      switch (((struct outputs)mydevices.myoutputs[x]).type) {

    case dimmer:
           if (array_contains(((struct light)mydevices.myoutputs[x].device).dim_value,datapoint))
           {
               ((struct light)mydevices.myoutputs[x].device).dim_value.value=value;
               ((struct light)mydevices.myoutputs[x].device).dim_value.needs_update=true;
           }
           else if (array_contains(((struct light)mydevices.myoutputs[x].device).on,datapoint))
           {
               ((struct light)mydevices.myoutputs[x].device).on.value=value;
               ((struct light)mydevices.myoutputs[x].device).on.needs_update=true;
           }
           else if (array_contains(((struct light)mydevices.myoutputs[x].device).off,datapoint))
           {
               ((struct light)mydevices.myoutputs[x].device).off.value=value;
               ((struct light)mydevices.myoutputs[x].device).off.needs_update=true;
           }
           break;

    case _on_off:

           break;
    case shutter:

           break;
    default:

            break; }

   }
}
void process_outpoint_answer(unsigned int adress,unsigned int value)
{
   //   printf("update answer adress=%u value=%u\n\r",adress,value);
      process_inpoints(adress,value); //internal processing
      //TODO CAN Sending of values
}

void process_outpoints()
{
    int x;
   for(x=0;x<mydevices.numberOfInputs;++x)
   { // printf("type=%u\n\r",((struct inputs)mydevices.myinputs[x]).type);
      switch (((struct inputs)mydevices.myinputs[x]).type) {
    case dimmer_switch:
             //  printf("dimmer_switch");
               if( ((struct dimmer_switch)mydevices.myinputs[x].device).dim_level.needs_update)
               {
                   process_outpoint_answer( ((struct dimmer_switch)mydevices.myinputs[x].device).dim_level.adress, ((struct dimmer_switch)mydevices.myinputs[x].device).dim_level.value);
                   ((struct dimmer_switch)mydevices.myinputs[x].device).dim_level.needs_update=false;
                   ((struct dimmer_switch)mydevices.myinputs[x].device).dim_level.value=0;
               }
           if( ((struct dimmer_switch)mydevices.myinputs[x].device).on.needs_update)
               {  
                  process_outpoint_answer( ((struct dimmer_switch)mydevices.myinputs[x].device).on.adress, ((struct dimmer_switch)mydevices.myinputs[x].device).on.value);
                   ((struct dimmer_switch)mydevices.myinputs[x].device).on.needs_update=false;
                   ((struct dimmer_switch)mydevices.myinputs[x].device).on.value=0;
               }
               if( ((struct dimmer_switch)mydevices.myinputs[x].device).off.needs_update)
               {
                  process_outpoint_answer( ((struct dimmer_switch)mydevices.myinputs[x].device).off.adress, ((struct dimmer_switch)mydevices.myinputs[x].device).off.value);
                   ((struct dimmer_switch)mydevices.myinputs[x].device).off.needs_update=false;
                   ((struct dimmer_switch)mydevices.myinputs[x].device).off.value=0;
               }
               break;
    case timer:

           break;
    default:

            break; }

   }
}


