#include "TcpClient.h"
#include <ace/OS_NS_stdio.h>
using namespace std;

bool TcpClient::connect(const char dst_ip_port[])
{
	address.set(dst_ip_port ? dst_ip_port : "localhost:65535");

	return reconnect();
}

bool TcpClient::reconnect(void)
{
	close();
	int32_t r = connector.connect(*this, address);

	if(0 == r)
	{
		printConnectionInfo();
		return true;
	}
	return false;
}

bool TcpClient::tryReconnect(const int32_t times, const int32_t interval)
{
	for(int32_t i = 0; i < (unsigned)times; i++)
	{
		if(reconnect())		
		{
			return true;
		}
		printf("%s %s:%d %d times\r", 
				strerror(errno), 
				address.get_host_addr(), 
				address.get_port_number(), 
				i);
		fflush(stdout);
		sleep((unsigned)interval);
	}
	return false;
}

void TcpClient::printConnectionInfo(void)
{
	ACE_INET_Addr l;
	ACE_INET_Addr r;

	get_local_addr(l);
	get_remote_addr(r);

	::printf("connection from %s:%d to %s:%d\n",
			l.get_host_addr(),
			l.get_port_number(),
			r.get_host_addr(),
			r.get_port_number());
}

int32_t  TcpClient::sendString(const string& s, const int32_t flags)
{
	return (int)send_n(s.data(), s.length(), flags);
}
