#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

void SPI_MasterInit(void);
void SPI_MasterTransmit(unsigned char cData);
unsigned char SPI_MasterReceive(void);
void SPI_SlaveInit(void);
unsigned char SPI_SlaveReceive(void);


#endif // SPI_H_INCLUDED
