# libACE-examples
TCP_Client

## TcpClient.h
```C++
#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <string>

class TcpClient : public ACE_SOCK_Stream
{
	protected:
		ACE_INET_Addr		address;
		ACE_SOCK_Connector	connector;

	public:
		bool connect(const char []="127.0.0.1:65535");
		bool reconnect(void);
		bool tryReconnect(const int32_t=3, const int32_t=3);
		void printConnectionInfo(void);
		int32_t sendString(const std::string&, const int32_t=MSG_NOSIGNAL);
};

#endif//_TCPCLIENT_H_
```

## TcpClient.cpp
```C++
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
```

## main.cpp
```C++
#include "TcpClient.h"
#include <ace/OS_NS_stdio.h>
#include <iostream>
#include <string>
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

