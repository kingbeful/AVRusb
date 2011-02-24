#ifndef A7105_H_INCLUDED
#define A7105_H_INCLUDED
// Define the address of each registers
#define    Reg_Mode             0x00
#define    Reg_ModeContril      0x01

#define    Reg_Calibration      0x02

#define    Reg_FifoRegister1    0x03
#define    Reg_FifoRegister2    0x04
#define    Reg_FifoData         0x05

#define    Reg_IdData           0x06

#define    Reg_RcOsc1           0x07
#define    Reg_RcOsc2           0x08
#define    Reg_RcOsc3           0x09

#define    Reg_CkoControl       0x0a
#define    Reg_Gpio1Control     0x0b
#define    Reg_Gpio2Control     0x0c

#define    Reg_Clock            0x0d
#define    Reg_DataRate         0x0e

#define    Reg_PLL1             0x0f
#define    Reg_PLL2             0x10
#define    Reg_PLL3             0x11
#define    Reg_PLL4             0x12
#define    Reg_PLL5             0x13

#define    Reg_TX1              0x14
#define    Reg_TX2              0x15

#define    Reg_Delay1           0x16
#define    Reg_Delay2           0x17

#define    Reg_RX               0x18
#define    Reg_RxGain1          0x19
#define    Reg_RxGain2          0x1a
#define    Reg_RxGain3          0x1b
#define    Reg_RxGain4          0x1c

#define    Reg_RSSIthreshold    0x1d

#define    Reg_AdcControl       0x1e

#define    Reg_Code1            0x1f
#define    Reg_Code2            0x20
#define    Reg_Code3            0x21

#define    Reg_IFcalibration1   0x22
#define    Reg_IFcalibration2   0x23

#define    Reg_VcoCurrentCal    0x24
#define    Reg_VcoBandCal1      0x25
#define    Reg_VcoBandCal2      0x26

#define    Reg_BatteryDetect    0x27

#define    Reg_TxTest           0x28
#define    Reg_RxDemTest1       0x29
#define    Reg_RxDemTest2       0x2a

#define    Reg_ChargePump       0x2b
#define    Reg_CrystalTest      0x2c
#define    Reg_PLLtest          0x2d

#define    Reg_VCOtest1         0x2e
#define    Reg_VCOtest2         0x2F

#define    Reg_IFDT             0x30
#define    Reg_RScale           0x31
#define    Reg_FilterTest       0x32

// Define the commands used for the A7105
#define    CMD_Sleep       0x80
#define    CMD_Idle        0x90
#define    CMD_StandBy     0xa0
#define    CMD_WPll        0xb0
#define    CMD_RX          0xc0
#define    CMD_TX          0xd0
#define    CMD_RxReset     0xe0
#define    CMD_TxReset     0xf0

// Define the ID code of the chip
//ACCC9369
#define    ID_Code0        0x12
#define    ID_Code1        0x34
#define    ID_Code2        0x56
#define    ID_Code3        0x78

// Define the FIFO length
#define FIFOlength 64

unsigned char A7105_Default_Parameter[] =
{
	0x00, /* RESET register : not use on config */

	0x02, /* MODE register: FIFO mode */

	0x00, /* CALIBRATION register */

	FIFOlength-1, /* FIFO1 register : packet length*/

	0xc0, /* FIFO2 register : FIFO pointer margin threshold 16/48bytes(TX/RX)*/

	0x00, /* FIFO register :not use on config */

	0x00, /* IDDATA register :not use on config */

	0x00, /* RCOSC1 register */
	0x00, /* RCOSC2 register */
	0x00, /* RCOSC3 register */

	0x00, /* CKO register : clkout enable,bit clock */

	0x01, /* GPIO1 register :WTR output,enable GPIO1 output */
	0x09, /* GPIO2 register :CD carrier detect,enable GPIO2 output */

	0x05, /* CLOCK register :
	                           clock generation disable,crystal oscillator(16MHZ) enable
	                           system clock    = master clock divider by 2,master clock = crystal*2(DBL = 1)
	                           system clock    = 16Mhz
	                           */

	0x01, /* DATARATE register :
	                            rate = system clock/(32*SDR[7:0]+1),SDR = 0000 0001;
	                            rate = 250kbps
	                            */
	0x04, /* PLL1 register: channel = 0x04,2400MHZ + 500k * channel = 2402MHZ */

	0x9E, /* PLL2 register :
	                         DBL =1,double select crystal
	                         RRC[1:0] = 00,Fpfd = 32MHZ
	                         channel step frequency = 500khz,CHR = 15
	                         */
	0x4B, /* PLL3 register :
	                         BIP = 75
	                         */

	0x00, /* PLL4 register : BFP[15:8] = 00 */
	0x00, /* PLL5 register :
	                         BFP[7:0]  = 00
	                         base frequency = Fpfd*(BIP +BFP/2^16) = 2400MHZ
	                         */

	0x16, /* TX1 register : modulation enable, frequency deviation power setting [110] */

	0x23, /* TX2 register :
	                        Fpfd = 32MHZ,PDV[1:0] = 01,SDR[7:0] = 0000 0001

	                        Tx rate = Fpfd / (32 * (PDV[1:0]+1) * (SDR[7:0]+1)) = 250kbps

	                        TX frequency deviation = Fpfd * 127 * 2^FDP[2:0] * (FD[4:0]+1) /2^24 = 62KHZ
	                        */

	0x14, /* DELAY1 register :  TDL = 10,PDL = 100, delay = 60us + 80us = 140us */
	0xF8, /* DELAY2 register :  crystal turn on delay 2.5ms,AGC delay 40us,RSSI measurement delay 10us */

	0x26, /* RX register : AFC manual,demodulator gain =x3,BPF =500khz, up side band */

	0x80, /* RXGAIN1 register : VGA manual,PGA gain 12db,Mixer gain 24db,LNA gain 24db */
	0x80, /* RXGAIN2 register : VGA calibrate upper limit 0x80 */
	0x00, /* RXGAIN3 register : VGA calibrate lower limit 0x80 */
	0x0E, /* RXGAIN4 register : VGC calibrate continues until ID code is received,Mixer current 0.6mA,LNA current 2mA */

	0x96, /* RSSI register : RSSI 255 */

    0xC3, /* ADC register */

    0x5f, /* CODE1 register : Manchest enable,FEC enable,CRC enable,ID length 4 bytes,Preamble length 4 bytes */
    0x12, /* CODE2 register : DC value hold at 16 bit after preamble detect,ID code 0 bit error */
    0x00, /* CODE3 register : data whitening setting */

    0x00, /* IFCAL1 register : calibration */
    0x00, /* IFCAL2 register : only read */

    0x00, /* VCOCCAL register : VCO current */
    0x00, /* VCOCAL1 register : VCO band */
	0x3A, /* VCOCAL2 register : REGA = 1.1V,VCO tuning voltage  =0.3V */

	0x00, /* BATTERY register : default */

	0x17, /* TXTEST register : default */

	0x47, /* RXDEM1 register : default */
	0x80, /* RXDEM2 register : default */

	0x01, /* CPC register : charge pump current 1.0mA */

	0x05, /* CRYSTAL register : default */

	0x45, /* PLLTEST register : default */

	0x18, /* VCOTEST1 register : default */
	0x00, /* VCOTEST2 register : default */

	0x01, /* IFAT register : default */
	0x0F, /* RSCALE register : default */
	0x00  /* FILTERTEST : default */
};


/* ÌøÆµ²éÑ¯±žÓÃ */
//__flash unsigned char RX_Channel_Table[] =
unsigned char RX_Channel_Table[] =
{
        0x02, /* 2401MHZ */
        0x0c, /* 2406MHZ */
        0x16, /* 2411MHZ */

        0x20, /* 2416MHZ */
        0x2a, /* 2421MHZ */
        0x34, /* 2426MHZ */

        0x3e, /* 2431MHZ */
        0x48, /* 2436MHZ */
        0x52, /* 2441MHZ */

        0x5c, /* 2446MHZ */
        0x66, /* 2451MHZ */
        0x70, /* 2456MHZ */

        0x7a, /* 2461MHZ */
        0x84, /* 2466MHZ */
        0x8e, /* 2471MHZ */

        0x98  /* 2476MHZ */
};

/* ÌøÆµ²éÑ¯±žÓÃ */
//__flash unsigned char TX_Channel_Table[] =
unsigned char TX_Channel_Table[] =
{
        0x03, /* 2401.5MHZ */
        0x0d, /* 2406.5MHZ */
        0x17, /* 2411.5MHZ */

        0x21, /* 2416.5MHZ */
        0x2b, /* 2421.5MHZ */
        0x35, /* 2426.5MHZ */

        0x3f, /* 2431.5MHZ */
        0x49, /* 2436.5MHZ */
        0x53, /* 2441.5MHZ */

        0x5d, /* 2446.5MHZ */
        0x67, /* 2451.5MHZ */
        0x71, /* 2456.5MHZ */

        0x7b, /* 2461.5MHZ */
        0x85, /* 2466.5MHZ */
        0x8f, /* 2471.5MHZ */

        0x99  /* 2476.5MHZ */
};


#endif // A7105_H_INCLUDED
