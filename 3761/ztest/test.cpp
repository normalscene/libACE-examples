#include <stdio.h>
#include <string.h>
#include "con3761adp.h"


int get_login_frame(uint8_t *buf, const char *region, uint16_t termaddr, uint8_t master);

int main(int argc, char **argv)
{
	int len = 0;
	uint8_t buf[1<<16];


	len = get_login_frame(buf, "4301", 256, 0);

	for(int i = 0; i <len; i++)
	{
		printf("%02X ", buf[i]);
	}
	printf("\n");
	fflush(stdout);

	return 0;
}





int get_login_frame(uint8_t *buf, const char *region, uint16_t termaddr, uint8_t master)
{
   eCmErr eRet;
   uint16_t len = 0;
   sCmPacket &pack = *(sCmPacket*)buf;
   

    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;

    eRet = ecm_3761_init(&sInit);

    if(eRet != MT_ERR_OK)
    {
        printf("初始化失败\n");
        return 0;
    }

    /* 3 封装参数 */
    memcpy(pack.sAddress.acRegionCode, region, 4);
    pack.sAddress.usTAddress = termaddr;
    pack.sAddress.bTeamAddr  = false;
    pack.sAddress.ucMAddress = master;
    pack.bReSend = false;
    pack.bActive = true;

    pack.ucCmdNum = 1;
    pack.sCmdData[0].eCmd  = CMD_LOG_IN;
    pack.sCmdData[0].bApp  = false;
    pack.sCmdData[0].usPN  = 0;

    /* 4 调用函数 */
    eRet = ecm_3761_pack(&pack, buf, &len);
    if(eRet != MT_ERR_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return 0;
    }
    
    return len;
}
