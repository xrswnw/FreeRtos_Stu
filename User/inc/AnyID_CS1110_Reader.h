#ifndef _ANYID_CS1110_READER_H
#define _ANYID_CS1110_READER_H

#include "Uart2.h"
#include "Anyid_Systick.h"
#include "AnyID_CS1110_Wight.h"
#define READER_VERSION_SIZE				50
#define READER_RSPFRAME_DAT_LEN			(2560)
#define READER_RSPFRAME_LEN				(READER_RSPFRAME_DAT_LEN + 32)


//errInfo
#define READER_RSPFRAME_FLAG_OK          0x00
#define READER_RSPFRAME_FLAG_FAIL        0x01
#define READER_OPTAG_RESPONSE_NOERR      0x00
#define READER_OPTAG_RESPONSE_TAGERR     0x01
#define READER_OPTAG_RESPONSE_CRCERR     0x02
#define READER_OPTAG_RESPONSE_NORSP      0x03
#define READER_OPTAG_RESPONSE_PARERR     0x04

#define READER_FRAME_BROADCAST_ADDR         0xFFFF

//cmd
#define READER_CMD_RESET				0x04
#define READER_CMD_GET_VERSION			0xF7
#define READER_CMD_CHG_IP				0x55

typedef struct readerRspFrame{
    u16 len;
    u8 buffer[READER_RSPFRAME_LEN];
    u8 flag;
    u8 err;
    u8 cmd;
    u16 destAddr;
    u32 tick;
}READER_RSPFRAME;

extern READER_RSPFRAME g_sDeviceRspFrame;









u16 Reader_ProcessUartFrames(u8 *pFrame, u16 len);
u8 Device_ResponseFrame(u8 *pParam, u8 len, READER_RSPFRAME *pOpResult);
#endif



