#include "C:\Users\JB\Documents\Electronica\PIC_C_Projects\main.h"


#define CAN_USE_EXTENDED_ID FALSE
#include <can-18xxx8.c>

#define CAN_RECEIVE_STACK_SIZE 3
int can_rspoint=0;
int can_rstack [CAN_RECEIVE_STACK_SIZE] [14];
int1 can_rsfull=FALSE;
#define can_receiver_full() can_rsfull
#define CAN_TRANSMIT_STACK_SIZE 3
int can_tspoint=0;
int can_tstack [CAN_TRANSMIT_STACK_SIZE] [13];
int1 can_tsempty=TRUE;
#define can_transmitter_empty() can_tsempty
#define CAN_FILTER_SIZE 0
int32 can_filter [CAN_FILTER_SIZE] = {};


void canReceive ( )
{
	int32 rx_id;
	int8  rx_len, rx_stat;
	int8  buffer [8];
	int8  i;

	if(can_rspoint==-1)
		can_rspoint++;

	if(can_rspoint < CAN_RECEIVE_STACK_SIZE)
	{
		can_getd(rx_id,buffer,rx_len,rx_stat);
	{
		int8 j;
		for(j=0;j<CAN_FILTER_SIZE;j++)
			if(rx_id==can_filter[j])
				return;
	}
		can_rstack[can_rspoint][0]=make8(rx_id,3);
		can_rstack[can_rspoint][1]=make8(rx_id,2);
		can_rstack[can_rspoint][2]=make8(rx_id,1);
		can_rstack[can_rspoint][3]=make8(rx_id,0);

		can_rstack[can_rspoint][4]=rx_len;
		can_rstack[can_rspoint][5]=rx_stat;

		for(i=0;i<rx_len;i++)
		{
			can_rstack[can_rspoint][i+6]=buffer[i];
		}

		can_rspoint++;
	}
	else
		can_getd(rx_id,buffer,rx_len,rx_stat);
}


int1 can_Pop ( int32 & rx_id, int * buffer, int & rx_len, int & rx_stat )
{
	int i;

	if(can_rspoint==CAN_RECEIVE_STACK_SIZE)
		can_rspoint--;

	if(can_rspoint!=-1)
	{
		rx_id=make32(can_rstack[can_rspoint][0],
						can_rstack[can_rspoint][1],
						can_rstack[can_rspoint][2],
						can_rstack[can_rspoint][3]);

		rx_len=can_rstack[can_rspoint][4];
		rx_stat=can_rstack[can_rspoint][5];

		for(i=0;i<rx_len;i++)
		{
			buffer[i]=can_rstack[can_rspoint][i+6];
		}
		can_rspoint--;

		return TRUE;
	}	
	else
	return FALSE;
}

void canTransmit ( )
{
	int32 tx_id;
	int8  tx_length;
	int8  tx_priority;
	int8  tx_buffer[8];
	int1  tx_extendedID;
	int1  tx_emptyframe;
	int8  i;

	if(can_tspoint==CAN_TRANSMIT_STACK_SIZE)
	{
		can_tspoint--;
	}

	if(can_tspoint!=-1)
	{

		tx_id=make32(can_tstack[can_tspoint][0],
						can_tstack[can_tspoint][1],
						can_tstack[can_tspoint][2],
						can_tstack[can_tspoint][3]);

		tx_length=can_tstack[can_tspoint][12]>>4;
		tx_priority=(can_tstack[can_tspoint][12]&0x0c)>>2;
		tx_extendedID=bit_test(can_tstack[can_tspoint][12],1);
		tx_emptyframe=bit_test(can_tstack[can_tspoint][12],0);

		for(i=0;i<tx_length;i++)
		{
			tx_buffer[i]=can_tstack[can_tspoint][i+4];
		}

		can_putd(tx_id,tx_buffer,tx_length,tx_priority,tx_extendedID,tx_emptyframe);

		can_tspoint--;
	}
	else
		return;
}


int1 can_push (int32 tx_id, int8 * tx_buffer, int8 tx_length, int8 tx_priority, int1 tx_extendedID, int1 tx_emptyframe)
{
	int8  i;
	if(can_tspoint==-1)
		can_tspoint++;

	if(can_tspoint < CAN_TRANSMIT_STACK_SIZE)
	{
		can_tstack[can_tspoint][0]=make8(tx_id,3);
		can_tstack[can_tspoint][1]=make8(tx_id,2);
		can_tstack[can_tspoint][2]=make8(tx_id,1);
		can_tstack[can_tspoint][3]=make8(tx_id,0);

		for(i=0;i<tx_length;i++)
		{
			can_tstack[can_tspoint][i+4]=tx_buffer[i];
		}

		can_tstack[can_tspoint][12]=(tx_length<<4)|(tx_priority<<2)|((int8)tx_extendedID<<1)|((int8)tx_emptyframe);

		can_tspoint++;
	}
}
#int_canrx0
void canrx0_int ( ) {
	canReceive ( );
	// TODO: add CAN recieve code here
}
#int_canrx1
void canrx1_int ( ) {
	canReceive ( );
	// TODO: add CAN recieve code here
}
#int_cantx0
void cantx0_int ( ) {
	canTransmit ( );
	// TODO: add CAN transmit code here
}
#int_cantx1
void cantx1_int ( ) {
	canTransmit ( );
	// TODO: add CAN transmit code here
}
#int_cantx2
void cantx2_int ( ) {
	canTransmit ( );
	// TODO: add CAN transmit code here
}
void main()
{
	can_init();
	
	enable_interrupts(int_canrx0); 
	enable_interrupts(int_canrx1); 
	enable_interrupts(int_cantx0); 
	enable_interrupts(int_cantx1); 
	enable_interrupts(int_cantx2); 


   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_OFF|ADC_TAD_MUL_0);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
//Setup_Oscillator parameter not selected from Intr Oscillotar Config tab

   // TODO: USER CODE!!

}
