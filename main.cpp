#include "TCP_Client.h"
#include <ace/OS_NS_stdio.h>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	TCP_Client client;	

	client.connect(argc > 1 ? argv[1] : "0.0.0.0:3322");
	client.enable(ACE_NONBLOCK);

	for(unsigned i = 0;;i++)
	{
		string s = "hello!\r";
		if( (int)client.send(s.data(), s.length(), MSG_NOSIGNAL) > 0)
		{
			::printf("send:%u\r", i*s.length());
			::fflush(stdout);
			::usleep(500 * 1000);
			continue;
		}
		perror("send");
		if(EPIPE == errno || client.get_handle() == ACE_INVALID_HANDLE)
		{
			client.try_reconnect(100, 1); 
			client.enable(ACE_NONBLOCK);
			perror("connect");
		}
	}

	return 0;
}
