#ifndef _ANYID_CS1110_WIGHT_H
#define _ANYID_CS1110_WIGHT_H

#include "AnyID_CS1110_Wight_HL.h"
#include "AnyID_Uart_Receive.h"

#define GPB_REG_ADDR_WGT		0x0001
#define GPB_REG_ADDR_ZERO		0x0002
#define GPB_REG_ADDR_SPEED		0x0009
#define GPB_REG_ADDR_RTO		0x000B
#define GPB_REG_ADDR_SET_ADDR		0x000F


#define GPB_STAT_IDLE 	            0x00000000
#define GPB_STAT_RCV 		        0x00000001
#define GPB_STAT_OVR 		        0x00000004
#define GPB_STAT_TX 		        0x00000008
#define GPB_STAT_TX_IDLE            0x00000010
#define GPB_STAT_WAIT               0x00000020
#define GPB_STAT_STEP               0x00000040
#define GPB_STAT_OP_DELAY			0x00000080

#define GPB_FLAG_IDLE 	                0x00
#define GPB_FLAG_CMD 		        0x01
#define GPB_FLAG_REG 		        0x04
#define GPB_FLAG_OVR		        0x08

#define GPB_MODBUS_READ_REG             0x03
#define GPB_MODBUS_SET_REG              0x10

#define GPB_BUF_MAX_LEN	                0x20
#define GPB_BUF_MIN_LEN	                0x08

#define GPB_WIGHT_VALUE_STABLE  	0x01
#define GPB_WIGHT_VALUE_FAIL      	0x00
#define GPBWIGHT_VALUE_NUM		3


#define GPB_SHANK_WIGHT_VALUE			2
#define GPB_SHANK_WIGHT_TICK			5
#define GPB_WITGH_MASK_VALUE            0x10000000
#define GPB_WITGH_MASK_VALUE_POINT		0x00F00000
#define GPB_WITGH_MASK_VALUE_ABS		0x000FFFFF
#define GPB_WITGH_MASK_ALL_VALUE        0x100FFFFF
#define GPB_WITGH_MASK_DOWN_VALUE       0x80000000

#define GPB_WITGH_LOW_MASK_VALUE        10
#define GPB_WITGH_FLAG_PLUS             0
#define GPB_WITGH_FLAG_MINUS            0x10000000
#define GPB_WIGHT_VALUE_POS             0x03
#define GPB_BUFFER_MAX_LEN             (256 + 32) 
#define GPB_READ_WITHT_VALUE_LEN		9

#define GPB_WIGHTOVER_LINIT_TICK		5

#define GPB_WOEK_NORMAL                 0x00
#define GPB_WORK_SET_ZERO               0x01

#define GPB_STAT_OPEN_SAMPLE            0x01
#define GPB_STAT_STOP_SAMPLE            0x02

#define GPB_FLAG_NORMAL                 0
#define GPB_FLAG_FAIL                   0x01
#define GPB_SAMPLE_NUM                  8

#define GPB_WITGH_MAX_VAR_DEFF			10000
typedef struct wightInfo{
	u8 tick;
    u32 flag;
    int sum;
    int avg;
    u32 index;
    int buffer[GPB_SAMPLE_NUM];
    u32 alarmTick;
}WIGHT_INFO;


extern WIGHT_INFO g_sWightTempInfo;

#define GPB_STAT_SAMPLE_NUM             2
#define Gpb_IsRcvFrame(rcvFrame)               ((rcvFrame).state == GPB_STAT_RCV || (rcvFrame).state == GPB_STAT_OVR)
#define Gpb_ResetFrame(rcvFrame)               do{rcvFrame.repeat=0;rcvFrame.errTick = 0;}while(0)

#define Gpb_GetValue(p)                 ({\
                                             u32 value;\
                                             value = (*(p + GPB_WIGHT_VALUE_POS + 0) << 24) | (*(p + GPB_WIGHT_VALUE_POS + 1) << 16) | (*(p + GPB_WIGHT_VALUE_POS + 2) << 8) | (*(p + GPB_WIGHT_VALUE_POS + 3) << 0);\
                                             (value);\
                                         })\
	
#define Gpb_ChkValue(u, v)     					 ({(v - u);})
											 
#define Gpb_ChkFrame(rcvBuf, cmd, lenth)					((rcvBuf)->len == lenth && (rcvBuf)->buffer[1] == cmd)

typedef struct wihgtTxBuf{
    u8 cmd;
    u16 regAdd;
    u16 regNum;
    u16 data0;
    u16 data1;
    u32 len;
    u8 buffer[GPB_BUF_MAX_LEN];
    u32 crc16;
}GPB_TX_BUF;

typedef struct wihgtRxBuf{
    u32 len;
    u8 buffer[GPB_BUF_MAX_LEN];
}GPB_RX_BUF;

#define GPB_OP_OT_TIME			10
#define GPB_OP_DELAY_TIME		10
#define GPB_OP_COM_ERR_TICK		10

typedef struct wihgtInfo{
	BOOL stableFlag;
    u8 linitTick;
    u8 flag;
    u8 repeat;
    u8 index;
    u8 num;
    u8 mode;
	u8 shankTick;
	u8 errTick;
    u32 state;
    int wightSampleValue;
    int wightRealValue;
    int witghStableValue;
    u32 tick; 
    GPB_TX_BUF txBuf;
    UART_RCVFRAME rxBuf;
}GPB_INFO;

#define Gpb_OpDelay(t)		do{g_sGpbInfo.state = GPB_STAT_OP_DELAY; g_sGpbInfo.tick = t;}while(0)
#define Gpb_ClearRcv()		do{memset(&g_sGpbInfo.rxBuf,0, sizeof(UART_RCVFRAME));}while(0)

//-----------

#define WIGHT_STAT_IDLE                  0x00
#define WIGHT_STAT_DATA                  0x01
#define WIGHT_STAT_TO                    0x02
#define WIGHT_STAT_END                   0x03
#define WIGHT_STAT_RCV                   0x04 	


#define WIGHT_BUFFER_MAX_LEN			256
typedef struct {
	u8 cmd;
	u8 state;
	u16 readRegAddr;
	u16	readRegNum;
	u16 writeRegAddr;
	u16	writeRegNum;
	u8 txLen;
	u8 rxLen;
	u8 txBuffer[WIGHT_BUFFER_MAX_LEN];
	u8 rxBuffer[WIGHT_BUFFER_MAX_LEN];
}WITHT_FRAME;


extern WITHT_FRAME	g_sWightFrame;





void Wight_Init();


//---------

extern GPB_INFO g_sGpbInfo ;
extern UART_RCVFRAME g_sGpbTempRcv;

void GPB_Init();
void GPB_TransmitCmd(GPB_TX_BUF *pBuffer);
void GPB_ChgValue(u32 value, char *buf);
void GPB_Adjust(GPB_TX_BUF *pBuffer);
void GPB_RtoChg(GPB_TX_BUF *pBuffer);
void GPB_Filt_Chg();
void Gpb_TxFrame(u32 tick);
void Gpb_OpStep();
BOOL GPB_CheckFrame(UART_RCVFRAME *pRcvFrame);

u8 GPB_GetValue(GPB_INFO *pBuffer);
u8 GPB_FormatFrame(u8 cmd,u16 regAdd,u16 regNum,u8 *pFrame);
u16 GPB_GetCrc16(u8 *pBuffer, u8 len);


void Witgh_CalAvg(WIGHT_INFO *pInfo, u32 value);

#endif