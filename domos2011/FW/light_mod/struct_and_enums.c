//enum switch_type {on_off,dimmer,button};
enum switch_outstate {on, off, dimming, dimmed, maxed};
enum switch_direction {up, down};
enum switch_state {button_pressed, button_depressed};
enum output_type {_on_off, _dimmer,shutter};
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
   unsigned int  out_value;
   unsigned int  timer;
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
      unsigned int numberOfInputs;
      unsigned int numberOfOutputs;
      struct inputs myinputs[10];
      struct outputs myoutputs[10];
}devicestype;

devicestype mydevices;
void dimmer_init(int dim_adr,int on_adr,int off_adr,struct inputs* input,int real_button)
{
      struct dimmer_switch sw;
      input->type=dimmer_switch;
      sw.dim_level.adress=dim_adr;
      sw.on.adress=on_adr;
      sw.off.adress=off_adr;
      sw.previous_state=button_depressed;
      sw.outstate=off;
      sw.current_level=0;
      sw.direction=up;
      sw.dim_level.needs_update=0;
      sw.on.needs_update=0;
      sw.off.needs_update=0;
      sw.dim_level.value=0;
      sw.on.value=0;
      sw.off.value=0;
      sw.realbutton=inputs[real_button];
      sw.timer=0;
      input->device=sw;
}
void on_off_init(int dim_adr,int on_adr,int off_adr,struct inputs* input,int real_button)
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
void button_init(int dim_adr,int on_adr,int off_adr,struct inputs* input,int real_button)
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


void test()
{     mydevices.numberOfInputs=1;
    //  struct dimmer_switch sw;
    //  sw.dim_level.value=69;
    //  mydevices.myinputs[0].device=sw;
    //  printf("VALUE=%u\n\r",mydevices.myinputs[0].device.dim_level.value);
    //  on_off_init(1,2,3,&mydevices.myinputs[0],0);
    button_init(1,2,3,&mydevices.myinputs[0],0);
     // dimmer_init(1,2,3,&mydevices.myinputs[0],0);
      //printf("VALUE=%u\n\r",mydevices.myinputs[0].device.dim_level.value);
     // while(true){};
}


