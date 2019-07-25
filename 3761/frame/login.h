#ifndef _3761_LOGIN_H_
#define _3761_LOGIN_H_
#include <string>
#include "con3761adp.h"
#include "TcpClient.h"
using namespace std;

class Login
{
protected:
	uint8_t		m_master;
	uint16_t	m_terminal;
	string		m_region;
	TcpClient&	m_client;
public:
	Login(TcpClient& client):
		m_client(client),
		m_terminal(0),
		m_master(0)
	{
	}
public:
	bool login(const string& region, uint16_t terminal, uint8_t master)
	{
		uint8_t data[1<<8];

		m_region	= region;
		m_terminal	= terminal;
		m_master	= master;

		int len = make(data);
		if(len < MT_FRM_LEN_MIN)
		{
			printf("failed to get login frame\n");
			return false;
		}
		if( m_client.send_n(data, len) != len )
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
	int make(uint8_t *buf)
	{
	   eCmErr eRet;
	   uint16_t len = 0;
	   sCmPacket &pack = *(sCmPacket*)buf;
	   

		sCmInit  sInit;
		sInit.eRole = MT_ROLE_CONTOR;
		sInit.ucPermitDelayMinutes = 0;

		eRet = ecm_3761_init(&sInit);

		if(eRet != MT_OK)
		{
			printf("初始化失败\n");
			return 0;
		}

		/* 3 封装参数 */
		memcpy(pack.sAddress.acRegionCode, m_region.data(), 4);
		pack.sAddress.usTAddress = m_terminal;
		pack.sAddress.bTeamAddr  = false;
		pack.sAddress.ucMAddress = m_master;
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

};


#endif//_3761_LOGIN_H_
