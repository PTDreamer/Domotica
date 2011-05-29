#define full 0
#define end_dest 1
#define sub_end_dest 2
#define end_orig 3
#define sub_end_orig 4
#define tipo 5
#define comp_dados 6
#define dados 7

//TIPOS
#define Set_Value 1
#define Get_Value 2
#define Value_Ans 3
#define Ping_Request 4
#define Ping_Ans 5
#define Set_Configuration 6
#define Get_Configuration 7
#define Configuration_Ans 8
#define Reset 9
#define Direct_Adress 10
#define Indirect_adress_config 11
#define Indirect_Set_Value 12
#define man_val_change 13
#define newadr 14
#define Set_virtual_int 15
#define Get_virtual_int 16
#define Virtual_int_Ans 17
#define Set_glob_adr    18
#define Get_glob_adr    19
#define glob_adr_ans    20


int iadress=0;
int pilha_can[11];
int pilha_canTX[11];
int daux;
int aux33;

void send_to_requester()
{
   pilha_canTX[1]=pilha_can[3];
   pilha_canTX[2]=pilha_can[4];
   pilha_canTX[3]=pilha_can[1];
   pilha_canTX[4]=pilha_can[2];
}

void recebe_can()
{
   int zaux;
   if (f2510rx(pilha_can)==0)
   {
      #IF CDEBUG 
      printf("mensagem\n\r");
      #ENDIF
         switch(pilha_can[tipo])
         {
            case Set_Value:
               if(pilha_can[sub_end_dest]<8) 
               {
                  ltlevel[pilha_can[sub_end_dest]]=pilha_can[dados];
                  oldltlevel[pilha_can[sub_end_dest]]=pilha_can[dados];
               }
               else if(pilha_can[sub_end_dest]==0x69)
               {
                  
                  for (zaux=0;zaux<8;++zaux)
                  {  
                     if(GAD[zaux]==1)
                     {
                        aux33=pilha_can[dados];
                        if(LOF[zaux]==0)
                        {
                           if(aux33<64) aux33=0;
                           else aux33=0x7F;
                        }
                        ltlevel[zaux]=aux33;
                        oldltlevel[zaux]=aux33;
                     }
                  }
               }
            org();
            pilha_can[full]=0;
            break;
            case Get_Value:
               if (pilha_canTX[full]==0)
               {
                  pilha_canTX[full]=1;
                  send_to_requester();
                  pilha_canTX[tipo]=3;
                  pilha_canTX[comp_dados]=1;
                  pilha_canTX[dados]=ltlevel[pilha_can[sub_end_dest]];
                  f2510tx(pilha_canTX);
                  pilha_can[full]=0;
               }
            break;
            case Ping_Request:
                   #IF CDEBUG 
                   printf("received ping\n\r");
                   printf("pilha_canTX[0]=%X \n\r",pilha_canTX[full]);
                   #ENDIF 
                   if (pilha_canTX[full]==0)
                   {
                      pilha_canTX[full]=1;
                      send_to_requester();
                      pilha_canTX[tipo]=Ping_Ans;
                      pilha_canTX[comp_dados]=0;
                      daux=f2510tx(pilha_canTX);
                      #IF CDEBUG 
                      printf("send res:%X\n\r",daux);
                      #ENDIF
                      pilha_can[full]=0;
                   }
            break;
            case Indirect_adress_config:
               iadress=pilha_can[dados];
               pilha_can[full]=0;
            break;
            case Direct_Adress:
               iadress=0;
               pilha_can[full]=0;
            break;
            case Indirect_Set_Value:
               if(iadress==0) {
               pilha_can[full]=0;
               break;
               }
               for (zaux=0;zaux<8;++zaux)
               {
                  if (bit_test(iadress,zaux)) ltlevel[zaux]=pilha_can[dados];
               }
               org();
               pilha_can[full]=0;
            break;
            case reset:
               pilha_can[full]=0;
               reset_cpu();
            break;
            case newadr:
               write_eeprom(save_adr,pilha_can[dados]);
               pilha_can[full]=0;
               delay_ms(1000);
               reset_cpu();
            break;
            case Set_Configuration:
               write_eeprom(cfg_adr,pilha_can[dados]);
               delay_ms(1000);
               pilha_can[full]=0;
               reset_cpu();
               
            break;
            case Get_Configuration:
                   if (pilha_canTX[full]==0)
                   {
                      pilha_canTX[full]=1;
                      send_to_requester();
                      pilha_canTX[tipo]=Configuration_Ans;
                      pilha_canTX[comp_dados]=1;
                      aux33=read_eeprom(cfg_adr);
                      delay_ms(1000);
                      pilha_canTX[dados]=aux33;
                      daux=f2510tx(pilha_canTX);
                      pilha_can[full]=0;
                   }
            break;
            case Set_virtual_int:
               write_eeprom(cfg2_adr,pilha_can[dados]);
               delay_ms(1000);
               //cfgload();
               pilha_can[full]=0;
               reset_cpu();
            break;
            case Get_virtual_int:
                   if (pilha_canTX[full]==0)
                   {
                      pilha_canTX[full]=1;
                      send_to_requester();
                      pilha_canTX[tipo]=Virtual_int_Ans;
                      pilha_canTX[comp_dados]=1;
                      aux33=read_eeprom(cfg2_adr);
                      delay_ms(1000);
                      pilha_canTX[dados]=aux33;
                      daux=f2510tx(pilha_canTX);
                      pilha_can[full]=0;
                   }
            break;
            case Set_glob_adr:
               write_eeprom(cfg3_adr,pilha_can[dados]);
               delay_ms(1000);
               //cfgload();
               pilha_can[full]=0;
               reset_cpu();
            break;
            case Get_glob_adr:
                   if (pilha_canTX[full]==0)
                   {
                      pilha_canTX[full]=1;
                      send_to_requester();
                      pilha_canTX[tipo]=glob_adr_ans;
                      pilha_canTX[comp_dados]=1;
                      aux33=read_eeprom(cfg2_adr);
                      delay_ms(1000);
                      pilha_canTX[dados]=aux33;
                      daux=f2510tx(pilha_canTX);
                      pilha_can[full]=0;
                   }
            break;



      }
   }
}
