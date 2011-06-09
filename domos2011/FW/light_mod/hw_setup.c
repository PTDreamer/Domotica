void hw_setup()
{
   ////CAN_BUS SETUP
   can_init () ;
   can_set_mode (CAN_OP_CONFIG) ;  
   BRGCON1.brp = 4;
   BRGCON1.sjw = 0;
   BRGCON2.prseg = 2;
   BRGCON2.seg1ph = 5;
   BRGCON2.sam = FALSE;
   BRGCON2.seg2phts = FALSE;
   BRGCON3.seg2ph = 5;
   BRGCON3.wakfil = TRUE;
   can_set_mode (CAN_OP_NORMAL) ;
      
   ////TIMERS SETUP
   setup_timer_0 (RTCC_INTERNAL | RTCC_DIV_4); //div16 1.6us thick 104ms overflow
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
   setup_timer_2(T2_DIV_BY_16,241,13);
   setup_ccp1(CCP_COMPARE_INT);
   ////OTHERS
   ext_int_edge(l_to_h);
   
   
   /////DISABLED HW////////
   setup_adc_ports (NO_ANALOGS | VSS_VDD) ;
   setup_adc (ADC_CLOCK_DIV_2 | ADC_TAD_MUL_0) ;
   setup_psp (PSP_DISABLED) ;
   setup_spi (SPI_SS_DISABLED) ;
   setup_comparator (NC_NC_NC_NC) ;
   setup_vref (FALSE) ;
   setup_timer_3 (T3_DISABLED | T3_DIV_BY_1) ;
   //setup_wdt (WDT_ON) ;
   }
