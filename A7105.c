#include "A7105.h"
#include    "spi.h"
/***********************************************
函 数:	A7105_InterFace()
功 能:	MCU与A7105的接口配置
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_InterFace(void)
{
        iSPI_Scs    = positive;
        iSPI_Sck    = negative;
        iSPI_Sdo    = negative;

        iRxen       = positive;
        iTxen       = negative;

        direction_Scs    = direction_output;
        direction_Sck    = direction_output;
        direction_Sdio   = direction_output;
        direction_Rxen   = direction_output;
        direction_Txen   = direction_output;

        direction_GPIO1_WTR  = direction_input;
}
/***********************************************
函 数:	Register_Write()
功 能:	写bit数据到sck和sdio
输 入:	byte
输 出:	/
描 述:	左移位,bit7在先,sck负边缘触发
***********************************************/
void    Register_Write(unsigned char byte,unsigned char bits)
{
        unsigned char i;

        for(i = 0x00; i < bits; i++)
        {
            if(byte&0x80)
                iSPI_Sdi     = positive;
            else
                iSPI_Sdi     = negative;

            iSPI_Sck         = positive;
            iSPI_Sck         = negative;

            byte           <<= 0x01;
        }
}
/***********************************************
函 数:	Register_Read()
功 能:	读8bit数据从sck和sdio
输 入:	/
输 出:	byte
描 述:	左移位,bit7在先,sck负边缘触发
***********************************************/
unsigned char   Register_Read(void)
{
        unsigned char i;
        unsigned char byte;

        for(i = 0x00; i < 0x08; i++)
        {
            byte           <<= 0x01;

            if(iSPI_Sdo)
                byte        |= 0x01;
            else
                byte        &= 0xfe;

            iSPI_Sck         = positive;
            iSPI_Sck         = negative;
        }

        /* 返回读取数值 */
        return    (byte);
}
/***********************************************
函 数:	A7105_Command_Write()
功 能:	写入命令到A7105寄存器
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_Command_Write(unsigned char command)
{
        iSPI_Scs        = negative;

        Register_Write(command,cNumber_4bit);

        iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7105_Register_Write()
功 能:	写入数据到A7105寄存器
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_Register_Write(unsigned char address,unsigned char parameter)
{
        iSPI_Scs        = negative;

        /* bit7 =0:data register */
        address        &= 0x7f;

        Register_Write(address,cNumber_8bit);
        Register_Write(parameter,cNumber_8bit);

        iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7105_Register_Read()
功 能:	读出状态从A7105寄存器
输 入:	/
输 出:	/
描 述:	/
***********************************************/
unsigned char A7105_Register_Read(unsigned char address)
{
        unsigned char temp;

        iSPI_Scs        = negative;

        /* bit7 = 0:data register,bit6 = 1:read data from register */
        address        &= 0x7f;
        address        |= 0x40;

        Register_Write(address,cNumber_8bit);

        declare_sdio_input();
        temp            = Register_Read();
        declare_sdio_output();

        iSPI_Scs        = positive;

        /* 返回读取数值 */
        return    (temp);
}
/***********************************************
函 数:	A7105_Reset_Chip()
功 能:	复位A7105芯片
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_Reset_Chip(void)
{
        /* 写A7105芯片Reset */
        A7105_Register_Write(Reg_Mode,0x00);
}
/***********************************************
函 数:	RF_Setup_Channel()
功 能:	A7105设置频点通道
输 入:	/
输 出:	/
描 述:	500khz/通道，2400mhz = base frequency
***********************************************/
void    RF_Setup_Channel(unsigned char channel)
{
        A7105_Register_Write(Reg_PLL1,channel);
}
/***********************************************
函 数:	RF_Setup_StandBy()
功 能:	A7105设置为stby状态
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    RF_Setup_StandBy(void)
{
        A7105_Command_Write(cCommand_StandBy);
}
/***********************************************
函 数:	RF_Setup_receiver()
功 能:	A7105设置为接收状态
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    RF_Setup_receiver(void)
{
        RF_Setup_StandBy();
        RF_Setup_Channel(0x02);  /* 默认2401MHZ */
        A7105_Command_Write(cCommand_RX);
        iRxen       = positive;
        iTxen       = negative;
}
/***********************************************
函 数:	RF_Setup_transmiter()
功 能:	A7105设置为发射状态
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    RF_Setup_transmiter(void)
{
        iRxen       = negative;
        iTxen       = positive;
        RF_Setup_StandBy();
        RF_Setup_Channel(0x03);   /* 默认2401.5MHZ */
        A7105_Command_Write(cCommand_TX);
}
/***********************************************
函 数:	A7105_Id_Write()
功 能:	写ID到A7105内部寄存器
输 入:	/
输 出:	/
描 述:	写ID必须一次性写入
***********************************************/
void    A7105_Id_Write(void)
{
        iSPI_Scs        = negative;

        Register_Write(Reg_IdData,cNumber_8bit);

        Register_Write(cID_Code0,cNumber_8bit);
        Register_Write(cID_Code1,cNumber_8bit);
        Register_Write(cID_Code2,cNumber_8bit);
        Register_Write(cID_Code3,cNumber_8bit);

        iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7105_Fifo_Read()
功 能:	读FIFO数据
输 入:	read_buff
输 出:	/
描 述:	/
***********************************************/
void    A7105_Fifo_Read(uchar *read_buff)
{
        uchar i;

        iSPI_Scs        = negative;

        /* 读命令 */
        Register_Write((Reg_FifoData|0x40),cNumber_8bit);

        declare_sdio_input();

        /* 读FIFO数据 */
        for(i = 0x00;i < cFIFOlength;i++)
        {
            *read_buff  = Register_Read();

            read_buff ++;
        }

        declare_sdio_output();

        iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7105_Fifo_Write()
功 能:	写FIFO数据
输 入:	write_buff
输 出:	/
描 述:	/
***********************************************/
void    A7105_Fifo_Write(uchar *write_buff)
{
        uchar i;

        iSPI_Scs        = negative;

        /* 写命令 */
        Register_Write(Reg_FifoData,cNumber_8bit);

        /* 写FIFO数据 */
        for(i = 0x00;i < cFIFOlength;i++)
        {
            Register_Write(*write_buff,cNumber_8bit);

            write_buff ++;
        }

        iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7105_Calibration()
功 能:	频率校准IF和VCO,VCO band
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_Calibration(void)
{
        unsigned char temp;

        /* standby状态下校准IF */
        RF_Setup_StandBy();
        A7105_Register_Write(Reg_Calibration,0x01);
        do
        {
            temp      = A7105_Register_Read(Reg_Calibration);
            temp      = (temp&0x01);
        }while(temp);

        temp          = A7105_Register_Read(Reg_IFcalibration1);
        temp          = (temp&0x10);

        if(temp)
        {
            /* 校准出错 */

        }

        /* anual vco current band 3,vco band 1 */
        A7105_Register_Write(Reg_VcoCurrentCal,0x13);
        A7105_Register_Write(Reg_VcoBandCal1,0x09);

        RF_Setup_StandBy();
}
/***********************************************
函 数:	A7105_Config_Chip()
功 能:	配置A7105参数寄存器
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7105_Config_Chip(void)
{
        unsigned char i;

        /* 0x00 mode register, for reset */
        /* 0x05 fifo data register */
        /* 0x06 id code register */
        /* 0x23 IF calibration II, only read */
        /* 0x32 filter test register */

        for (i=0x01; i<=0x04; i++)
            A7105_Register_Write(i,A7105_Default_Parameter[i]);

        for (i=0x07; i<=0x22; i++)
            A7105_Register_Write(i,A7105_Default_Parameter[i]);

        for (i=0x24; i<=0x31; i++)
            A7105_Register_Write(i,A7105_Default_Parameter[i]);
}
/***********************************************
函 数:	Initialisation_RF()
功 能:	上电初始化A7105
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    Initialisation_RF(void)
{
        uint i;

        A7105_InterFace();

        A7105_Reset_Chip();

        for(i = 0;i < 0xffff;i++)
        {/* A7105稳定时间延迟配置 */
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }

        A7105_Id_Write();

        A7105_Config_Chip();

        A7105_Calibration();

        RF_Setup_receiver();
}
