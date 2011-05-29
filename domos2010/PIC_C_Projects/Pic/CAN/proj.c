#DEFINE CDEBUG 0

#include "proj.h"
#include "mcp2510.inc"
#include <stdlib.h>
#include "mcp.c"
#define save_adr  1


void ledtest(void);
void f2510dump(void);
void f2510cfg(void);
int1 f2510tx(int *pilha);
void f2510bs(int end,int bbit);
void f2510cfg_end(int end,int1 intr);
void f2510rst(void);
int f2510rx(int *bu);
void f2510bc(int end,int bbit);

INT1 DEBUG;
int pilha_rs232[12];
char Sstring[25];
char go[6];

int Saux;

unsigned int atoi_b16(char *s)
{
   unsigned int result = 0;
   int i;

   for (i=0; i<2; i++)  
   {
      if (*s >= 'A' && *s <= 'F')
         result = 16*result + (*s) - 'A' + 10;
      else if (*s >= '0' && *s <= '9')
         result = 16*result + (*s) - '0';

      s++;
   }

   return(result);
}


#int_RDA
void RDA_isr() 
{
   gets(Sstring);

   if(pilha_rs232[0]==1) return;

   if (!stricmp(Sstring,go))//go="DEBUG"
   {
      DEBUG=TRUE;
      printf("DEBUG MODE ON \n\r");
      disable_interrupts(INT_RDA);
      return;
   }

   if (Sstring[0]==':')
   {
      pilha_rs232[0]=1;                      // |trans_pending|
      pilha_rs232[1]=atoi_b16(&Sstring[1]);  // | dest_adr    |
      pilha_rs232[2]=atoi_b16(&Sstring[3]);  // | sub_dest_adr|
      pilha_rs232[3]=atoi_b16(&Sstring[5]);  // | source_adr  |
      pilha_rs232[4]=atoi_b16(&Sstring[7]);  // |sub_source_adr|
      pilha_rs232[5]=atoi_b16(&Sstring[9]);  // | type        |
      pilha_rs232[6]=atoi_b16(&Sstring[11]); // | length(data)|

      for (Saux=0;Saux<pilha_rs232[6];++Saux)// | data [0-5]  |
      {
         pilha_rs232[7+Saux]=atoi_b16(&Sstring[13+2*Saux]);
      }
    }
}



// buffer recepção rs232 - transmissao can
// | data [0-5]  |
// | length(data)|
// | type        |
// |sub_source_adr|
// | source_adr  |
// | sub_dest_adr|
// | dest_adr    |
// |trans_pending|

// trama
//   dados  - sub_adr
//          - end_orig
//          - type
//          - data





void main() 
{
   char string[30];
   char ch;
   int auxc1,auxcb,auxs;
   int pilha_can[11];

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_spi(spi_master|spi_h_to_l|spi_clk_div_4|spi_ss_disabled|spi_sample_at_end);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_OFF);
   setup_ccp2(CCP_OFF);
   enable_interrupts(INT_RDA);//tirar se quiser arrancar em debug
   disable_interrupts(global);

   DEBUG=false;
   
   #IF CDEBUG
      DEBUG=true;
   #ENDIF 
   strcpy(go,"debug");

   //START MCP2510***************************************************
   f2510rst();
   delay_ms(1000);
   f2510cfg();
   delay_ms(1000);
   auxc1=read_eeprom(save_adr);
   f2510cfg_end(auxc1,0);
   //****************************************************************

   pilha_rs232[0]=0;
   pilha_can[0]=0;
   enable_interrupts(global);

   while(true)
   {
      while(DEBUG==false)
      {
        
         auxs=f2510rx(pilha_can);
      
         if (auxs==1) printf("ERROR CAN_BUFFER FULL n\\r");
         
         else if (auxs==0) 
         {
            printf(":%X%X%X%X%X%X",pilha_can[1],pilha_can[2],pilha_can[3],pilha_can[4],pilha_can[5],pilha_can[6]);
   
            for(auxs=0;auxs<pilha_can[6];++auxs)
            {
               printf("%X",pilha_can[7+auxs]);
            }
            printf("\r");
            pilha_can[0]=0; }

            if(pilha_rs232[0]==1)
            {
               auxs=f2510tx(pilha_rs232);

               if (auxs==false) printf("ERROR TX CAN BUFFER FULL \n\r");
            }

   }




   while(DEBUG==TRUE)
   {

      printf("******************************\n\r");
      printf("*     1-Debug OFF            *\n\r");
      printf("*     2-MCP dump             *\n\r");
      printf("*     3-MCP config           *\n\r");
      printf("*     4-MCP Send             *\n\r");
      printf("*     5-configuracao de end  *\n\r");
      printf("*     6-MCP reset            *\n\r");
      printf("*     7-MCP receive          *\n\r");
      printf("*     8-clear receive buffer *\n\r");
      printf("*     g-SAVE Adress          *\n\r");
      printf("*     l-READ Adress          *\n\r");
      printf("******************************\n\r");

      
      ch=getc();
      printf("%C\r",ch);
      switch(ch)
      {
         case '1': printf("\n\r DEBUG MODE OFF \n\r");
                   DEBUG=FALSE;
                   pilha_can[0]=0;
                   pilha_rs232[0]=0;
                   enable_interrupts(INT_RDA);
                   enable_interrupts(global);

         break;
         case '2': f2510dump();
         break;
         case '3': f2510cfg();
         break;
         case '4': printf ("\n\r Introduza endereco de destino \n\r");
                   gets(string);

                   pilha_rs232[1]=atoi(string);
                   printf ("\n\r Introduza sub endereco de destino \n\r");
                   gets(string);

                   pilha_rs232[2]=atoi(string);
                   printf ("\n\r Introduza endereco de origem \n\r");
                   gets(string);
                   pilha_rs232[3]=atoi(string);

                   printf ("\n\r Introduza sub endereco de origem \n\r");
                   gets(string);
                   pilha_rs232[4]=atoi(string);


                   printf ("\n\r Introduza tipo \n\r");
                   gets(string);

                   pilha_rs232[5]=atoi(string);
                   printf ("\n\r Introduza tamanho dado \n\r");
                   gets(string);

                   pilha_rs232[6]=atoi(string);

                   for(auxc1=1;auxc1<pilha_rs232[6]+1;++auxc1){
                   printf ("\n\r Introduza dado %d\n\r",auxc1);
                   gets(string);

                   pilha_rs232[7+auxc1-1]=atoi(string);
                   }
                   auxcb=f2510tx(pilha_rs232);

                   printf ("\n\r resultado = %02x \n\r",auxcb);
         break;
         case '5': printf ("\n\r Introduza endereco \n\r");
                   gets(string);
                   auxc1=atoi(string);
                   f2510cfg_end(auxc1,0);
         break;
         case 'g': printf ("\n\r Introduza endereco \n\r");
                   gets(string);
                   auxc1=atoi(string);
                   write_eeprom(save_adr,auxc1);
         break;

         case 'l': auxc1=read_eeprom(save_adr);
                   printf("\n\r Endereco=%u\n\r",auxc1);
         break;

         case '6': f2510rst();
         break;

         case '7': printf("\n RESULTADO=%u\n",f2510rx(pilha_can));
                   printf("\n\r end_dest=%u \n\r",pilha_can[1]);
                   printf("\n\r sub_end_dest=%u \n\r",pilha_can[2]);
                   printf("\n\r end_orig=%u \n\r",pilha_can[3]);
                   printf("\n\r sub_end_orig=%u \n\r",pilha_can[4]);
                   printf("\n\r tipo=%u \n\r",pilha_can[5]);
                   printf("\n\r comp_dados=%u \n\r",pilha_can[6]);
                   for(auxc1=0;auxc1<pilha_can[6];++auxc1)
                   {
                    printf("\n\r dado %u=%u \n\r",auxc1+1,pilha_can[7+auxc1]);
                   }
         break;

         case '8': f2510bc(0x2c,0);
                   pilha_can[0]=0;
         break;



      }

   }
}

}

void ledtest(void)
{  int i;
   for(i=0;i<10;++i){
      output_high(pin_a0);
      delay_ms(1000);
      output_low(pin_a0);
      delay_ms(1000);
   }
}


void f2510dump(void)
{
   int rd[8],adrr,adr;
   printf("\n\r");
   printf("     0   1   2   3   4   5   6   7\n\r");
   for(adrr=0;adrr<16;++adrr)
      {output_low(cs);
       spi_write(d2510rd);
       spi_write(adrr);
       for(adr=0;adr<8;++adr)
       {
         rd[adr]=spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
         spi_read(0);
       }
       output_high(cs);
       printf("%02d-%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x\n\r",adrr,rd[0],rd[1],rd[2],rd[3],rd[4],rd[5],rd[6],rd[7]);
   }
   getch();
}



















