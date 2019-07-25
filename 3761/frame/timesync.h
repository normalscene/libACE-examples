#ifndef _3761_TIMESYNC_H_
#define _3761_TIMESYNC_H_
#include <string>
#include "con3761adp.h"
#include "TcpClient.h"
using namespace std;

class Timesync

	emt_err_t test_pack_0001_m2s()
	{
	   eCmErr eRet;
	   uint16_t usBuflen = 0;
	   smt_pack_t *pscmPacket = (smt_pack_t*)g_ucPackMem;
	   

		/* 2 环境初始化 */
		sCmInit  sInit;
		sInit.eRole = MT_ROLE_CONTOR;
		sInit.ucPermitDelayMinutes = 255;
		eRet = ecm_3761_init(&sInit);
		if(eRet != MT_OK)
		{
			printf("初始化失败\n");
			return eRet;
		}
		//const char *str = "0123456789ABCDEF";
		//bcm_set_pw(str);
	  
		
		/* 3 封装参数 */
		memcpy(pscmPacket->sAddress.acRegionCode, "4301", 4);
		pscmPacket->sAddress.usTAddress = 256;
		pscmPacket->sAddress.bTeamAddr  = false;
		pscmPacket->sAddress.ucMAddress = 1;

		pscmPacket->eAFN = AFN_00_CONF;
		pscmPacket->eDir = MT_DIR_S2M;
		pscmPacket->ePRM = MT_PRM_PASIVE;
		
		pscmPacket->ePos = MT_POS_SIGLE;
		pscmPacket->ucSeq = 1;
		pscmPacket->bAcdFcb = true;


		pscmPacket->usDataNum = 1;
		pscmPacket->sData[0].eCmd  = CMD_AFN_0_F1_ALL_OK;
		pscmPacket->sData[0].bApp  = true;
		pscmPacket->sData[0].usPN  = 0;

		time_t now = time(0);



		/* 4 调用函数 */
		eRet = emt_pack(pscmPacket, &usBuflen, (uint8_t*)g_ucOutBuf);
		if(eRet != MT_OK)
		{
			printf("ecm_3761_pack error %d\n", eRet);
			return eRet;
		}
		
		/* 5 输出结果 */ 
		printf_buffer_color((char*)g_ucOutBuf, usBuflen);
		return MT_OK;
	}


#endif//_3761_TIMESYNC_H_
