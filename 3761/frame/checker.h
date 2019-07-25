#ifndef _3761_CHECK_H_
#define _3761_CHECK_H_
#include <string>
#include "con3761adp.h"
#include "TcpClient.h"
#include "frame.h"
using namespace std;

class Checker
{
public:
	int check(uint8_t* begin, uint8_t* end)
	{
		uint16_t len = end - begin + 1;
		Header &head = *(Header*)begin;

		if(len < MT_FRM_LEN_MIN)
		{
			return 0;
		}
		if( head.H1 != 0x68 
		||  head.H2 != 0x68
		||  head.V1 != 0x02
		||  head.V2 != 0x02
		||  head.L1 != head.L2
		||  head.L1 > (MT_FRM_LEN_MAX-sizeof(Header)-2))
		{
			printf("head{%d,%d,%d,%d,%d,%d}\n",
					head.H1,
					head.V1,
					head.L1,
					head.V2,
					head.L2,
					head.H2
					);
			return -1;
		}

		if(head.L1 > (len - sizeof(Header) - 2))
		{
			return 0;
		}
		if(*end != 0x16)
		{
			printf("check.end != 0x16 == %02X\n", *end);
			return -1;
		}
		uint8_t cs = 0;
		uint8_t *p = begin + sizeof(Header);
		for(int i = 0; i < head.L1; i++)
		{
			cs += p[i];	
		}
		p += head.L1;
		if(cs != *p)
		{
			printf("checksum:%02X != %02X\n", cs, *p);
			return -1;
		}
		return 1;
	}
};

#endif//_3761_CHECK_H_
