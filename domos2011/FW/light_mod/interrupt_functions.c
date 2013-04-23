#int_TIMER0
void  TIMER0_isr(void) 
{
   ++clock;
   if (bit_test (clock, 1)) read_inputs();
}

#int_TIMER1
void  TIMER1_isr(void) 
{
   syncError = 1;
      portc=MAKE8(onoffsvalue,1);
         portd=MAKE8(onoffsvalue,0);
         printf("MISS");
}
#int_TIMER2
void  TIMER2_isr(void)
{
   static unsigned int clockT2temp;
   //static int lixo=0;
   ++clockT2temp;
   if(clockT2temp==200)//1 second call
   {
      //output_toggle(LED);
      clockT2temp=0;
      ++secClock;
      secondFlag=true;
      /*
      if(!lixo)
      {
         ++((struct light)mydevices.myoutputs[0].device).dim_value.value;
         ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
      }
      else
      {
         --((struct light)mydevices.myoutputs[0].device).dim_value.value;
         ((struct light)mydevices.myoutputs[0].device).dim_value.needs_update=true;
      }
      if(((struct light)mydevices.myoutputs[0].device).dim_value.value==127)
         lixo=1;
      else if(((struct light)mydevices.myoutputs[0].device).dim_value.value==0)
         lixo=0;
      */
   }
}

#int_EXT
void  EXT_isr(void) 
{
/*
portc=MAKE8(onoffsvalue,1);
portd=MAKE8(onoffsvalue,0);
CCP_1=36819;
mnumluzes=0;
set_timer1(0);
return;
*/
         portc=MAKE8(onoffsvalue,1);
         portd=MAKE8(onoffsvalue,0);
   {
         if (organizado)
         {
            organizado=0;
            if (actmat) 
            {
               actmat=0;
               pointer=delays1;
            }
            else
            {
               actmat=1;
               pointer=delays2;
            }
         }
         vez=0;
         CCP_1=matrizluz[fpointer(0,0)];
         mnumluzes=fpointer(N_LUZES,0);
         
   }
         set_timer1(0);
         syncError=false;
}

#int_CCP1
void CCP1_isr(void) 
{  /*
portc=(portc & MAKE8(0b1111111111111110,1));
portd=(portd & MAKE8(0b1111111111111110,0));
return;*/
    // while(true){};
   if(mnumluzes!=0)
   {
      int16 auxccp=fpointer(vez,1);
      portc=(portc & MAKE8(auxccp,1));
      portd=(portd & MAKE8(auxccp,0));
      ++vez;
      --mnumluzes;
      if(mnumluzes!=0) CCP_1=matrizluz[fpointer(vez,0)];
   }
}

#int_RDA
void  RDA_isr(void) 
{
}

#int_TBE
void  TBE_isr(void) 
{
}

#int_BUSCOL
void  BUSCOL_isr(void) 
{
}

#int_OSCF
void  OSCF_isr(void) 
{
   oscError=1;
}

#int_canrx0
void canrx0_int ( ) 
{
   canReceive ();
   // TODO: add CAN recieve code here
}

#int_canrx1
void canrx1_int ( ) 
{
   canReceive ();
   // TODO: add CAN recieve code here
}

#int_cantx0
void cantx0_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_cantx1
void cantx1_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_cantx2
void cantx2_int ( ) 
{
   canTransmit ();
   // TODO: add CAN transmit code here
}

#int_canirx
void canirx_int ( ) 
{
   // TODO: add CAN IRX handling code here
}

#int_canerr
void canerr_int ( ) 
{
   // TODO: add CAN error handling code here
}

void interrupts_enable()
{
   //enable_interrupts (int_canrx0);
   //enable_interrupts (int_canrx1);
   //enable_interrupts (int_cantx0);
   //enable_interrupts (int_cantx1);
   //enable_interrupts (int_cantx2);
   //enable_interrupts (int_canirx);
   //enable_interrupts (int_canerr);
   enable_interrupts(INT_CCP1);
   enable_interrupts (int_TIMER0) ;
   enable_interrupts (int_TIMER1) ;
   enable_interrupts (int_TIMER2) ;
   enable_interrupts (int_EXT) ;
   //enable_interrupts (int_TBE) ;
   //enable_interrupts (int_BUSCOL) ;
   //enable_interrupts (int_CANIRX) ;
   //enable_interrupts (int_CANERR) ;
   //enable_interrupts (int_CANTX2) ;
   //enable_interrupts (int_CANTX1) ;
   //enable_interrupts (int_CANTX0) ;
   //enable_interrupts (int_CANRX1) ;
   //enable_interrupts (int_CANRX0) ;
   //enable_interrupts (int_OSCF) ;
   enable_interrupts (GLOBAL) ;
}
