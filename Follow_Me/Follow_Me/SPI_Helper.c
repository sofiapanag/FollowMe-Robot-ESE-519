/*
 * SPI.c
 *
 * Created: 12/9/2021 4:56:21 PM
 *  Author: Dennis
 */ 

#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BLE.h"
#include "Adafruit_FIFO.h"


float parsefloat(uint8_t *buffer)
{
	float f;
	memcpy(&f, buffer, 4);
	return f;
}



uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout)
{
	uint16_t origtimeout = timeout, replyidx = 0;

	memset(packetbuffer, 0, READ_BUFSIZE);

	while (timeout--) {
		if (replyidx >= 20) break;
		if ((packetbuffer[1] == 'A') && (replyidx == PACKET_ACC_LEN))
		break;
		if ((packetbuffer[1] == 'G') && (replyidx == PACKET_GYRO_LEN))
		break;
		if ((packetbuffer[1] == 'M') && (replyidx == PACKET_MAG_LEN))
		break;
		if ((packetbuffer[1] == 'Q') && (replyidx == PACKET_QUAT_LEN))
		break;
		if ((packetbuffer[1] == 'L') && (replyidx == PACKET_LOCATION_LEN))
		break;

		while (ble->available()) {
			char c =  ble->read();
			if (c == '!') {
				replyidx = 0;
			}
			packetbuffer[replyidx] = c;
			replyidx++;
			timeout = origtimeout;
		}
		
		if (timeout == 0) break;
		_delay_ms(100);
	}

	packetbuffer[replyidx] = 0;  // null term

	if (!replyidx)  // no data or timeout
	return 0;
	if (packetbuffer[0] != '!')  // doesn't start with '!' packet beginning
	return 0;
	
	// check checksum!
	uint8_t xsum = 0;
	uint8_t checksum = packetbuffer[replyidx-1];
	
	for (uint8_t i=0; i<replyidx-1; i++) {
		xsum += packetbuffer[i];
	}
	xsum = ~xsum;

	// Throw an error message if the checksum's don't match
	if (xsum != checksum)
	{
		char output[100] = {'\0'};
		sprintf(output, "Checksum mismatch in packet\n");
		putstring_UART(output);
		// printHex(packetbuffer, replyidx+1);
		return 0;
	}
	
	// checksum passed!
	return replyidx;
}
