void process_onOff(struct on_off_switch *sw)
{
   if((sw->current_state==button_pressed) && (sw-> previous_state == button_depressed))
   {   
      sw-> previous_state = button_pressed;
      if(sw->outstate==off)
      {
         sw->outstate=on;
         sw->on.value=1;
         sw->on.needs_update=true;
         sw->off.value=0;
         sw->off.needs_update=true;
      }
      else if(sw->outstate==on)
      {
         sw->outstate=off;
         sw->off.value=1;
         sw->off.needs_update=true;
         sw->on.value=0;
         sw->on.needs_update=true;
      }
}
   else if(sw->current_state==button_depressed)
      sw-> previous_state = button_depressed;
}
void process_buttons(struct button_switch *sw)
{
   if(sw->current_state==button_pressed && sw->outstate==off) 
   {
         sw->outstate=on;
         sw->on.value=1;
         sw->on.needs_update=true;
         sw->off.value=0;
         sw->off.needs_update=true;
   }
   else if(sw->current_state==button_depressed && sw->outstate==on)  
   {
         sw->outstate=off;
         sw->off.value=1;
         sw->off.needs_update=true;
         sw->on.value=0;
         sw->on.needs_update=true;
   }
}
void process_dimmers(struct dimmer_switch *sw)
{
   int last_level=sw->current_level;
   if(sw->current_state==button_pressed)//se o botao esta premido
   {
    //  printf("STATE %u",sw->current_state);
     // printf("DIF %LU",clock - sw->timer);
      sw-> previous_state = button_pressed;
      if ((clock - sw->timer > reg || sw->outstate == dimming) && sw->outstate!=maxed)
      { 
         if(sw->outstate==off)
         {
            sw->outstate=maxed;
            sw->on.value=1;
            sw->current_level = 127;
            sw->on.needs_update=1;
         }
        else
        {
         sw->outstate = dimming;
         if (clock - sw->timer > TimeBase) //regula
         {
            if (sw->direction == up) //para cima
            {
               ++sw->current_level;
               if (sw->current_level == 127) sw->direction = down;
            }
            else//para baixo
            {
               --sw->current_level;
               if (sw->current_level == 0) 
               {  
                  sw->direction = up;
                  sw->off.value=1;
               }
            }
         }
         sw->timer = clock;
      }
      }
      else
      {
         //sw - > timer = clock;
      }
   }
   else//se o botao nao esta premido
   {
      if (sw->previous_state == button_pressed) //se o botao acabou de ser deslargado
      {
         sw->previous_state = button_depressed;
         if (sw->outstate == dimming) //se esteve a regular
         {
            sw-> outstate = on;
            if (sw->direction == up) sw->direction = down; //inverte direccao
            else sw->direction = up;
         }
         else//se nao esteve a regular
         {
            if (sw->outstate == off) //se estava desligada
            {
               sw->outstate=on;
               sw->on.value=1;
               sw->direction = down;
               sw->on.needs_update=true;
            }
            else if (sw->outstate == on)
            {
               sw->outstate=off;
               sw->off.value=1;
               sw->direction = up;
               sw->off.needs_update=true;
            }
            else if(sw->outstate == maxed)
            {
               sw->outstate=on;
               sw->direction = down;
            }
         }
      }
      sw->timer = clock;
   }
 //  onOffHelper(sw);
   if (last_level!=sw->current_level) 
   {
      sw->dim_level.value=sw->current_level;
      sw->dim_level.needs_update=true;
   }
}

void read_inputs()//called periodicaly to pass current phisical input (or timer values) value to structures
{
  int x;
   for(x=0;x<mydevices.numberOfInputs;++x)
   {
      switch (((struct inputs)mydevices.myinputs[x]).type) {
    case dimmer_switch:
            if (input (((struct dimmer_switch)mydevices.myinputs[x].device).realbutton)) 
            {
               ((struct dimmer_switch)mydevices.myinputs[x].device).current_state = button_depressed;
            }
            else 
            {
               ((struct dimmer_switch)mydevices.myinputs[x].device).current_state = button_pressed;
            }
            process_dimmers(&mydevices.myinputs[x].device);
            break;
    case timer:

           break;
    case on_off_switch:
            if (input (((struct on_off_switch)mydevices.myinputs[x].device).realbutton)) 
            {
               ((struct on_off_switch)mydevices.myinputs[x].device).current_state = button_depressed;
            }
            else 
            {
               ((struct on_off_switch)mydevices.myinputs[x].device).current_state = button_pressed;
            }
            process_onOff(&mydevices.myinputs[x].device);
    break;
    case button_switch:
            if (input (((struct button_switch)mydevices.myinputs[x].device).realbutton)) 
            {
               ((struct button_switch)mydevices.myinputs[x].device).current_state = button_depressed;
            }
            else 
            {
               ((struct button_switch)mydevices.myinputs[x].device).current_state = button_pressed;
            }
            process_buttons(&mydevices.myinputs[x].device);
    break;
    default:

            break; 
            }

   }
}
void print_inputs(int1 disable_updates)
{
   int x;
   
   {
      delay_ms (200) ;
      for(x=0;x<mydevices.numberOfInputs;++x)
      {
         switch (((struct inputs)mydevices.myinputs[x]).type) {
            case dimmer_switch:
               if(((struct dimmer_switch)mydevices.myinputs[0].device).dim_level.needs_update)
               {
                  printf("dim_level %u\n\r"((struct dimmer_switch)mydevices.myinputs[0].device).dim_level.value);
                  if(disable_updates)((struct dimmer_switch)mydevices.myinputs[0].device).dim_level.needs_update=false;
               }
               if(((struct dimmer_switch)mydevices.myinputs[0].device).on.needs_update)
               {
                  printf("on %u\n\r",((struct dimmer_switch)mydevices.myinputs[0].device).on.value);
                  if(disable_updates)((struct dimmer_switch)mydevices.myinputs[0].device).on.needs_update=false;
               }           
               if(((struct dimmer_switch)mydevices.myinputs[0].device).off.needs_update)
               {
                  printf("off %u\n\r",((struct dimmer_switch)mydevices.myinputs[0].device).off.value);
                  if(disable_updates)((struct dimmer_switch)mydevices.myinputs[0].device).off.needs_update=false;
               }
               break;
            case on_off_switch:
               if(((struct on_off_switch)mydevices.myinputs[0].device).on.needs_update)
               {
                  printf("on %u\n\r",((struct on_off_switch)mydevices.myinputs[0].device).on.value);
                  if(disable_updates)((struct on_off_switch)mydevices.myinputs[0].device).on.needs_update=false;
               }           
               if(((struct on_off_switch)mydevices.myinputs[0].device).off.needs_update)
               {
                  printf("off %u\n\r",((struct on_off_switch)mydevices.myinputs[0].device).off.value);
                  if(disable_updates)((struct on_off_switch)mydevices.myinputs[0].device).off.needs_update=false;
               }
            break;
            case button_switch:
               if(((struct button_switch)mydevices.myinputs[0].device).on.needs_update)
               {
                  printf("on %u\n\r",((struct button_switch)mydevices.myinputs[0].device).on.value);
                  if(disable_updates)((struct button_switch)mydevices.myinputs[0].device).on.needs_update=false;
               }           
               if(((struct button_switch)mydevices.myinputs[0].device).off.needs_update)
               {
                  printf("off %u\n\r",((struct button_switch)mydevices.myinputs[0].device).off.value);
                  if(disable_updates)((struct button_switch)mydevices.myinputs[0].device).off.needs_update=false;
               }
            break;
         }
      }
   }
}
