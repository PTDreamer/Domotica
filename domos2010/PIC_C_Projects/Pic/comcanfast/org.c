//#inline
void org(void)
{  int clockorg;
   int tempd[8][2];
   int numluzes;
   int1 again=1;
   int fa;
   int fl;
   int afa;
   clockorg=clock;
   for (fa=0;fa<8;++fa)
   {

      if(oldltlevel[fa]!=ltlevel[fa])
      {
         if (pilha_canTX[full]==0)
         {
            pilha_canTX[full]=1;
            pilha_canTX[end_dest]=0;
            pilha_canTX[sub_end_dest]=0;
            pilha_canTX[end_orig]=adr;
            pilha_canTX[comp_dados]=1;
            pilha_canTX[sub_end_orig]=fa;
            pilha_canTX[tipo]=man_val_change;
            pilha_canTX[dados]=ltlevel[fa];
            f2510tx(pilha_canTX);
         }
         oldltlevel[fa]=ltlevel[fa];
      }

      tempd[fa][0]=ltlevel[fa];
      tempd[fa][1]=lights[fa];
   }

   while(again)
   {
      again=0;
      for (fa=0;fa<7;++fa)
      {
         if (tempd[fa][0]<tempd[fa+1][0])
         {
            again=1;
            fl=tempd[fa][0];
            tempd[fa][0]=tempd[fa+1][0];
            tempd[fa+1][0]=fl;
            fl=tempd[fa][1];
            tempd[fa][1]=tempd[fa+1][1];
            tempd[fa+1][1]=fl;
         }
      }
   }
   if (tempd[0][0]==0) numluzes=0;
   else
   {
      numluzes=1;
      afa=0;
      if (actmat)
      {   
         delays1[0][0]=tempd[0][0];
         delays1[0][1]=tempd[0][1];
      }

      else       
      {
         delays2[0][0]=tempd[0][0];
         delays2[0][1]=tempd[0][1];
      }

      for (fa=1;fa<8;++fa)
      {
         if ((tempd[fa][0]==tempd[fa-1][0]) & (!tempd[fa][0]==0)) 
         {
            if (actmat) delays1[afa][1]=delays1[afa][1] & tempd[fa][1];
            else delays2[afa][1]=delays2[afa][1] & tempd[fa][1];
         }
         else if (!tempd[fa][0]==0)
         {
            ++afa;
            if (actmat)
            {
               delays1[afa][0]=tempd[fa][0];
               delays1[afa][1]=tempd[fa][1];}
            else       
            {
               delays2[afa][0]=tempd[fa][0];
               delays2[afa][1]=tempd[fa][1];
            }
            ++numluzes;
         }
      }
   }
   if (actmat) delays1[8][0]=numluzes;
   else        delays2[8][0]=numluzes;
  //TESTE****************************************
  /*
  for (fa=0;fa<9;++fa)
   {
   if (actmat) printf("%U: [0]=%U [1]=%U\n\r",fa,delays1[fa][0],delays1[fa][1]);
   else        printf("%U: [0]=%U [1]=%U\n\r",fa,delays2[fa][0],delays2[fa][1]);   
   }*/
organizado=1;
clock=clockorg;
}
