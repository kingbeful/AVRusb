//The Max Frequency of SPI for A7105 is 10 MHz
#include <avr/io.h>
#include "spi.h"
#include "A7105.h"
#define SPI_SS_0 PORTB &= 0xfb
#define SPI_SS_1 PORTB |= 0x04

void RF_StorbeCommand (unsigned char cmd)
{
    SPI_SS_0;
    SPI_MasterTransmit(cmd);
    SPI_SS_1;
}
void RF_WriteRegister (unsigned char addr, unsigned char data)
{
    // bit7 = 0 --> data register
    // bit6 = 0 --> write to data register
    addr &= 0x3f;

    SPI_SS_0;

    SPI_MasterTransmit(addr);
    SPI_MasterTransmit(data);

    SPI_SS_1;
}
unsigned char RF_ReadRegister (unsigned char addr)
{
    unsigned char data;
    // bit7 =0 --> data register
    // bit6 =1 --> read data register
    addr &= 0x7f;
    addr |= 0x40;

    SPI_SS_0;

    SPI_MasterTransmit(addr);
    data = SPI_MasterReceive();

    SPI_SS_1;
    return data;
}
void A7105_Id_Write(void)
{
    SPI_SS_0;

    SPI_MasterTransmit(Reg_IdData);

    SPI_MasterTransmit(ID_Code0);
    SPI_MasterTransmit(ID_Code1);
    SPI_MasterTransmit(ID_Code2);
    SPI_MasterTransmit(ID_Code3);

    SPI_SS_1;
}
void A7105_Config_Chip(void)
{
    unsigned char i;

    // Addr 0x00 is mode register, for reset
    // Addr 0x05 is fifo data register
    // Addr 0x06 is id code register
    // Addr 0x23 is IF calibration II, only read
    // Addr 0x32 is filter test register
    // Above Addrs will NOT be set

    SPI_SS_0;

    for (i=0x01; i<=0x04; i++)
        RF_WriteRegister(i,A7105_Default_Parameter[i]);

    for (i=0x07; i<=0x22; i++)
        RF_WriteRegister(i,A7105_Default_Parameter[i]);

    for (i=0x24; i<=0x31; i++)
        RF_WriteRegister(i,A7105_Default_Parameter[i]);

    SPI_SS_1;
}
void A7105_Calibration(void)
{
    unsigned char temp;

    // To calibrat IF in standby state
    RF_StorbeCommand(CMD_StandBy);
    RF_WriteRegister(Reg_Calibration,0x01);
    do
    {
        temp = RF_ReadRegister(Reg_Calibration);
        temp &= 0x01;
    }
    while (temp);

    temp = RF_ReadRegister(Reg_IFcalibration1);
    temp = (temp&0x10);

    if (temp)
    {
        // Calibration Error
    }

    // manual vco current 3, vco band 1
    RF_WriteRegister(Reg_VcoCurrentCal,0x13);
    RF_WriteRegister(Reg_VcoBandCal1,0x09);

    RF_StorbeCommand(CMD_StandBy);
}

void A7105_Fifo_Read(unsigned char *read_buff)
{
        unsigned char i;

        SPI_SS_0;

        // bit6 = 1 --> read data from fifo
        SPI_MasterTransmit((Reg_FifoData|0x40));

        //read FIFO data
        for(i = 0x00;i < FIFOlength;i++)
        {
            *read_buff  = SPI_MasterReceive();
            read_buff ++;
        }

        SPI_SS_1;
}

void A7105_Fifo_Write(unsigned char *write_buff)
{
        unsigned char i;

        SPI_SS_0;

        // bit6 = 0 --> write data to fifo
        SPI_MasterTransmit((Reg_FifoData&0x3f));

        // write FIFO data
        for(i = 0x00;i < FIFOlength;i++)
        {
            SPI_MasterTransmit(*write_buff);
            write_buff ++;
        }

        SPI_SS_1;
}
void RF_init (void)
{
    SPI_MasterInit();
    // Reset the RF chip
    RF_WriteRegister(Reg_Mode,0x00);
    A7105_Id_Write();

    A7105_Config_Chip();

    A7105_Calibration();
}
