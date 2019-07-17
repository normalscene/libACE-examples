#include "TCP_Client.h"
#include <ace/OS_NS_stdio.h>

bool TCP_Client::connect(const char dst_ip_port[])
{
	address.set(dst_ip_port ? dst_ip_port : "localhost:65545");

	return do_reconnect();
}

bool TCP_Client::do_reconnect(void)
{
	close();
	int r = connector.connect(*this, address);

	if(0 == r)
	{
		print_connection();
		return true;
	}
	return false;
}

bool TCP_Client::try_reconnect(const int times, const int interval)
{
	for(int i = 0; i < (unsigned)times; i++)
	{
		if(do_reconnect())		
		{
			return true;
		}
		printf("%s %d\r", strerror(errno), i);
		fflush(stdout);
		sleep((unsigned)interval);
	}
	return false;
}

void TCP_Client::print_connection(void)
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
