#include <avr/io.h>
#define DDR_SPI DDRB
#define DD_SS   DDB2
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK  DDB5

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(unsigned char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1<<SPIF)))
        ;
}
unsigned char SPI_MasterReceive(void)
{
    /* Start transmission */
    SPDR = 0x00;
    /* Wait for transmission complete */
    while (!(SPSR & (1<<SPIF)))
        ;
    // Now the received data is in the SPDR
    return SPDR;
}
void SPI_SlaveInit(void)
{
    /* Set MISO output, all others input */
    DDR_SPI = (1<<DD_MISO);
    /* Enable SPI */
    SPCR = (1<<SPE);
}
unsigned char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while (!(SPSR & (1<<SPIF)))
        ;
    /* Return data register */
    return SPDR;
}
