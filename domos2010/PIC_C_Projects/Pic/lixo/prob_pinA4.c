#include "C:\Projecto_Domotica\Pic\lixo\prob_pinA4.h"


void main()
{

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(FALSE);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);

   // TODO: USER CODE!!
   while (true)
   {
    output_high(pin_a4);
    delay_ms(1000);
    output_low(pin_a4);
    delay_ms(1000);
   }

}
