#include <string>
#include <iostream>
#include <ace/OS_NS_stdio.h>
#include "TcpClient.h"
#include "AFN01/F01.h"
using namespace std;

int main(int argc, char **argv)
{
	TcpClient client;	

	client.connect(argc > 1 ? argv[1] : "0.0.0.0:3322");
	client.enable(ACE_NONBLOCK);

	for(unsigned i = 0;;i++)
	{
		string s = "hello!\r";
		int l = client.sendString(s);
		if(l >= 0)
		{
			::printf("sendString:%u\r", i * l);
			::fflush(stdout);
			::usleep(500 * 1000);
			continue;
		}
		client.tryReconnect(100, 1); 
		client.enable(ACE_NONBLOCK);
	}

	return 0;
}
