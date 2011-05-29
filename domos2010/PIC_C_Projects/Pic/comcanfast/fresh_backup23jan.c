#DEFINE CDEBUG 1

#define TimeBase 10
#define reg 200
#DEFINE ADRESS 3
#DEFINE baudrate 9600
//#DEFINE baudrate 57600
#define save_adr  1
#define  cfg_adr  2

#include "fresh.h"

#use fast_io(c)
#use fast_io(a)
#use fast_io(b)
#byte porta=0x05
#byte trisa=0x85
#byte portb=0x06
#byte trisb=0x86
#byte portc=0x07
#byte trisc=0x87
#include <stdlib.h>
#include "mcp2510.inc"
#include "mcp.c"

//vector com o nivel de cada lampada
int ltlevel[8]={0,0,0,0,0,0,0,0};

int LOF[8]={0,0,0,0,0,0,0,0};

const long Matrizluz[128]={
38400,38731 , 38492 , 38253 , 38014 , 37775 , 37536 , 37297 , 37058 , 36819 , 36580
, 36341 , 36102 , 35863 , 35624 , 35385 , 35146 , 34907 , 34668 , 34429 , 34190 , 33951
 , 33712 , 33473 , 33234 , 32995 , 32756 , 32517 , 32278 , 32039 , 31800 , 31561 , 31322 , 31083 , 30844 , 30605 , 30366 , 30127 , 29888 , 29649 , 29410 , 29171 , 28932 , 28693 , 28454 , 28215 , 27976 , 27737 , 27498 , 27259 , 27020 , 26781 , 26542 , 26303 , 26064 , 25825 , 25586 , 25347 , 25108 , 24869 , 24630 , 24391 , 24152 , 23913 , 23674 , 23435 , 23196 , 22957 , 22718 , 22479 , 22240 , 22001 , 21762 , 21523 , 21284 , 21045 , 20806 , 20567 , 20328 , 20089 , 19850 , 19611 , 19372 , 19133 , 18894 , 18655 , 18416 , 18177 , 17938 , 17699 , 17460 , 17221 , 16982 , 16743 , 16504 , 16265 , 16026 , 15787 , 15548 , 15309 , 15070 , 14831 , 14592 , 14353 , 14114 , 13875 , 13636 , 13397 , 13158 , 12919 , 12680 , 12441 , 12202 , 11963 , 11724 , 11485 , 11246 , 11007 , 10768 , 10529 , 10290 , 10051 , 9812 , 9573 , 9334 , 9095 , 8856 , 617 };


int1 organizado; //flag k diz se ja acabou a reorganizacao

const int lights[8]={0b11111110,0b11111101,0b11111011,0b11110111,0b11101111,0b11011111,0b10111111,0b01111111};
                 /*<............portA................................................> <.......portC........>*/
int mnumluzes;

int flag[8]={0b00000010,0b00000010,0b00000010,0b00000010,0b00000010,0b00000010
,0b00000010,0b00000010};

//Interruptores
#define sw1 pin_c7
#define sw2 pin_b1
#define sw3 pin_b2
#define sw4 pin_b3
#define sw5 pin_b4
#define sw6 pin_b5
#define sw7 pin_b6
#define sw8 pin_b7


int delays1[9][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7};
int delays2[9][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7};
// vector com as contagens do timer de cada lampada
int swcont[8]={0,0,0,0,0,0,0,0};
int1 actmat=0;//matriz em uso
int pointer;
int vez;
int auxccp;
int auxccp2;
int temp;
int clock;
int1 cl;
int prevclock;


#include "org.c"


   void cfgload(void)
   {
      int xau;
      int xxau;
      xxau=read_eeprom(cfg_adr);
      for (xau=0;xau<8;++xau)
      {
         LOF[xau]=bit_test(xxau,xau);
      }
   }   


#include "Lcan.c"

#inline
   int fpointer(int ax,int ay)
   {
      return *(pointer+ax*2+ay);
   }

#int_EXT   // desliga todos os triacs, poe o apontador (vez) para a primeira
      void EXT_isr(void) // lampada e carrega o comparador com o nivel da primeira lampada
      {
         ++clock;
         porta=0xff;
         portc=portc|0x03;
      
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
         mnumluzes=fpointer(8,0);
         set_timer1(0);
      }
   

#int_CCP1
void CCP1_isr(void) 
{
   if(mnumluzes!=0)
   {
      auxccp=fpointer(vez,1);
      porta=(porta & auxccp);
   
      if((auxccp & 0b11000000)!=0b11000000)
      {         
         auxccp2=0xff;
         #ASM
         rlf auxccp,1
         rlf auxccp2,1
         rlf auxccp,1
         rlf auxccp2,1
         #ENDASM
         portc=(portc & auxccp2);
      }
      
      ++vez;
      --mnumluzes;
      CCP_1=matrizluz[fpointer(vez,0)];
   }
}

int1 switches(int swit)
{
   // flag de mudanca de valor
   int1 vl;
   int1 a;
   vl=0;
 
   switch (swit)
   {
      case 0:  a=!input(sw1); //nao esquecer de tirar
               #IF CDEBUG 
               a=0;
               #ENDIF
      break;
      case 1:  a=!input(sw2);
      break;
      case 2:  a=!input(sw3);
      break;
      case 3:  a=!input(sw4);
      break;
      case 4:  a=!input(sw5);
      break;
      case 5:  a=!input(sw6);
      break;
      case 6:  a=!input(sw7);
               #IF CDEBUG 
               a=0;
               #ENDIF
      break;
      case 7:  a=!input(sw8);
               #IF CDEBUG 
               a=0;
               #ENDIF
      break;
   }

   if(a)//se o botao esta premido
   {
      bit_set(flag[swit],0);
      if (clock-prevclock>TimeBase)
      {
         #IF CDEBUG
         printf("sw=%U swcont[swit]=%U\n\r",swit,swcont[swit]);
         #ENDIF
         if((clock-swcont[swit]>reg||bit_test(flag[swit],2))&& LOF[SWIT]==1)
         {
            bit_set(flag[swit],2);
            vl=true;
            if(bit_test(flag[swit],1))
            {
               ++ltlevel[swit];
               if(ltlevel[swit]==127) bit_clear(flag[swit],1);
            }
            else
            {
               --ltlevel[swit];
               if(ltlevel[swit]==0) bit_set(flag[swit],1);
            }
         }
         prevclock=clock;
      }
   }
   else//se o botao nao esta premido
   {
      if (bit_test(flag[swit],0))//se o botao acabou de ser deslargado
      {
         bit_clear(flag[swit],0);
         if(bit_test(flag[swit],2))//se esteve a regular
         {   
            bit_clear(flag[swit],2);
            flag[swit]^=0b00000010; //inverte direccao
         }   
         else//se nao esteve a regular 
         {
            if (ltlevel[swit]==0)//se estava desligada 
            {
               ltlevel[swit]=127;//liga
               bit_clear(flag[swit],1);
               vl=true;
            }
            else 
            {
               ltlevel[swit]=0;//senao desliga
               bit_set(flag[swit],1);
               vl=true;
            }
         }
      }
      swcont[swit]=clock;
   }
   return vl;
}


void main() 
{
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_CLK_DIV_4);
   setup_counters(RTCC_INTERNAL,WDT_18MS);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_COMPARE_INT);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_CCP1);
   ext_int_edge(l_to_h);
   setup_spi(spi_master|spi_h_to_l|spi_clk_div_4|spi_ss_disabled|spi_sample_at_end);
   actmat=0;
   org();
   actmat=1;
   cfgload();
   while(organizado==0){}
   
   enable_interrupts(global);
   
   pilha_can[0]=0;
   pilha_canTX[0]=0;
   trisa=0x00; //tudo saidas
   trisb=0xff; //tudo entradas
   trisc=0b10010000; //modificar para CAN
   
   delay_ms(1000);
   f2510rst();
   delay_ms(1000);
   f2510cfg();
   delay_ms(1000);
   f2510cfg_end(read_eeprom(save_adr),0);
   
   #IF CDEBUG
   f2510cfg_end(ADRESS,0);
   #ENDIF
   
   delay_ms(1000);
   
   #IF CDEBUG
   printf("STARTado\r\n");
   #ENDIF

   while(true)
   { 
      cl=0;
   
      while(organizado==1){}
   
      recebe_can();

      for (temp=0;temp<8;++temp)
      {
         if(switches(temp))
         {
            if (pilha_canTX[full]==0)
            {
               pilha_canTX[full]=1;
               pilha_canTX[end_dest]=0;
               pilha_canTX[sub_end_dest]=0;
               pilha_canTX[end_orig]=ADRESS;
               pilha_canTX[sub_end_orig]=temp;
               pilha_canTX[tipo]=man_val_change;
               pilha_canTX[comp_dados]=1;
               pilha_canTX[dados]=ltlevel[temp];
               f2510tx(pilha_canTX);
            }
            cl=1;
         }
         
       
      }
      if(cl) org();       
   }
}
