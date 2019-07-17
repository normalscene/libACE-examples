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
