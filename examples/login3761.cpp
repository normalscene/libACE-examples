#include <string>
#include <iostream>
#include <ace/OS_NS_stdio.h>
#include "TcpClient.h"
#include "AFN00/F01.h"
#include "AFN00/F02.h"
#include "AFN00/F03.h"
#include "AFN01/F01.h"
#include "AFN05/F31.h"
using namespace std;

void procFrame(Frame& frame, TcpClient& client);
void procAFN00(Frame& frame, TcpClient& client);
void procAFN01(Frame& frame, TcpClient& client);
void procAFN02(Frame& frame, TcpClient& client);
void procAFN03(Frame& frame, TcpClient& client);
void procAFN04(Frame& frame, TcpClient& client);
void procAFN05(Frame& frame, TcpClient& client);
void procAFN06(Frame& frame, TcpClient& client);
void procAFN07(Frame& frame, TcpClient& client);
void procAFN08(Frame& frame, TcpClient& client);
void procAFN09(Frame& frame, TcpClient& client);
void procAFN10(Frame& frame, TcpClient& client);
void procAFN11(Frame& frame, TcpClient& client);
void procAFN12(Frame& frame, TcpClient& client);
void procAFN13(Frame& frame, TcpClient& client);
void procAFN14(Frame& frame, TcpClient& client);
void procAFN15(Frame& frame, TcpClient& client);

int main(int argc, char **argv)
{
	TcpClient	client;	
	AFN01::F01	login;

	login.init(0x4301, 256, 0);
	login.make();

	client.connect(argc > 1 ? argv[1] : "124.232.137.226:2028");

	if( client.send_n(&login, login.getLength()) == login.getLength() )
	{
		Frame frame;
		uint8_t *p = (uint8_t*)&frame;
		uint8_t *s = (uint8_t*)&frame;

		login.printHex();
		while(1)
		{
			ACE_Time_Value t(0,1000);
			int len = client.recv_n(p, 1, MSG_NOSIGNAL, &t);
			if(len > 0)
			{
				if(0x68 != s[0])
				{
					memmove(s, s+1, p-s-1);
					p--;
				}
				if((p - s) >= frame.linkHeader.size()
				&& frame.linkHeader.check() == false)
				{
					memmove(s, s+1, p-s-1);
					p--;
				}
				printf("%02X ", p[0]);
				if(frame.check())
				{
					procFrame(frame, client);
					frame.init();
					p = s;
					continue;
				}
				fflush(stdout);
				p++;
			}
		}
	}
	sleep(3);
	client.close();

	return 0;
}

void procFrame(Frame& frame, TcpClient& client)
{
	printf("\n");
	switch(frame.userHeader.function.AFN)
	{
		case 0:
			procAFN00(frame, client);
			break;
		case 1:
			procAFN01(frame, client);
			break;
		case 2:
			procAFN02(frame, client);
			break;
		case 3:
			procAFN03(frame, client);
			break;
		case 4:
			procAFN04(frame, client);
			break;
		case 5:
			procAFN05(frame, client);
			break;
		case 6:
			procAFN06(frame, client);
			break;
		case 7:
			procAFN07(frame, client);
			break;
		case 8:
			procAFN08(frame, client);
			break;
		case 9:
			procAFN09(frame, client);
			break;
		case 10:
			procAFN10(frame, client);
			break;
		case 11:
			procAFN11(frame, client);
			break;
		case 12:
			procAFN12(frame, client);
			break;
		case 13:
			procAFN13(frame, client);
			break;
		case 14:
			procAFN14(frame, client);
			break;
		case 15:
			procAFN15(frame, client);
			break;
	}
}

void procAFN00(Frame& frame, TcpClient& client)
{
	DataPoint& dataPoint = frame.payload.dataPoint(0);
	switch(dataPoint.getFn())
	{
	case 1:
		((AFN00::F01*)&frame)->print();
		break;
	case 2:
		((AFN00::F02*)&frame)->print();
		break;
	case 3:
		((AFN00::F03*)&frame)->print();
		break;
	}
}
void procAFN01(Frame& frame, TcpClient& client)
{
}
void procAFN02(Frame& frame, TcpClient& client)
{
}
void procAFN03(Frame& frame, TcpClient& client)
{
}
void procAFN04(Frame& frame, TcpClient& client)
{
}
void procAFN05(Frame& frame, TcpClient& client)
{
	((AFN05::F31*)&frame)->print();

	AFN00::F01 response;
	response.userHeader.address		 = frame.userHeader.address;
	response.userHeader.sequence.SEQ = frame.userHeader.sequence.SEQ;
	response.make();

	response.printHex();
	return;

	if( client.send_n(&response, response.getLength()) > 0)
	{
		response.printHex();
		response.print();
	}
}
void procAFN06(Frame& frame, TcpClient& client)
{
}
void procAFN07(Frame& frame, TcpClient& client)
{
}
void procAFN08(Frame& frame, TcpClient& client)
{
}
void procAFN09(Frame& frame, TcpClient& client)
{
}
void procAFN10(Frame& frame, TcpClient& client)
{
}
void procAFN11(Frame& frame, TcpClient& client)
{
}
void procAFN12(Frame& frame, TcpClient& client)
{
}
void procAFN13(Frame& frame, TcpClient& client)
{
}
void procAFN14(Frame& frame, TcpClient& client)
{
}
void procAFN15(Frame& frame, TcpClient& client)
{
}
