# libACE-examples
TCP_Client

## TCP_Client.h
```C++
#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

class TCP_Client : public ACE_SOCK_Stream
{
	protected:
		ACE_INET_Addr		address;
		ACE_SOCK_Connector	connector;

	public:
		bool connect(const char dst_ip_port[]);
		bool do_reconnect(void);
		bool try_reconnect(const int=3, const int=3);
		void print_connection(void);
};



#endif//_TCPCLIENT_H_

```

## TCP_Client.cpp
```C++
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
```

## main.cpp
```C++
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
```
## makefile
```C++
tcpclient.exe: *.cpp
	g++ -lACE $+ -o $@
	strip $@

arm_tcpclient.exe: *.cpp
	arm-linux-g++ -lACE $+ -o $@
	arm-linux-strip $@


clean:
	rm *.exe
```
