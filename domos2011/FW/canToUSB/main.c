#include "C:\Users\Xapo\Documents\Electronica\domos2011\FW\canToUSB\main.h"




void main()
{

   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2|ADC_TAD_MUL_0);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_OFF);
   setup_vref(FALSE);

   //TODO: User Code
   while(true)
   {
      output_toggle(PIN_A0);
	  delay_ms(100);
   }

}
