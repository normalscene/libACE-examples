#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include <ace/OS_NS_stdio.h>
#include "TcpClient.h"
#include "con3761adp.h"
using namespace std;


const uint8_t	g_master	= 0;
const uint16_t	g_terminal	= 256;
const string	g_region	= "4301";
const string	g_svraddr	= "124.232.137.226:2028";

bool login(TcpClient&);
int  check(uint8_t* begin, uint8_t* end);
void process(uint8_t* begin, uint8_t* end);

int main(int argc, char **argv)
{
	uint8_t		data[1<<16];
	TcpClient	client;	
	string		addr = argc > 1 ? argv[1] : g_svraddr;

	if( client.connect(addr.data()) == false )
	{
		return -1;
	}
	if(login(client) == false)
	{
		return -1;
	}

	while(1)
	{
		uint8_t*		p		= data;
		uint8_t*		end		= data + sizeof(data);
		ACE_Time_Value	t(0, 10000);

		while(p < end)
		{
			int len = 0;
			if( (len = client.recv_n(p, 1, 0, &t)) <= 0)
			{
				if(len < 0)
				{
					//perror("recv_n");
				}
				usleep(100);
				continue;
			}
			//printf("%02X ", *p);fflush(stdout);
			switch(check(data, p))
			{
				case 0:
					p++;
					break;
				case -1:
					memmove(data, data+1, p - data - 1);
					p--;
					break;
				default:
					process(data, p);
					p = data;
					break;
			}
		}
	}

	sleep(3);
	client.close();

	return 0;
}

int get_login_frame(uint8_t*, const char *, uint16_t , uint8_t);
bool login(TcpClient& client)
{

	uint8_t data[1<<8];

	int len = get_login_frame(data, "4301", 256, 0);
	if(len < MT_FRM_LEN_MIN)
	{
		printf("failed to get login frame\n");
		return false;
	}
	if( client.send_n(data, len) != len )
	{
		printf("login failed!\n");
		return false;
	}
	printf("login:");
	for(int i = 0; i < len; i++)
	{
		printf("%02X ", data[i]);
	}
	printf("\n");

	return true;
}

int check(uint8_t* begin, uint8_t* end)
{
#pragma pack(1)
	typedef struct{
		uint8_t H1;
		uint16_t V1:2;
		uint16_t L1:14;
		uint16_t V2:2;
		uint16_t L2:14;
		uint8_t H2;
	}header_t;
#pragma pack(0)

	uint16_t len = end - begin + 1;
	header_t &head = *(header_t*)begin;

	if(len < MT_FRM_LEN_MIN)
	{
		return 0;
	}
	if( head.H1 != 0x68 
	||  head.H2 != 0x68
	||  head.V1 != 0x02
	||  head.V2 != 0x02
	||  head.L1 != head.L2
	||  head.L1 > (MT_FRM_LEN_MAX-sizeof(header_t)-2))
	{
		printf("head{%d,%d,%d,%d,%d,%d}\n",
				head.H1,
				head.V1,
				head.L1,
				head.V2,
				head.L2,
				head.H2
				);
		return -1;
	}

	if(head.L1 > (len - sizeof(header_t) - 2))
	{
		return 0;
	}
	if(*end != 0x16)
	{
		printf("check.end != 0x16 == %02X\n", *end);
		return -1;
	}
	uint8_t cs = 0;
	uint8_t *p = begin + sizeof(header_t);
	for(int i = 0; i < head.L1; i++)
	{
		cs += p[i];	
	}
	p += head.L1;
	if(cs != *p)
	{
		printf("checksum:%02X != %02X\n", cs, *p);
		return -1;
	}
	return 1;
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

    if(eRet != MT_OK)
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
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return 0;
    }
    
    return len;
}

void process(uint8_t *data, uint8_t* end)
{
	uint8_t *p = data;
	printf("\nrecv:");
	while(p <= end)
	{
		printf("%02X ", *p++);
	}
	printf("\n");
	fflush(stdout);
}
