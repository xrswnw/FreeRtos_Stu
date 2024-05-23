#include "AnyID_CS1110_Reader.h"

const u8 READER_VERSION[READER_VERSION_SIZE]@0x08005000 = "CS_FREERTOS 23122002 G230200";
const u8 READER_HARD_TYPE[READER_VERSION_SIZE]@0x08005080 = "CS201S-GD-AD";


READER_RSPFRAME g_sDeviceRspFrame = {0};
u16 Reader_ProcessUartFrames(u8 *pFrame, u16 len)
{       
    u8 cmd = 0;
    u16 destAddr = 0;
    u16 paramsLen = 0;
    
	memset(&g_sDeviceRspFrame, 0, sizeof(READER_RSPFRAME));
    destAddr = *((u16 *)(pFrame + UART_FRAME_POS_DESTADDR));
    if((destAddr != READER_FRAME_BROADCAST_ADDR) && (destAddr != /*g_sDeviceParamenter.addr*/ 0x01))
    {
        return 0;
    }

    if(*(pFrame + UART_FRAME_POS_CMD) == UART_FRAME_RESPONSE_FLAG)
    {
        cmd = *(pFrame + UART_FRAME_POS_CMD + 1);
    }
    else
    {
        cmd = *(pFrame + UART_FRAME_POS_CMD);
    }

    g_sDeviceRspFrame.destAddr = *((u16 *)(pFrame + UART_FRAME_POS_SRCADDR));
    g_sDeviceRspFrame.cmd = cmd;
    g_sDeviceRspFrame.flag = UART_FRAME_FLAG_OK;
    g_sDeviceRspFrame.err = UART_FRAME_RSP_NOERR;
	
    if(pFrame[UART_FRAME_POS_LEN] == 0)
    {
        paramsLen = *((u16 *)(pFrame + UART_FRAME_POS_PAR));
    }
    else
    {
        paramsLen = len - UART_FRAME_MIN_LEN;
    }
    
    switch(cmd)
    {
        case READER_CMD_RESET:
			if(paramsLen == 0)
            {
				g_sDeviceRspFrame.len = Device_ResponseFrame(NULL, 0, &g_sDeviceRspFrame); 
			}
			break;
        case READER_CMD_GET_VERSION:
            if(paramsLen == 0)
            {
				g_sDeviceRspFrame.len = Device_ResponseFrame((u8 *)READER_VERSION, READER_VERSION_SIZE, &g_sDeviceRspFrame); 
			}
			break;
        default:
            break;
	}
		
     if(g_sDeviceRspFrame.len == 0 && (cmd != READER_CMD_CHG_IP))
    {
        g_sDeviceRspFrame.flag = READER_RSPFRAME_FLAG_FAIL;
        g_sDeviceRspFrame.err = READER_OPTAG_RESPONSE_PARERR;
        g_sDeviceRspFrame.len = Device_ResponseFrame(NULL, 0, &g_sDeviceRspFrame);
	}
    
    return g_sDeviceRspFrame.len;
}





u8 Device_ResponseFrame(u8 *pParam, u8 len, READER_RSPFRAME *pOpResult)
{
    u16 pos = 0;
    u16 crc = 0;

    pOpResult->buffer[pos++] = UART_FRAME_FLAG_HEAD1; // frame head first byte
    pOpResult->buffer[pos++] = UART_FRAME_FLAG_HEAD2; // frame haed second byte
    pOpResult->buffer[pos++] = 0x00;   // length
    pOpResult->buffer[pos++] = (/*g_sDeviceParamenter.addr*/ 0x01 >> 0) & 0xFF;
    pOpResult->buffer[pos++] = (/*g_sDeviceParamenter.addr*/ 0x01 >> 8) & 0xFF;
    pOpResult->buffer[pos++] = (pOpResult->destAddr >> 0) & 0xFF;
    pOpResult->buffer[pos++] = (pOpResult->destAddr >> 8) & 0xFF;
    pOpResult->buffer[pos++] = UART_FRAME_RESPONSE_FLAG;
    pOpResult->buffer[pos++] = pOpResult->cmd;			   // cmd

    if(len > UART_FRAME_PARAM_MAX_LEN)
    {
        pOpResult->buffer[pos++] = (len >> 0) & 0xFF;
        pOpResult->buffer[pos++] = (len >> 8) & 0xFF;
        memcpy(pOpResult->buffer + pos, pParam, len);
        pos += len;
    }
    else
    {
		pOpResult->buffer[pos++] = UART_FRAME_PARAM_RFU;
        if(pOpResult->flag == READER_RSPFRAME_FLAG_OK)
        {
			memcpy(pOpResult->buffer + pos, pParam, len);
        }
        else
        {
			memset(pOpResult->buffer + pos, 0, len);
        }
        pos += len;
        pOpResult->buffer[pos++] = pOpResult->flag;
        pOpResult->buffer[pos++] = pOpResult->err;
        pOpResult->buffer[UART_FRAME_POS_LEN] = pos - 3 + 2; //减去帧头7E 55 LEN 的三个字节，加上CRC的两个字节
    }

    crc = a_GetCrc(pOpResult->buffer + UART_FRAME_POS_LEN, pos - UART_FRAME_POS_LEN); //从LEN开始计算crc
    pOpResult->buffer[pos++] = crc & 0xFF;
    pOpResult->buffer[pos++] = (crc >> 8) & 0xFF;

    pOpResult->len = pos;

    return pos;
}