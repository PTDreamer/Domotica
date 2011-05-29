  void swit_virtual(int n)
  {
     int clockorg;
     int1 a;
     clockorg=clock;
     if (LOF[n]!=2) return;
     
     switch (n)
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
     
     if (s_virtual[n]!=a)
     {
         pilha_canTX[full]=1;
         pilha_canTX[end_dest]=0;
         pilha_canTX[sub_end_dest]=0;
         pilha_canTX[end_orig]=adr;
         pilha_canTX[comp_dados]=1;
         pilha_canTX[sub_end_orig]=n;
         pilha_canTX[tipo]=man_val_change;
         pilha_canTX[dados]=a;
         f2510tx(pilha_canTX);
         s_virtual[n]=a;
     }
     clock=clockorg;
  }
