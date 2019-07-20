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
