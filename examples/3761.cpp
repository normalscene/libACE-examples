#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include <ace/OS_NS_stdio.h>
#include "TcpClient.h"
#include "con3761adp.h"
#include "qgdw_3761_api.h"


void login(TcpClient);
void check_connection(TcpClient&);
bool check_3761header(uint8_t*p);

int main(int argc, char **argv)
{
	TcpClient	client;	
	uint8_t		data[1<<16];

	client.connect(argc > 1 ? argv[1] : "124.232.137.226:2028");

	while(1)
	{
		bool			valid	= false; 
		uint8_t*		p		= data;
		uint8_t*		end		= data + sizeof(data);
		ACE_Time_Value	t(0, 1000);
		time_t			now		= time(0);

		while(p < end)
		{
			if(abs(time(0) - now) > 10)
			{
				login(client);
				now = time(0);
			}
			if( client.recv_n(p, 1, MSG_NOSIGNAL, &t) <= 0)
			{
				check_connection(client);
				continue;
			}
			printf("%02X\n");
			if(!valid)
			{
				valid = check_3761header(data);
			}
		}
	}

	sleep(3);
	client.close();

	return 0;
}

void login(TcpClient& client)
{

    uint16_t		len			= 0;
	uint8_t			buf[1<<16]	= {0};
    smt_pack_t*		pack		= (smt_pack_t*)buf;
    emt_dir_t		eDir		= MT_DIR_UNKOWN;
    emt_err_t		err			= MT_OK;


	eDir = MT_DIR_S2M;

    // address
    uint8_t strRegionCode[5] = "4301"; 
    memcpy((void*)pack->sAddress.acRegionCode, (void*)strRegionCode, strlen((char*)strRegionCode));
    pack->sAddress.usTAddress = 256;
    pack->sAddress.ucMAddress = 0;
    pack->sAddress.bTeamAddr = false;

    // main_code 
    pack->eMainCmd	 = AFN_02_LINK;                              // (2)这里需要修改
    pack->sCtrl.eRPM = MT_PRM_ACTIVE;

    // ctrl_filed
    err = eMtGetCtrl(pack->eMainCmd, eDir,pack->sCtrl.eRPM, &(pack->sCtrl));
    

    // subCode
    pack->ucSubNum =		1;
    pack->sSub[0].eSubCmd   =  CMD_AFN_2_FN_1_LOG_IN;      // (3)这里需要修改
    pack->sSub[0].usPN      =  0;
    pack->sSub[0].len		=  0;

    /* 4 调用函数 */ 
    err = emt_pack(pack, &len, (uint8_t *)buf);
    if(err != MT_OK)
    {
        printf("emt_pack error code %d\n", err);
        return err;
    }
	client.send_n(buf, len);
}

void check_connection(TcpClient& client)
{
}

bool check_3761header(uint8_t *p)
{
}

