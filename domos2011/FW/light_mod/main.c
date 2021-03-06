
#include "main.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\global_defs.h"
#include "C:\Users\Programacao\Documents\CCS_Projects\18F\LightMod\can_functions.c"
int temp;
#define DEBUG TRUE
#include "struct_and_enums.c"
#include "dimming_functions.c"

#include "datapoints.c"
#include "timedevents.c"
#include "debug.c"
#include "eprom.c"
//////////////////////////////////////////
//apagar
volatile unsigned int32 clock;
volatile unsigned int1 secondFlag;
volatile unsigned int1 syncError;
volatile unsigned int1 oscError=0;
volatile unsigned long misscounter;
//dimmer constants
static int TimeBase=0;
static int32 reg=39;
int16 ledErrorCounter;
//dimmer end

//struct switches msw;
#include "switch_functions.c"
#include "interrupt_functions.c"
#include "hw_setup.c"
//////////////////////////////////////////////////////////////////////////

void main()
{
   mydevices.numberOfInputs=0;
   mydevices.numberOfOutputs=0;
   clock = 0;
   ledErrorCounter=0;   
   hw_setup();
   //TODO: User Code
   //struct switches array[NUMBER_OF_SWITCHES];
  /*
   button_test();
   dimmer_test();
   */
   
   
   ///////////////SALA//////////////////////////////
 /*  eeprom_on_off_init(20,21,6);//2 int Grande a comecar da esquerda
   eeprom_on_off_init(22,23,0);//2 
   //eeprom_dimmer_init(40,41,42,7);//2
   eeprom_on_off_init(41,42,7);//2
   eeprom_on_off_init(26,27,3);//2
   
   
   
   ///Sanca parede grande
   unsigned int on_adr[8]={20,255,255,255,255,255,255,255};
   unsigned int off_adr[8]={21,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,3);//Sanca parede grande
   eeprom_onOff_out_init(on_adr,off_adr,5);//Sanca parede grande
   
   ///Sanca parede rosa
   unsigned int on_adrr[8]={22,255,255,255,255,255,255,255};
   unsigned int off_adrr[8]={23,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adrr,off_adrr,7);//Sanca parede grande
   on_adrr[1]=26;
   off_adrr[1]=27;
   eeprom_onOff_out_init(on_adrr,off_adrr,4);//Sanca parede grande
   
    ///VARANDA
   unsigned int dimmer_dim_adr[8]={40,255,255,255,255,255,255,255}; 
   unsigned int dimmer_on_adr[8]={41,255,255,255,255,255,255,255};
   unsigned int dimmer_off_adr[8]={42,255,255,255,255,255,255,255};
   //eeprom_dimmer_out_init(dimmer_dim_adr,dimmer_on_adr,dimmer_off_adr,6);//varanda
   eeprom_onOff_out_init(dimmer_on_adr,dimmer_off_adr,6);//varanda
   
   eeprom_button_init(28,29,4,true);//estores down
   eeprom_button_init(30,31,5,true);//estores up
   unsigned int up_adr[8]={30,255,255,255,255,255,255,255};
   unsigned int down_adr[8]={28,255,255,255,255,255,255,255};
   unsigned int x_adr[8]={255,255,255,255,255,255,255,255};
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,8,9,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,10,11,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,12,13,0,10);
   eeprom_shutter_out_init(up_adr,down_adr,x_adr,x_adr,14,15,0,10);
   */
   /////////////////////////////////////////////////7
   
   /*
   ///////////////QUARTO GRANDE//////////////////////////////
   eeprom_on_off_init(1,2,2);//2 on_adr off_adr real_button inicia botao
   unsigned int on_adr[8]={1,255,255,255,255,255,255,255};
   unsigned int off_adr[8]={2,10,255,255,255,255,255,255};
 
   unsigned int nill_adr[8]={255,255,255,255,255,255,255,255};  
   unsigned int ventax_off_adr[8]={16,255,255,255,255,255,255,255};

   unsigned int x_adr[8]={1,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,4);//SANCA?
   
   //outputs 4=sanca 3=casabanho 2=casa de banho 9=ventax 1=corredor
   ///casa de banho
   eeprom_on_off_init(3,4,4);//interruptor casa de banho??  
   off_adr[0]=4;
   on_adr[0]=3;
   eeprom_timer_init(off_adr,on_adr,15,16,time_off,60);
   eeprom_onOff_out_init(on_adr,off_adr,3); //luz casa de banho
   eeprom_onOff_out_init(on_adr,off_adr,2); //luz2 casa de banho
   eeprom_onOff_out_init(on_adr,ventax_off_adr,9);//VENTAX
   ///fim casa de banho
   
   //corredor
   eeprom_on_off_init(17,18,5);//interruptor luz corredor
   unsigned int corredor_on_adr[8]={17,255,255,255,255,255,255,255};
   unsigned int corredor_off_adr[8]={18,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(corredor_on_adr,corredor_off_adr,1); //luz corredor
   //fim corredor
   
   eeprom_button_init(5,6,0,true);//2 //estores
   eeprom_button_init(7,8,1,true);//2
   
   off_adr[0]=255;
   off_adr[1]=255;
   on_adr[0]=5;
   x_adr[0]=7;
   eeprom_shutter_out_init(on_adr,x_adr,off_adr,off_adr,11,10,0,10);//up down fullup full down
   
   eeprom_on_off_init(9,10,6);
   eeprom_on_off_init(20,21,7);
   unsigned int cama_on_adr[8]={9,255,255,255,255,255,255,255};
   unsigned int cama_off_adr[8]={10,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(cama_on_adr,cama_off_adr,7);
   cama_on_adr[0]=20;
   cama_off_adr[0]=21;
   eeprom_onOff_out_init(cama_on_adr,cama_off_adr,6);
   /////////////////////////////////////////////////7
   */ 
   /////////////////////QUARTO RENATA///////////////
   //
   // INPUTS 
   // 0,1 cozinha
   // 2,3 quarto
   //
   // OUTPUTS
   // 5 quarto renata economica
   // 6 janela hologeneo
   // 7 entrada
   // 2 cozinha fluorescente
   // 3 cima balcao halogeneo
   // 4 divisoria halogeneo
   // 10 balcao baixo verde
   // 11 balcao cima verde
   // 12 balcao cima vermelho
   // 13 balcao cima azul
   // 14 balcao baixo azul
   // 15 balcao baixo vermelho
   //INPUT INIT
   eeprom_on_off_init(60,61,0);//on_adr off_adr real_btn interruptor cozinha 1
   eeprom_on_off_init(62,63,1);//on_adr off_adr real_btn interruptor cozinha 2  
// eeprom_dimmer_init(68,62,63,1);//dim_adr on_adr off_adr
   eeprom_on_off_init(64,65,2);//on_adr off_adr real_btn interruptor quarto renata 1
//   eeprom_on_off_init(66,67,3);//on_adr off_adr real_btn interruptor quarto renata 2  
   eeprom_dimmer_init(68,66,67,3);//dim_adr on_adr off_adr
  
   //OUTPUT INIT
   unsigned int on_adr[8]={60,255,255,255,255,255,255,255};
   unsigned int off_adr[8]={61,10,255,255,255,255,255,255};
   unsigned int dim_adr[8]={68,255,255,255,255,255,255,255};
   eeprom_onOff_out_init(on_adr,off_adr,2); //luz cozinha fluorescente
   on_adr[0]=62;
   off_adr[0]=63;
   eeprom_onOff_out_init(on_adr,off_adr,3); //luz cozinha cima balcao halogeneo
   eeprom_onOff_out_init(on_adr,off_adr,4); //luz cozinha divisoria
   eeprom_onOff_out_init(on_adr,off_adr,15); //luz balcao baixo vermelho
   eeprom_onOff_out_init(on_adr,off_adr,12); //luz balcao cima vermelho
   //eeprom_dimmer_out_init(dim_adr,on_adr,off_adr,3); //luz cozinha cima balcao halogeneo
   //eeprom_dimmer_out_init(dim_adr,on_adr,off_adr,4); //luz cozinha divisoria
   //eeprom_dimmer_out_init(dim_adr,on_adr,off_adr,15); //luz cozinha divisoria
   
   on_adr[0]=64;
   off_adr[0]=65;
   eeprom_onOff_out_init(on_adr,off_adr,5); //luz quarto renata economica
   on_adr[0]=66;
   off_adr[0]=67;
  // eeprom_onOff_out_init(on_adr,off_adr,7); //luz quarto renata entrada
  // eeprom_onOff_out_init(on_adr,off_adr,6); //luz quarto renata janela
   eeprom_dimmer_out_init(dim_adr,on_adr,off_adr,7); //luz quarto renata entrada
   eeprom_dimmer_out_init(dim_adr,on_adr,off_adr,6); //luz quarto renata janela
   
  
   readDevices();
#ifdef DEBUG  
    printf("inputs:%d outputs:%d %d %d\n\r",mydevices.numberOfInputs,mydevices.numberOfOutputs,((struct outputs)mydevices.myoutputs[0]).type,((struct outputs)mydevices.myoutputs[1]).type);
#endif
   dimmer_outputs_init();
   // printf("start %Lu %Lu %Lu\n\r",fpointer(N_LUZES,0),delays1[N_LUZES][0],delays2[N_LUZES][0]); 
   /*((struct light)mydevices.myoutputs[0].device).dim_value.value=50;
   ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
   ((struct light)mydevices.myoutputs[0].device).off.value=1;
   ((struct light)mydevices.myoutputs[0].device).off.needs_update=true;
 */
 
   write_outputs();
// printf("start %Lu\n\r",fpointer(N_LUZES,0)); 
 interrupts_enable();

   while(true){
   restart_wdt();
   if(syncError || oscError)
   {  
      ++ledErrorCounter;
      if(ledErrorCounter>1000)
      {
         output_toggle(LED);
         ledErrorCounter=0;
      }
   }
#ifdef DEBUG
   if(kbhit())
   {
      setup_wdt(WDT_OFF);
      disable_interrupts (GLOBAL) ;
      goDebug();
   }
#endif
   process_outpoints();
   write_outputs();
   if(secondFlag)
   {
      secondFlag=false;
      processTimedEvents();
      if(!syncError && !oscError) output_toggle(LED);
   }
  // print_inputs(false);
  
}
}

