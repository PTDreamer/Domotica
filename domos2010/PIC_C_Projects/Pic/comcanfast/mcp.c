#define cs pin_c2

void f2510wr(int end,int dado);
int f2510rd(int end);
void f2510cfg(void);
int1 f2510tx(int *pilha);
void f2510bs(int end,int bbit);
void f2510cfg_end(int end,int1 intr);
void f2510rst(void);
int f2510rx(int *bu);
void f2510bc(int end,int bbit);



void f2510wr(int end,int dado)
{
   output_low(cs);
   spi_write(d2510wrt);
   spi_write(end);
   spi_write(dado);
   output_high(cs);
}

int f2510rd(int end)
{
   int aux;
   output_low(cs);
   spi_write(d2510rd);
   spi_write(end);
   aux=spi_read(0);
   output_high(cs);
   return aux;
}

//1MHz com cristal de 20 MHz
void f2510cfg(void)
{
    f2510wr(canctrl,0x80);
   //f2510wr(0x25,0x30);
   f2510wr(cnf1,0x00);//BRP=0
   f2510wr(cnf2,0x11);//PHSEG1=3(2) PRSEG=2(1)
   f2510wr(cnf3,0x03);//PHSEG2=4(3)
   f2510wr(canctrl,0x00);
   //f2510wr(canctrl,0x40);
}


int1 f2510tx(int *pilha)

{  int   buffer1,buffer,aux1_length,aux2;

   if      (!bit_test(f2510rd(0x30),3)) buffer1=0x30;
   else if (!bit_test(f2510rd(0x40),3)) buffer1=0x40;
   else if (!bit_test(f2510rd(0x50),3)) buffer1=0x50;
   else return false;

   buffer=buffer1;
   f2510wr(buffer+=1,pilha[1]);
   f2510wr(buffer+=1,0x00);
   f2510wr(buffer+=3,pilha[6]+4);// | length(data)|
   f2510wr(buffer+=1,pilha[2]); // | sub_dest_adr|
   f2510wr(buffer+=1,pilha[3]);// | source_adr  |
   f2510wr(buffer+=1,pilha[4]); // |sub_source_adr|
   f2510wr(buffer+=1,pilha[5]);// | type        |

   aux1_length=pilha[6];

   for (aux2=7;aux2<(aux1_length+7);++aux2)
   {
   f2510wr(buffer+=1,pilha[aux2]);
   }
   pilha[0]=0;
   f2510bs(buffer1,3);

   return true;
}


void f2510bs(int end,int bbit)
{
   int aux;
   output_low(cs);
   bit_set(aux,bbit);
   spi_write(d2510bitmod);
   spi_write(end);
   spi_write(aux);
   spi_write(0xff);
   output_high(cs);
}

void f2510bc(int end,int bbit)
{
   int aux;
   output_low(cs);
   bit_set(aux,bbit);
   spi_write(d2510bitmod);
   spi_write(end);
   spi_write(aux);
   spi_write(0x00);
   output_high(cs);
}

void f2510rst(void)
{

   output_low(cs);
   spi_write(0xc0);
   output_high(cs);
}



void f2510cfg_end(int end,int1 intr)//endereco, intr=0 sem interrupt em rx0bf
{
   f2510wr(canctrl,0x80);
   f2510bs(0x0c,0);
   if(intr) f2510bs(0x0c,1);
   f2510wr(0x00,end);   // acceptance filter rxf0
   f2510wr(0x04,0x69);   // acceptance filter rxf1
   f2510wr(0x08,0xff);  // acceptance filter rxf2
   f2510wr(0x10,0xff);  // acceptance filter rxf3
   f2510wr(0x14,0xff);  // acceptance filter rxf4
   f2510wr(0x18,0xff);  // acceptance filter rxf5

   f2510wr(0x01,0);
   f2510wr(0x05,0);
   f2510wr(0x09,0xf0);//???
   f2510wr(0x11,0xf0);
   f2510wr(0x15,0xf0);
   f2510wr(0x19,0xf0);

   f2510wr(0x20,0xff);
   f2510wr(0x21,0xff);
   f2510wr(0x24,0xff);
   f2510wr(0x25,0xff);
   f2510wr(canctrl,0x00);
}


   int f2510rx(int *bu) //recebe dados do mcp 2510 e coloca-os no buffer devolve 0
   {int auxrx;          // se o buffer estiver cheio (bu[0]=1) nao recebe e devolve 1
      if (!bit_test(f2510rd(0x2c),0)) return 2;
      if (bu[0]==0)     // se nao a mensagem nova devolve 2
    {
      bu[1]=f2510rd(0x61);//dest_adr
      bu[6]=f2510rd(0x65)-4;//lenght
      bu[2]=f2510rd(0x66);//sub_dest_adr
      bu[3]=f2510rd(0x67);//source_adr
      bu[4]=f2510rd(0x68);//sub_source_adr
      bu[5]=f2510rd(0x69);//type

      for (auxrx=0;auxrx<bu[6];++auxrx)
      {
         bu[7+auxrx]=f2510rd(0x6A+auxrx);
      }

      f2510bc(0x2c,0);
      bu[0]=1;
      return 0;
    }
    return 1;
   }





















