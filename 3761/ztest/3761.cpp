#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include <ace/OS_NS_stdio.h>
#include "login.h"
#include "checker.h"
#include "processer.h"
using namespace std;


const uint8_t	g_master	= 0;
const uint16_t	g_terminal	= 256;
const string	g_region	= "4301";
const string	g_server	= "124.232.137.226:2028";

int main(int argc, char **argv)
{
	uint8_t		data[1<<16];
	TcpClient	client;	
	Checker		checker;
	Login		login(client);
	Processer	processer(client);
	string		addr = argc > 1 ? argv[1] : g_server;

	if( client.connect(addr.data()) == false )
	{
		return -1;
	}
	if(login.login(g_region, g_terminal, g_master) == false)
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
			switch(checker.check(data, p))
			{
				case 0:
					p++;
					break;
				case -1:
					memmove(data, data+1, p - data - 1);
					p--;
					break;
				default:
					processer.process(data, p);
					p = data;
					break;
			}
		}
	}

	sleep(3);
	client.close();

	return 0;
}
