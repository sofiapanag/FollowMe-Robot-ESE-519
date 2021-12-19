/*
 * SPI.h
 *
 * Created: 12/9/2021 4:56:40 PM
 *  Author: Dennis
 */ 


#ifndef SPI_H_
#define SPI_H_

#define READ_BUFSIZE                    (20)
#define PACKET_ACC_LEN                  (15)
#define PACKET_GYRO_LEN                 (15)
#define PACKET_MAG_LEN                  (15)
#define PACKET_QUAT_LEN                 (19)
#define PACKET_LOCATION_LEN             (15)
#define BLE_READPACKET_TIMEOUT         500   // Timeout in ms waiting to read a response

uint8_t packetbuffer[READ_BUFSIZE+1];

#define BLUEFRUIT_SPI_CS 10
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4

#define BLUEFRUIT_SPI_SCK              13
#define BLUEFRUIT_SPI_MISO             12
#define BLUEFRUIT_SPI_MOSI             11

#endif /* SPI_H_ */