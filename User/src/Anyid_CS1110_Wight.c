#include "AnyID_CS1110_Wight.h"

GPB_INFO g_sGpbInfo ;
UART_RCVFRAME g_sGpbTempRcv = {0};
WIGHT_INFO g_sWightTempInfo = {0};


//
WITHT_FRAME	g_sWightFrame = {0};
void Wight_Init()
{	
	Wight_InitInterface(WIGHT_BAUD_RATE);
	Wight_DmaTxInit();
	Wight_DmaRxInit(g_sWightFrame.rxBuffer, WIGHT_BUFFER_MAX_LEN);
	g_sWightFrame.state |= WIGHT_STAT_TX;
	//建立信号量二进制
	/*
	g_sWightFrame.rcvBrySre = xSemaphoreCreateBinary();
	
	if(xSemaphoreCreateBinary == NULL)
	{
		//建立失败
		while(1)
		{
			////////////////
		}
	}*/
	
}


#define POLYNOMIAL                      0xA001   //x^16 + x^12 + x^5 + 1
#define PRESET_VALUE                    0xFFFF
u16 Wight_GetCrc16(u8 *pBuffer, u8 len)
{
    u16 i = 0;
    u16 crc = 0;
    u8 j = 0;

    crc = PRESET_VALUE;
    for(i = 0; i < len; i++)
    {
        crc = crc ^ pBuffer[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
            {
                crc = (crc >> 1) ^ POLYNOMIAL;
            }
            else
            {
                crc = (crc >> 1);
            }
        }
    }

    return crc;
}

u16 Wight_FormatFrame(WITHT_FRAME *pFrame)
{
	u16 pos = 0, crc16 = 0;
	
	pFrame->txBuffer[pos ++] = WIGHT_MODBUS_BROAD_ADDR;
	pFrame->txBuffer[pos ++] = pFrame->cmd;
	pFrame->txBuffer[pos ++] = (pFrame->regAddr >> 8) & 0xFF;
	pFrame->txBuffer[pos ++] = (pFrame->regAddr >> 0) & 0xFF;
	pFrame->txBuffer[pos ++] = (pFrame->regNum >> 8) & 0xFF;
	pFrame->txBuffer[pos ++] = (pFrame->regNum >> 0) & 0xFF;
	
	if(pFrame->cmd == WIGHT_MODBUS_FUN_READ_HOLDREGS)
	{
		memcpy(pFrame->txBuffer + pos, pFrame->frame, pFrame->regNum);
		pos += (pFrame->regNum * WIGHT_MODBUS_REG_BUYE_LEN);
	}
	else
	{
	
	}
	crc16 = Wight_GetCrc16(pFrame->txBuffer, pos);
	pFrame->txBuffer[pos ++] = (crc16 >> 0) & 0xFF;
    pFrame->txBuffer[pos ++] = (crc16 >> 8) & 0xFF;
	
	return pos;
}

void Wight_Transion(WITHT_FRAME *pFrame)
{
	if(pFrame->mode == WIGHT_WOEK_NORMAL)
	{
		pFrame->cmd = WIGHT_MODBUS_FUN_READ_HOLDREGS;
		pFrame->regAddr = WIGHT_MODBUSADDRREG_WGT;
		pFrame->regNum = WIGHT_MODBUS_REG_BUYE_LEN * 2;
	}
	else if(pFrame->mode ==  WIGHT_WORK_SET_ZERO)
	{	
		pFrame->cmd = WIGHT_MODBUS_FUN_WRITE_HOLDREGS;
		pFrame->regAddr = WIGHT_MODBUSADDRREG_WGT;
		pFrame->regNum = 0x0002;
		pFrame->frame[0] = 0x0000;
		pFrame->frame[1] = 0x0001;
	}
	else if(pFrame->mode == WIGHT_WORK_QP)
	{

	}
	
	pFrame->txLen = Wight_FormatFrame(pFrame);
	Wight_EnableDmaTx(pFrame->txBuffer, pFrame->txLen);
	//pFrame->tick = g_n

}

BOOL Wight_ChkRcvFrame(WITHT_FRAME *pFrame)
{
	BOOL bOk = FALSE;
	u16 crc1 = 0, crc2 = 0;
	if(pFrame->rxLen >= WIGHT_FRAME_MIN_LENTH)
	{
		//if(pFrame->cmd == pFrame->rxBuffer[WIGHT_FRAME_POS_CMD])
		{
			crc1 = Wight_GetCrc(pFrame->rxBuffer, pFrame->rxLen);
			crc2 = Wight_GetCrc16(pFrame->rxBuffer, pFrame->rxLen - 2);
			if(crc1 == crc2)
			{
				bOk = TRUE;
			}
		}
	}
	
	return bOk;
}

//
u8 GPB_RFormatFrame(u8 cmd,u16 regAdd,u16 regNum,u8 *pFrame)
{
    u8 pos = 0;
    u16 crc = 0;
    pFrame[pos++] = 0xFF;
    pFrame[pos++] = cmd;
    pFrame[pos++] = (regAdd >> 8) & 0xFF;
    pFrame[pos++] = (regAdd >> 0) & 0xFF;
    pFrame[pos++] = (regNum >> 8) & 0xFF;
    pFrame[pos++] = (regNum >> 0) & 0xFF;
	//校验
    crc = GPB_GetCrc16(pFrame, pos);
    pFrame[pos++] = (crc >> 0) & 0xFF;
    pFrame[pos++] = (crc >> 8) & 0xFF;
    return pos;
}

u8 GPB_WFormatFrame(u8 cmd,u8 len,u16 regAdd,u16 regNum,u8 *pFrame, u16 data0,u16 data1)//写两个寄存器
{
    u8 pos = 0;
    u16 crc = 0;
    pFrame[pos++] = 0xFF;
    pFrame[pos++] = cmd;
    pFrame[pos++] = (regAdd >> 8) & 0xFF;
    pFrame[pos++] = (regAdd >> 0) & 0xFF;
    pFrame[pos++] = (regNum >> 8) & 0xFF;
    pFrame[pos++] = (regNum >> 0) & 0xFF;
    pFrame[pos++] = (len >> 0) & 0xFF;
    pFrame[pos++] = (data0 >> 8) & 0xFF;
    pFrame[pos++] = (data0 >> 0) & 0xFF;
    pFrame[pos++] = (data1 >> 8) & 0xFF;
    pFrame[pos++] = (data1 >> 0) & 0xFF;
	//校验
    crc = GPB_GetCrc16(pFrame, pos);
    pFrame[pos++] = (crc >> 0) & 0xFF;
    pFrame[pos++] = (crc >> 8) & 0xFF;
    return pos;
}


#define POLYNOMIAL                      0xA001   //x^16 + x^12 + x^5 + 1
#define PRESET_VALUE                    0xFFFF
u16 GPB_GetCrc16(u8 *pBuffer, u8 len)
{
    u16 i = 0;
    u16 crc = 0;
    u8 j = 0;

    crc = PRESET_VALUE;
    for(i = 0; i < len; i++)
    {
        crc = crc ^ pBuffer[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
            {
                crc = (crc >> 1) ^ POLYNOMIAL;
            }
            else
            {
                crc = (crc >> 1);
            }
        }
    }

    return crc;
}


BOOL GPB_CheckFrame(UART_RCVFRAME *pRcvFrame)
{
    BOOL b = FALSE, a = FALSE;
	GPB_INFO *pGpbInfo = NULL;

	pGpbInfo = &g_sGpbInfo;
    if(pRcvFrame->length >= GPB_BUF_MIN_LEN)
    {
        u16 crc1 = 0, crc2 = 0;
        crc1 = GPB_GetCrc16(pRcvFrame->buffer, pRcvFrame->length - 2);
        crc2 = pRcvFrame->buffer[pRcvFrame->length - 1] ;
        crc2 <<= 8;
        crc2 |= pRcvFrame->buffer[pRcvFrame->length - 2];		
		
		if(pGpbInfo->mode == GPB_WOEK_NORMAL)
		{
			if((pRcvFrame->buffer[1] == GPB_MODBUS_READ_REG) && (pRcvFrame->length == GPB_READ_WITHT_VALUE_LEN))
			{
				a = TRUE;
			}
		}
		else if(pGpbInfo->mode == GPB_WORK_SET_ZERO)
		{
			if((pRcvFrame->buffer[1] == GPB_MODBUS_SET_REG) && (pRcvFrame->length == GPB_READ_WITHT_VALUE_LEN - 1))
			{
				a = TRUE;
			}
		}
		
		
        if((crc1 == crc2) && a)
        {
            b = TRUE;
        }
    }
    return b;
}


void GPB_TransmitCmd(GPB_TX_BUF *pBuffer)
{	
    g_sGpbInfo.txBuf.cmd = GPB_MODBUS_READ_REG;
    g_sGpbInfo.txBuf.regAdd = GPB_REG_ADDR_WGT;
    g_sGpbInfo.txBuf.regNum = 0x0002;
    g_sGpbInfo.txBuf.len = GPB_RFormatFrame(pBuffer->cmd, pBuffer->regAdd, pBuffer->regNum,  pBuffer->buffer);
    //GPB_WriteBuffer(g_sGpbInfo.txBuf.buffer, g_sGpbInfo.txBuf.len);
}

void GPB_Adjust(GPB_TX_BUF *pBuffer)
{
    pBuffer->cmd = GPB_MODBUS_SET_REG;
    pBuffer->regAdd = GPB_REG_ADDR_WGT;
    pBuffer->regNum = 0x0002;
    pBuffer->data0 = 0x0000;
    pBuffer->data1 = 0x0001;
    g_sGpbInfo.txBuf.len = GPB_WFormatFrame(pBuffer->cmd, 4, pBuffer->regAdd, pBuffer->regNum, pBuffer->buffer, pBuffer->data0, pBuffer->data1);
    
    //GPB_WriteBuffer(g_sGpbInfo.txBuf.buffer, g_sGpbInfo.txBuf.len);

}

void GPB_RtoChg(GPB_TX_BUF *pBuffer)
{
    pBuffer->cmd = GPB_MODBUS_READ_REG;
    pBuffer->regAdd = GPB_REG_ADDR_RTO;
    pBuffer->regNum = 0x0002;
    g_sGpbInfo.txBuf.len = GPB_WFormatFrame(pBuffer->cmd, 4, pBuffer->regAdd, pBuffer->regNum, pBuffer->buffer, pBuffer->data0, pBuffer->data1);
    
   // GPB_WriteBuffer(g_sGpbInfo.txBuf.buffer, g_sGpbInfo.txBuf.len);

}



void GPB_Filt_Chg()
{
    u8 pos = 0;
    u16 crc;
    g_sGpbInfo.txBuf.buffer[pos++] = 0xFF;
    g_sGpbInfo.txBuf.buffer[pos++] = GPB_MODBUS_SET_REG;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x00;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x0D;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x00;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x02;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x04;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x00;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x00;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x00;
    g_sGpbInfo.txBuf.buffer[pos++] = 0x03;
    
    crc = GPB_GetCrc16(g_sGpbInfo.txBuf.buffer, pos);
    g_sGpbInfo.txBuf.buffer[pos++] = (crc >> 0) & 0xFF;
    g_sGpbInfo.txBuf.buffer[pos++] = (crc >> 8) & 0xFF;
    g_sGpbInfo.txBuf.len = pos;
     //GPB_WriteBuffer(g_sGpbInfo.txBuf.buffer, g_sGpbInfo.txBuf.len);

}


void GPB_ChgValue(u32 value, char *buf)
{
    u32 tempValue = value & 0x000FFFFF;
    if(tempValue < 10000)
    {
         sprintf(buf, "%d.%d%d%dKg", (value & 0x000FFFFF) / 1000, ((value & 0x000FFFFF) % 1000) / 100, ((value & 0x000FFFFF) % 100) / 10, ((value & 0x000FFFFF) % 100) % 10);    
    }
    else if( 10000 <= tempValue < 100000)
    {
        sprintf(buf, "%d%d.%d%d%dKg", (value & 0x000FFFFF) / 10000, ((value %10000) & 0x000FFFFF) / 1000, ((value & 0x000FFFFF) % 1000) / 100, ((value & 0x000FFFFF) % 100) / 10, ((value & 0x000FFFFF) % 100) % 10); 
    }
    else if( 100000 <= tempValue < 1000000)
    {
        sprintf(buf, "%d%d%d.%d%d%dkg", (value & 0x000FFFFF) / 100000, (value & 0x000FFFFF) / 1000, (value & 0x000FFFFF) / 1000, ((value & 0x000FFFFF) % 1000) / 100, ((value & 0x000FFFFF) % 100) / 10, ((value & 0x000FFFFF) % 100) % 10); 
    }
}




void Witgh_CalAvg(WIGHT_INFO *pInfo, u32 value)
{
    //16窗口平滑滤波
    pInfo->sum -= pInfo->buffer[pInfo->index & 0x07];
	
    if(pInfo->index == 0)
    {
        pInfo->index = GPB_SAMPLE_NUM;
    }
    pInfo->buffer[pInfo->index & 0x07] = value;
	pInfo->index++;
    pInfo->sum += value;
    if(pInfo->index >= GPB_SAMPLE_NUM)
    {
        pInfo->avg = pInfo->sum >> 3;
    }
    else
    {
        pInfo->avg = pInfo->sum / pInfo->index;
    }
}

void Gpb_TxFrame(u32 tick)
{
	GPB_INFO *pGpbInfo = NULL;
	
	pGpbInfo = &g_sGpbInfo;
	if(pGpbInfo->mode == GPB_WOEK_NORMAL)
	{
		pGpbInfo->txBuf.cmd = GPB_MODBUS_READ_REG;
    	pGpbInfo->txBuf.regAdd = GPB_REG_ADDR_WGT;
    	pGpbInfo->txBuf.regNum = 0x0002;
   		pGpbInfo->txBuf.len = GPB_RFormatFrame(pGpbInfo->txBuf.cmd, pGpbInfo->txBuf.regAdd, pGpbInfo->txBuf.regNum,  pGpbInfo->txBuf.buffer);
	}
	else if(pGpbInfo->mode == GPB_WORK_SET_ZERO)
	{
		pGpbInfo->txBuf.cmd = GPB_MODBUS_SET_REG;
		pGpbInfo->txBuf.regAdd = GPB_REG_ADDR_WGT;
		pGpbInfo->txBuf.regNum = 0x0002;
		pGpbInfo->txBuf.data0 = 0x0000;
		pGpbInfo->txBuf.data1 = 0x0001;
		pGpbInfo->txBuf.len = GPB_WFormatFrame(pGpbInfo->txBuf.cmd, 4, pGpbInfo->txBuf.regAdd, pGpbInfo->txBuf.regNum, pGpbInfo->txBuf.buffer, pGpbInfo->txBuf.data0, pGpbInfo->txBuf.data1);
	}

	//GPB_WriteBuffer(pGpbInfo->txBuf.buffer, pGpbInfo->txBuf.len);
	pGpbInfo->tick = tick;
}


void Gpb_OpStep()
{
	GPB_INFO *pGpbInfo = NULL;
	
	pGpbInfo = &g_sGpbInfo;
	if(pGpbInfo->mode == GPB_WOEK_NORMAL)
	{
		pGpbInfo->mode = GPB_WOEK_NORMAL;
	}
	else if(pGpbInfo->mode == GPB_WORK_SET_ZERO)
	{
		pGpbInfo->mode = GPB_WOEK_NORMAL;	
	}
}


void Gpb_RcvSlove()
{
	GPB_INFO *pGpbInfo = NULL;
	
	pGpbInfo = &g_sGpbInfo;
	if(pGpbInfo->mode == GPB_WOEK_NORMAL)
	{
		pGpbInfo->mode = GPB_WOEK_NORMAL;
	}
	else if(pGpbInfo->mode == GPB_WORK_SET_ZERO)
	{
		pGpbInfo->mode = GPB_WOEK_NORMAL;	
	}
}




