void onOffHelper(struct switches *sw)
{
   if(sw->dim_level.value!=0) 
   {
       sw->on.value=1;
       sw->off.value=0;
   }
   else
   {
      sw->on.value=0;
      sw->off.value=1;
   }
}
void process_onOff(struct switches *sw)
{
   int last_dim=sw->dim_level.value;
   int last_on=sw->on.value;
   int last_off=sw->off.value;
   if(sw->current_state==button_pressed && sw-> previous_state == button_depressed)
   {   
      if(sw->dim_level.value==0)
      {  
         sw->dim_level.value=127;
      }
      else 
      {
         sw->dim_level.value=0;
      }
      sw->previous_state=button_pressed;
   }
   else if(sw->current_state==button_depressed)
      sw-> previous_state = button_depressed;
   onOffHelper(sw);
   if (last_dim!=sw->dim_level.value) sw->dim_level.needs_update=true;
   if (last_on!=sw->on.value) sw->on.needs_update=true;
   if (last_off!=sw->off.value) sw->off.needs_update=true;
}
void process_buttons(struct switches *sw)
{
   int last_dim=sw->dim_level.value;
   int last_on=sw->on.value;
   int last_off=sw->off.value;  
   if(sw->current_state==button_pressed) 
   {
      sw->dim_level.value=127;
   }
   else  
   {
      sw->dim_level.value=0;
   }
   onOffHelper(sw);
   if (last_dim!=sw->dim_level.value) sw->dim_level.needs_update=true;
   if (last_on!=sw->on.value) sw->on.needs_update=true;
   if (last_off!=sw->off.value) sw->off.needs_update=true;
}
void process_dimmers(struct switches *sw)
{
   int last_level=sw->current_level;
   int last_dim=sw->dim_level.value;
   if(sw->current_state==button_pressed)//se o botao esta premido
   {
     // printf("DIF %LU",clock - sw->timer);
      sw-> previous_state = button_pressed;
      if ((clock - sw->timer > reg || sw->outstate == dimming))
      { 
         if(sw->outstate==off)
         {
            sw->on.value=1;
            sw->on.needs_update=1;
         }
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
            sw-> outstate = dimmed;
            if (sw->direction == up) sw->direction = down; //inverte direccao
            else sw->direction = up;
         }
         else//se nao esteve a regular
         {
            if (sw->outstate == off) //se estava desligada
            {
              // printf("on");
               if(sw->current_level > 0) //liga
                  sw->outstate=dimmed;
               else
                  sw->outstate=on;
               sw->on.value=1;
               sw->direction = down;
               sw->on.needs_update=true;
            }
            else
            {
              // printf("off");
               //sw->current_level = 0; //senao desliga
               sw->outstate=off;
               sw->off.value=1;
               sw->direction = up;
               sw->off.needs_update=true;
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

void read_inputs()
{
  int x;
   for(x=0;x<mydevices.numberOfInputs;++x)
   {
      switch (mydevices.myinputs[x].type) {
    case switch_:
           // printf("aki");
            if (input (mydevices.myinputs[x].device.realbutton)) mydevices.myinputs[x].device.current_state = button_depressed;
            else mydevices.myinputs[x].device.current_state = button_pressed;
            process_dimmers(&mydevices.myinputs[x].device);
            break;
    case timer:

           break;
    default:

            break; 
            }

   }
}
