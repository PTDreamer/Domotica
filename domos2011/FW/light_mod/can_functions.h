void canReceive ( );
int1 can_Pop ( int32 & rx_id, int * buffer, int & rx_len, int & rx_stat );
void canTransmit ( );
int1 can_push (int32 tx_id, int8 * tx_buffer, int8 tx_length, int8 tx_priority, int1 tx_extendedID, int1 tx_emptyframe);
