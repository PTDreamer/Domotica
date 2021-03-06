#define N_LUZES 16 //numero maximo de dimmers
volatile int used_dimmers=0;

//#define printf_ printf
#define printf_ //
int1 organizado;//flag que diz se os dimmings estao organizados
int1 actmat=0;//matriz em uso
int16 pointer;//apontador para matriz em uso
int vez;//indice da matriz de dimming corrente
volatile int mnumluzes=0;//quantos valores de dimming diferentes temos
volatile unsigned int16 onoffsvalue=0xFFFF;
int ltlevel[N_LUZES]={50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50};
volatile int16 delays1[N_LUZES+1][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15};
volatile int16 delays2[N_LUZES+1][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15};
const unsigned int16 light_pins[N_LUZES]={
0b1111111111101111,0b1111111111011111,0b1111111110111111,0b1111111101111111,0b1111111111110111,0b1111111111111011,0b1111111111111110,0b1111111111111101,
/*<........................................................................portD..................................................................................>*/
0b0111111111111111,0b1011111111111111,0b1101111111111111,0b1110111111111111,0b1111011111111111,0b1111101111111111,0b1111110111111111,0b1111111011111111};
/*<.................................................................................portC.............................................................>*/

unsigned int16 lights[N_LUZES];
const long Matrizluz[128]={
38400,38731 , 38492 , 38253 , 38014 , 37775 , 37536 , 37297 , 37058 , 36819 , 36580
, 36341 , 36102 , 35863 , 35624 , 35385 , 35146 , 34907 , 34668 , 34429 , 34190 , 33951
 , 33712 , 33473 , 33234 , 32995 , 32756 , 32517 , 32278 , 32039 , 31800 , 31561 , 31322
 , 31083 , 30844 , 30605 , 30366 , 30127 , 29888 , 29649 , 29410 , 29171 , 28932 , 28693
 , 28454 , 28215 , 27976 , 27737 , 27498 , 27259 , 27020 , 26781 , 26542 , 26303 , 26064
 , 25825 , 25586 , 25347 , 25108 , 24869 , 24630 , 24391 , 24152 , 23913 , 23674 , 23435
 , 23196 , 22957 , 22718 , 22479 , 22240 , 22001 , 21762 , 21523 , 21284 , 21045 , 20806
 , 20567 , 20328 , 20089 , 19850 , 19611 , 19372 , 19133 , 18894 , 18655 , 18416 , 18177
 , 17938 , 17699 , 17460 , 17221 , 16982 , 16743 , 16504 , 16265 , 16026 , 15787 , 15548
 , 15309 , 15070 , 14831 , 14592 , 14353 , 14114 , 13875 , 13636 , 13397 , 13158 , 12919
 , 12680 , 12441 , 12202 , 11963 , 11724 , 11485 , 11246 , 11007 , 10768 , 10529 , 10290
 , 10051 , 9812 , 9573 , 9334 , 9095 , 8856 , 617 };
 
 
 #INLINE
  INT16 fpointer(int ax,int ay)
   {
      int16 * p;
      p=(pointer + 2* ax * 2 + 2*ay);
     RETURN *(int16*)(pointer + 2* ax * 2 + 2*ay);
   }

  
  VOID org(void)
  {
    // printf("org ");
     INT16 tempd[N_LUZES][2];
     INT numluzes=0;
     INT1 again = 1;
     INT fa;
     INT16 fl;
     INT afa;
     FOR(fa = 0;fa < used_dimmers;++fa)
     {
        tempd[fa][0] = ltlevel[fa];//tempd[x][0] fica com o valor de dimming 0...127
        tempd[fa][1] = lights[fa];//tempd[x][1] fica com o valor binario DO porto de saida
     }

     WHILE(again)
     {
        again = 0;
        FOR(fa = 0;fa < used_dimmers-1;++fa)
        {
           //coloca valores por ordem tempd[0][0] o maior
           IF(tempd[fa][0] < tempd[fa + 1][0])
           {
              again = 1;
              fl = tempd[fa][0];
              tempd[fa][0] = tempd[fa + 1][0];
              tempd[fa + 1][0] = fl;
              fl = tempd[fa][1];
              tempd[fa][1] = tempd[fa + 1][1];
              tempd[fa + 1][1] = fl;
           }
        }
     }
   /*   for(temp=0;temp<used_dimmers;++temp)
   {
      printf("DIMMER:%u value=%lu <-> portWrite=%lu\n\r",temp,tempd[temp][0],tempd[temp][1]);
   }*/
     IF(tempd[0][0] == 0)
     {
         numluzes = 0;
         //luzes todas off
         //printf("TUDO OFF");
     }
     ELSE
     {
        numluzes = 1;
        afa = 0;
        IF(actmat)
        {
           delays1[0][0] = tempd[0][0];
           delays1[0][1] = tempd[0][1];
        }

        ELSE
        {
           delays2[0][0] = tempd[0][0];
           delays2[0][1] = tempd[0][1];
        }

        FOR(fa = 1;fa < used_dimmers;++fa)
        {
           IF((tempd[fa][0] == tempd[fa - 1][0])&( ! tempd[fa][0] == 0))
           {
              IF(actmat)delays1[afa][1] = delays1[afa][1]&tempd[fa][1];
              ELSE delays2[afa][1] = delays2[afa][1]&tempd[fa][1];
           }

           else IF( ! tempd[fa][0] == 0)
           {
              ++afa;
              IF(actmat)
              {
                 delays1[afa][0] = tempd[fa][0];
                 delays1[afa][1] = tempd[fa][1];
              }

              ELSE
              {
                 delays2[afa][0] = tempd[fa][0];
                 delays2[afa][1] = tempd[fa][1];
              }
     //          printf("numluzes=%d used_dimmers=%d",numluzes,used_dimmers);
              ++numluzes;
           }
        }
     }
     // printf("XnumluzesX=%d used_dimmers=%d",numluzes,used_dimmers);
     IF(actmat)delays1[N_LUZES][0] = numluzes;
     ELSE  delays2[N_LUZES][0] = numluzes;
   //  printf("num luzes activas=%d valor primeira=%lu portWrite=%lu\n\r",numluzes,delays1[0][0],delays1[0][1]);
   //  printf("num luzes activas=%d valor primeira=%lu portWrite=%lu\n\r",numluzes,delays2[0][0],delays2[0][1]);
    // delays2[0][1]=27498;
    // delays1[0][1]=27498;
     organizado=1;
  }

void dimmer_outputs_init()
{
   delays1[N_LUZES][0]=0;
   delays2[N_LUZES][0]=0;
   pointer=delays1;
   actmat=0;
   portc=0xFF;
   portd=0xFF;
  // trisc=0x00; //tudo saidas
  // trisd=0x00; //tudo saidas
   SET_TRIS_C( 0x00 );
   SET_TRIS_D( 0x00 );
   portc=0xFF;
   portd=0xFF;
  
   int x;
   for(x=0;x<mydevices.numberOfOutputs;++x)
   {
      switch (((struct outputs)mydevices.myoutputs[x]).type) {
         case dimmer:
#ifdef DEBUG
         printf("used dimmer!!!\n\r");
#endif
            lights[used_dimmers]=light_pins[((struct light)mydevices.myoutputs[x].device).output_pin];
            ((struct light)mydevices.myoutputs[x].device).internal_order=used_dimmers;
            ++used_dimmers;          
         break;
      }
   }
   if(used_dimmers>0)
   {
   actmat=0;
   pointer=delays1;
   org();
   pointer=delays2;
   actmat=1;
   org();
   }
}

void write_outputs()
{
   volatile int x;
   int1 update_dimmers=false;
   for(x=0;x<mydevices.numberOfOutputs;++x)
   {
     // printf_("%d",x);
      switch (((struct outputs)mydevices.myoutputs[x]).type) {
         case dimmer:
            if(((struct light)mydevices.myoutputs[x].device).dim_value.needs_update)
            {
               if(((struct light)mydevices.myoutputs[x].device).out_state==_on)
               {
                  ltlevel[((struct light)mydevices.myoutputs[x].device).internal_order]=((struct light)mydevices.myoutputs[x].device).dim_value.value;
                //  printf_("ltlevel[%d]=%d\n\r",((struct light)mydevices.myoutputs[x].device).internal_order,((struct light)mydevices.myoutputs[x].device).dim_value.value);
                  update_dimmers=true;
               }
               ((struct light)mydevices.myoutputs[x].device).dim_value.needs_update=false;
            }
            
            if(((struct light)mydevices.myoutputs[x].device).off.needs_update)
            {
               if(((struct light)mydevices.myoutputs[x].device).off.value)
               {
                  ((struct light)mydevices.myoutputs[x].device).out_state=_off;
                  ltlevel[((struct light)mydevices.myoutputs[x].device).internal_order]=0;
                  update_dimmers=true;
               }
               ((struct light)mydevices.myoutputs[x].device).off.needs_update=false;
            }
            
            if(((struct light)mydevices.myoutputs[x].device).on.needs_update)
            {
               if(((struct light)mydevices.myoutputs[x].device).on.value)
               {
                  ((struct light)mydevices.myoutputs[x].device).out_state=_on;
                  ltlevel[((struct light)mydevices.myoutputs[x].device).internal_order]=((struct light)mydevices.myoutputs[x].device).dim_value.value;
                  update_dimmers=true;
               }
               ((struct light)mydevices.myoutputs[x].device).on.needs_update=false;
            }
         break;
         case _on_off:
            if(((struct oNoFF)mydevices.myoutputs[x].device).off.needs_update)
            {
               if(((struct oNoFF)mydevices.myoutputs[x].device).off.value)
               {
                  onoffsvalue=onoffsvalue|~light_pins[((struct oNoFF)mydevices.myoutputs[x].device).output_pin];
               }
               ((struct oNoFF)mydevices.myoutputs[x].device).off.needs_update=false;
            }
            if(((struct oNoFF)mydevices.myoutputs[x].device).on.needs_update)
            {
               if(((struct oNoFF)mydevices.myoutputs[x].device).on.value)
               {
                  onoffsvalue=onoffsvalue & light_pins[((struct oNoFF)mydevices.myoutputs[x].device).output_pin];
                 
               }
               ((struct light)mydevices.myoutputs[x].device).on.needs_update=false;
            }
         break;
           case shutter:
           if(((struct shutter)mydevices.myoutputs[x].device).up.needs_update && ((((struct shutter)mydevices.myoutputs[x].device).state==idle)||(((struct shutter)mydevices.myoutputs[x].device).state==goingUp)))
            {
               if(((struct shutter)mydevices.myoutputs[x].device).up.value)
               {
                     ((struct shutter)mydevices.myoutputs[x].device).internalCounter=0;
                     ((struct shutter)mydevices.myoutputs[x].device).state=goingUp;
                     onoffsvalue=onoffsvalue & light_pins[((struct shutter)mydevices.myoutputs[x].device).output_pin_up];
               }
               ((struct shutter)mydevices.myoutputs[x].device).up.needs_update=false;
            }
            if(((struct shutter)mydevices.myoutputs[x].device).down.needs_update && ((((struct shutter)mydevices.myoutputs[x].device).state==idle)||(((struct shutter)mydevices.myoutputs[x].device).state==goingDown)))
            {
               if(((struct shutter)mydevices.myoutputs[x].device).down.value)
               {
                   ((struct shutter)mydevices.myoutputs[x].device).internalCounter=0;
                   ((struct shutter)mydevices.myoutputs[x].device).state=goingDown;
                   onoffsvalue=onoffsvalue & light_pins[((struct shutter)mydevices.myoutputs[x].device).output_pin_down];
               }
               ((struct shutter)mydevices.myoutputs[x].device).down.needs_update=false;
            }
            if(((struct shutter)mydevices.myoutputs[x].device).fullup.needs_update && ((((struct shutter)mydevices.myoutputs[x].device).state==idle)||(((struct shutter)mydevices.myoutputs[x].device).state==goingFullUp)))
            {
               if(((struct shutter)mydevices.myoutputs[x].device).fullup.value)
               {
                   ((struct shutter)mydevices.myoutputs[x].device).internalCounter=0;
                   ((struct shutter)mydevices.myoutputs[x].device).state=goingFullUp;
                   onoffsvalue=onoffsvalue & light_pins[((struct shutter)mydevices.myoutputs[x].device).output_pin_up];
               }
               ((struct shutter)mydevices.myoutputs[x].device).fullup.needs_update=false;
            }          
            if(((struct shutter)mydevices.myoutputs[x].device).fulldown.needs_update && ((((struct shutter)mydevices.myoutputs[x].device).state==idle)||(((struct shutter)mydevices.myoutputs[x].device).state==goingFullDown)))
            {
               if(((struct shutter)mydevices.myoutputs[x].device).fulldown.value)
               {
                   ((struct shutter)mydevices.myoutputs[x].device).internalCounter=0;
                   ((struct shutter)mydevices.myoutputs[x].device).state=goingFullDown;
                   onoffsvalue=onoffsvalue & light_pins[((struct shutter)mydevices.myoutputs[x].device).output_pin_down];
               }
               ((struct shutter)mydevices.myoutputs[x].device).fulldown.needs_update=false;
            }
           
            break;
   }
   }
   if(update_dimmers)
      org();
}

void dimmer_test()
{
   mydevices.numberOfoutputs=1;
   unsigned int dim_adr[8]={1,21,31,255,255,255,255,255};
  // unsigned int off_adr[8]={3,51,61,255,255,255,255,255};
  // unsigned int on_adr[8]={2,81,91,255,255,255,255,255};
   unsigned int timedon_adr[8]={4,81,91,255,255,255,255,255};
   unsigned int empty[8]={255,255,255,255,255,255,255,255};
   //dimmer_out_init(dim_adr,on_adr,off_adr,&mydevices.myoutputs[0],0);
  // shutter_out_init(on_adr,empty,empty,empty,&mydevices.myoutputs[0],0,1,3,10);
   unsigned int off_adr[8]={2,255,255,255,255,255,255,255};
   unsigned int on_adr[8]={1,255,255,255,255,255,255,255};
   onOff_out_init(on_adr,off_adr,&mydevices.myoutputs[0],4);
  //onOff_out_init(timedon_adr,off_adr,&mydevices.myoutputs[0],0);
}
