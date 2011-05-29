int1 array_contains(int * array,int & value)
{
   int x;
   for(x=0;x<8;++x)
   {
      if(&(array)==value) return true;
      ++array;
   }
   return false;
}

void process_outpoint_answer(int adress,int value)
{
   
}

void process_inpoints(int &datapoint, int &value)
{
   int x;
   for(x=0;x<mydevices.numberOfOutputs;++x)
   {
      switch (mydevices.myoutputs[x].type) {

    case _dimmer:
           if (array_contains(mydevices.myoutputs[x].device.dim_value.adress,datapoint))
           {
               mydevices.myoutputs[x].device.dim_value.value=value;
               mydevices.myoutputs[x].device.dim_value.needs_update=true;
           }
           else if (array_contains(mydevices.myoutputs[x].device.on.adress,datapoint))
           {
               mydevices.myoutputs[x].device.on.value=value;
               mydevices.myoutputs[x].device.on.needs_update=true;
           }
           else if (array_contains(mydevices.myoutputs[x].device.off.adress,datapoint))
           {
               mydevices.myoutputs[x].device.off.value=value;
               mydevices.myoutputs[x].device.off.needs_update=true;
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

void process_outpoints()
{
    int x;
   for(x=0;x<mydevices.numberOfInputs;++x)
   {
      switch (mydevices.myinputs[x].type) {
    case switch_:
               if(mydevices.myinputs[x].device.dim_level.needs_update)
               {
                  process_outpoint_answer(mydevices.myinputs[x].device.dim_level.adress,mydevices.myinputs[x].device.dim_level.value);
                  mydevices.myinputs[x].device.dim_level.needs_update=false;
                  mydevices.myinputs[x].device.dim_level.value=0;
               }
           if(mydevices.myinputs[x].device.on.needs_update)
               {
                  process_outpoint_answer(mydevices.myinputs[x].device.on.adress,mydevices.myinputs[x].device.on.value);
                  mydevices.myinputs[x].device.on.needs_update=false;
                  mydevices.myinputs[x].device.on.value=0;
               }
               if(mydevices.myinputs[x].device.off.needs_update)
               {
                  process_outpoint_answer(mydevices.myinputs[x].device.off.adress,mydevices.myinputs[x].device.off.value);
                  mydevices.myinputs[x].device.off.needs_update=false;
                  mydevices.myinputs[x].device.off.value=0;
               }
               break;
    case timer:

           break;
    default:

            break; }

   }
}
