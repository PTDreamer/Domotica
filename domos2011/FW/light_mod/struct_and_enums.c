enum switch_type {on_off,dimmer,button};
enum switch_outstate {on, off, dimming, dimmed};
enum switch_direction {up, down};
enum switch_state {button_pressed, button_depressed};
enum output_type {_on_off, _dimmer,shutter};
enum output_state {_on, _off, _dimmed};
enum timer_type {time_on, time_off, periodic_on};
enum input_type {switch_,timer,none};

void dimmer_init(int dim_adr,int on_adr,int off_adr,struct inputs* input);
typedef struct data_point_out
{
   int adress;
   int value;
   int1 needs_update;
};
typedef struct data_point_in
{
   int adress[8];
   int value;
   int1 needs_update;
};
///////////////////////INPUTS//////////////////////////////////////
typedef struct switches
{
   switch_type type;
   struct data_point_out dim_level;
   struct data_point_out on;
   struct data_point_out off;
   int current_level;
   int32  timer;
   switch_state current_state;
   switch_state previous_state;
   switch_direction direction;
   switch_outstate outstate;
   int realbutton;
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
   struct switches;
   struct timer;
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
   int  out_value;
   int  timer;
   int1  needs_update;
   output_state previous_state;
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
      int numberOfInputs;
      int numberOfOutputs;
      struct inputs myinputs[10];
      struct outputs myoutputs[10];
}devicestype;

devicestype mydevices;
void dimmer_init(int dim_adr,int on_adr,int off_adr,struct inputs* input,int real_button)
{
      input->type=switch_;
      input->device.dim_level.adress=dim_adr;
      input->device.on.adress=on_adr;
      input->device.off.adress=off_adr;
      input->device.previous_state=button_depressed;
      input->device.outstate=off;
      input->device.current_level=0;
      input->device.direction=up;
      input->device.dim_level.needs_update=0;
       input->device.on.needs_update=0;
      input->device.off.needs_update=0;
      
      input->device.dim_level.value=0;
      input->device.on.value=0;
      input->device.off.value=0;
      
      input->device.type=dimmer;
      input->device.realbutton=inputs[real_button];
}




void test()
{     mydevices.numberOfInputs=1;
      dimmer_init(1,2,3,&mydevices.myinputs[0],0);
}


