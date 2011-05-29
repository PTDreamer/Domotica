#define N_LUZES 16 //numero maximo de dimmers
int used_dimmers=16;
#use fast_io(c)
#use fast_io(d)
int1 organizado;//flag que diz se os dimmings estao organizados
int1 actmat;//matriz em uso
int16 pointer;//apontador para matriz em uso
int vez;//indice da matriz de dimming corrente
int mnumluzes;//quantos valores de dimming diferentes temos
int ltlevel[N_LUZES]={10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
int16 delays1[N_LUZES+1][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15};
int16 delays2[N_LUZES+1][2]={0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,12,0,13,0,14,0,15};
const int16 lights[N_LUZES]={0b1111111011111111,0b1111110111111111,0b1111101111111111,0b1111011111111111,0b1110111111111111,0b1101111111111111,0b1011111111111111,0b0111111111111111,
         /*<........................................................................portC..................................................................................>*/
0b1111111111111110,0b1111111111111101,0b1111111111111011,0b1111111111110111,0b1111111111101111,0b1111111111011111,0b1111111110111111,0b1111111101111111};
/*<.................................................................................portD.............................................................>*/
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
     INT clockorg;
     INT16 tempd[N_LUZES][2];
     INT numluzes;
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
      for(temp=0;temp<17;++temp)
   {
      printf("%lu <-> %lu\n\r",tempd[temp][0],tempd[temp][1]);
   }
     IF(tempd[0][0] == 0)numluzes = 0;//luzes todas off
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

              ++numluzes;
           }
        }
     }

     IF(actmat)delays1[N_LUZES][0] = numluzes;
     ELSE  delays2[N_LUZES][0] = numluzes;
     printf("numluzes=%d %lu %lu\n\r",numluzes,delays1[0][0],delays1[0][1]);
     
     organizado=1;
  }


