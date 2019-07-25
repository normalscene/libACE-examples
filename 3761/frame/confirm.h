#ifndef _3761_CONFIRM_H_
#define _3761_CONFIRM_H_
#include "con3761adp.h"
#include "TcpClient.h"
#include "calendar.h"

class Confirm
{
protected:
	TcpClient&	m_client;
public:
	Confirm(TcpClient& client):m_client(client)
	{
	}
public:
	bool allOk(const uint8_t *data)
	{
	   eCmErr eRet;
	   uint16_t usBuflen = 0;
	   uint8_t cleanbuf[1<<10];
	   smt_pack_t *pscmPacket = (smt_pack_t*)cleanbuf;
	   CommonHeader& head = *(CommonHeader*)data;

		/* 2 环境初始化 */
		sCmInit  sInit;
		sInit.eRole = MT_ROLE_CONTOR;
		sInit.ucPermitDelayMinutes = 0;
		eRet = ecm_3761_init(&sInit);
		if(eRet != MT_OK)
		{
			printf("初始化失败\n");
			return eRet;
		}
		const char *str = "1234567890123456";
		//bcm_set_pw(str);
	  
		
		/* 3 封装参数 */
		memcpy(pscmPacket->sAddress.acRegionCode, "4301", 4);
		pscmPacket->sAddress.usTAddress = head.terminal;
		pscmPacket->sAddress.bTeamAddr  = head.team;
		pscmPacket->sAddress.ucMAddress = head.master;

		pscmPacket->eAFN = AFN_00_CONF;
		pscmPacket->eDir = MT_DIR_S2M;
		pscmPacket->ePRM = MT_PRM_PASIVE;
		
		pscmPacket->ePos = MT_POS_SIGLE;
		pscmPacket->ucSeq = head.SEQ;
		pscmPacket->bAcdFcb = true;


		pscmPacket->usDataNum = 1;
		pscmPacket->sData[0].eCmd  = CMD_AFN_0_F1_ALL_OK;
		pscmPacket->sData[0].bApp  = true;
		pscmPacket->sData[0].usPN  = 0;

		/*
		Calendar now;
		pscmPacket->sData[0].uApp.sCheckTime.ucYear   = now.year();
		pscmPacket->sData[0].uApp.sCheckTime.ucMonth  = now.month();
		pscmPacket->sData[0].uApp.sCheckTime.ucWeek   = now.weekday();
		pscmPacket->sData[0].uApp.sCheckTime.ucDay    = now.day();
		pscmPacket->sData[0].uApp.sCheckTime.ucHour   = now.hour();
		pscmPacket->sData[0].uApp.sCheckTime.ucMinute = now.minute();
		pscmPacket->sData[0].uApp.sCheckTime.ucSecond = now.second();
		*/


		/* 4 调用函数 */
		uint8_t buf[1<<10];
		uint16_t len = 0;

		eRet = emt_pack(pscmPacket, &len, buf);
		if(eRet != MT_OK)
		{
			printf("ecm_3761_pack error %d\n", eRet);
			return eRet;
		}

		if(m_client.send_n(buf, len) == len)
		{
			printf("%s(%d)\n", __func__, len);
			for(int i = 0; i<len; i++)
			{
				printf("%02X ", buf[i]);
			}
			printf("\n");
			fflush(stdout);
		}
		
		return len;
	}

};

#endif//_3761_CONFIRM_H_
