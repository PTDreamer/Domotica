
enum switch_outstate {on, off, dimming, dimmed, maxed};
enum switch_direction {up, down};
enum switch_state {button_pressed, button_depressed};
enum output_type {_on_off, dimmer,shutter};
enum output_state {_on, _off, _dimmed};
enum timer_type {time_on, time_off, periodic_on};
enum input_type {dimmer_switch,on_off_switch,button_switch,timer,none};

void dimmer_init(unsigned int dim_adr,unsigned int on_adr,unsigned int off_adr,struct inputs* input);
typedef struct data_point_out
{
   unsigned int adress;
   unsigned int value;
   int1 needs_update;
};
typedef struct data_point_in
{
   unsigned int adress[8];
   unsigned int value;
   int1 needs_update;
};
///////////////////////INPUTS//////////////////////////////////////
typedef struct dimmer_switch
{
 //  switch_type type;
   struct data_point_out dim_level;
   struct data_point_out on;
   struct data_point_out off;
   unsigned int current_level;
   int32  timer;
   switch_state current_state;
   switch_state previous_state;
   switch_direction direction;
   switch_outstate outstate;
   unsigned int realbutton;
};
typedef struct on_off_switch
{
 //  switch_type type;
   struct data_point_out on;
   struct data_point_out off;
    switch_state current_state;
   switch_state previous_state;
   int32  timer;
   switch_outstate outstate;
   unsigned int realbutton;
};

typedef struct button_switch
{
 //  switch_type type;
   struct data_point_out on;
   struct data_point_out off;
   switch_state current_state;
   switch_state previous_state;
   int32  timer;
   switch_outstate outstate;
   unsigned int realbutton;
};

typedef struct timer
{
   timer_type type;
   int32 timeToAct;
   struct data_point_out on;
   struct data_point_out off;
   int32  timer;
};
union input
{
   struct on_off_switch;
   struct dimmer_switch;
   struct timer;
   struct button_switch;
};

typedef struct inputs
{
   enum input_type type;
   union input device;
};
///////////////////////OUTPUTS//////////////////////////////////////
struct light
{
   struct data_point_in on;
   struct data_point_in off;
   struct data_point_in dim_value;
   enum output_state out_state;
   unsigned int output_pin;
   int internal_order;
   int1  needs_update;
};

union output
{
   struct light;
};

typedef struct outputs
{
   enum output_type type;
   union output device;
};

typedef struct devices
{
      unsigned int numberOfInputs;
      unsigned int numberOfOutputs;
      struct inputs myinputs[10];
      struct outputs myoutputs[10];
}devicestype;

devicestype mydevices;

void copy_array(unsigned int *origin,struct data_point_in* destination , unsigned int n_elements)
{
   int x;
   for (x=0;x<n_elements;++x)
   {
      ((struct data_point_in*)destination)->adress[x]=*origin;
      printf("%u:%u<----->%u\n\r",x,((struct data_point_in*)destination)->adress[x],*origin);
      ++origin;
   }
}


///////INPUTS INITIALIZATION//////////////////////////////////////////////////
void dimmer_init(unsigned int dim_adr,unsigned int on_adr,unsigned int off_adr,struct inputs* input,unsigned int real_button)
{
      input->type=dimmer_switch;
      ((struct dimmer_switch)input->device).dim_level.adress=dim_adr;
      ((struct dimmer_switch)input->device).on.adress=on_adr;
      ((struct dimmer_switch)input->device).off.adress=off_adr;
      ((struct dimmer_switch)input->device).previous_state=button_depressed;
      ((struct dimmer_switch)input->device).outstate=off;
      ((struct dimmer_switch)input->device).current_level=0;
      ((struct dimmer_switch)input->device).direction=up;
      ((struct dimmer_switch)input->device).dim_level.needs_update=0;
      ((struct dimmer_switch)input->device).on.needs_update=0;
      ((struct dimmer_switch)input->device).off.needs_update=0;
      ((struct dimmer_switch)input->device).dim_level.value=0;
      ((struct dimmer_switch)input->device).on.value=0;
      ((struct dimmer_switch)input->device).off.value=0;
      ((struct dimmer_switch)input->device).realbutton=inputs[real_button];
      ((struct dimmer_switch)input->device).timer=0;
}
void on_off_init(unsigned int dim_adr,unsigned int on_adr,unsigned int off_adr,struct inputs* input,unsigned int real_button)
{
      struct on_off_switch sw;
      input->type=on_off_switch;
      sw.on.adress=on_adr;
      sw.off.adress=off_adr;
      sw.previous_state=button_depressed;
      sw.outstate=off;
      sw.on.needs_update=0;
      sw.off.needs_update=0;
      sw.on.value=0;
      sw.off.value=0;
      sw.realbutton=inputs[real_button];
      input->device=sw;
}
void button_init(unsigned int dim_adr,unsigned int on_adr,unsigned int off_adr,struct inputs* input,unsigned int real_button)
{
      struct button_switch sw;
      input->type=button_switch;
      sw.on.adress=on_adr;
      sw.off.adress=off_adr;
      sw.previous_state=button_depressed;
      sw.outstate=off;
      sw.on.needs_update=0;
      sw.off.needs_update=0;
      sw.on.value=0;
      sw.off.value=0;
      sw.realbutton=inputs[real_button];
      input->device=sw;
}
//////////////////////////////////////////////////////////////////////////////

///////INPUTS INITIALIZATION//////////////////////////////////////////////////
void dimmer_out_init(unsigned int *dim_adr,unsigned int *on_adr,unsigned int *off_adr,struct outputs* output,unsigned int output_pin)
{
   ((struct outputs *)output)->type=dimmer;
   ((struct light)output->device).dim_value.needs_update=0;
   ((struct light)output->device).on.needs_update=0;
   ((struct light)output->device).off.needs_update=0;
   ((struct light)output->device).dim_value.value=0;
   ((struct light)output->device).on.value=0;
   ((struct light)output->device).off.value=0;
   ((struct light)output->device).output_pin=output_pin;
   ((struct light)output->device).dim_value.adress[0]=1;
   ((struct light)output->device).dim_value.adress[1]=2;
   printf("adress test:%u %u\n\r",((struct light)output->device).dim_value.adress[0],((struct light)output->device).dim_value.adress[1]);
   copy_array(dim_adr,((struct light)output->device).dim_value,8);
   copy_array(on_adr,((struct light)output->device).on,8);
   copy_array(off_adr,((struct light)output->device).off,8);
  ((struct light)output->device).out_state=_off;
}
//////////////////////////////////////////////////////////////////////////////

void button_test()
{     mydevices.numberOfInputs=1;
    //  struct dimmer_switch sw;
    //  sw.dim_level.value=69;
    //  mydevices.myinputs[0].device=sw;
    //  printf("VALUE=%u\n\r",mydevices.myinputs[0].device.dim_level.value);
    //  on_off_init(1,2,3,&mydevices.myinputs[0],0);
    //button_init(1,2,3,&mydevices.myinputs[0],0);
      dimmer_init(1,2,3,&mydevices.myinputs[0],0);
      //printf("VALUE=%u\n\r",mydevices.myinputs[0].device.dim_level.value);
     // while(true){};
}


