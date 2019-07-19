#ifndef _3761_FRAME_H_
#define _3761_FRAME_H_
#include "LinkHeader.h"
#include "UserHeader.h"
#include "Payload.h"

class Frame
{
public:
	LinkHeader	linkHeader;
	UserHeader	userHeader;
	Payload		payload;
public:
	void init(void)
	{
		linkHeader.init();
		userHeader.init();
	}
	bool check(void)const
	{
		return linkHeader.check()
			&&  getLinkTail().check(checksum());
	}
	uint8_t checksum(void)const
	{
		uint8_t cs = 0;
		uint8_t *p = (uint8_t*)&userHeader;
		uint16_t l = linkHeader.getLength();
		
		if(l < sizeof(UserHeader))
		{
			return 0;
		}
		if(l > (payload.maxPayload() + sizeof(UserHeader)) )
		{
			return 0;
		}
		for(int i = 0; i < l; i++)
		{
			cs += p[i];
		}
		return cs;
	}
	uint16_t getLength(void)const
	{
		return sizeof(LinkHeader) 
			+  linkHeader.getLength()
			+  sizeof(LinkTail);
	}
	void setLength(uint16_t len)
	{
		linkHeader.setLength(len);
	}
	const LinkTail& getLinkTail(void)const
	{
		uint8_t *p = (uint8_t*)&userHeader;

		return *(LinkTail*)(p + linkHeader.getLength());
	}
	const uint16_t getPayloadLength(void)const
	{
		uint16_t l = linkHeader.getLength();

		if(l < sizeof(UserHeader))
		{
			return 0;
		}
		return l - sizeof(UserHeader);
	}
	void printHex(void)
	{
		printf("frame: ");
		for(int i = 0; i < getLength(); i++)
		{
			printf("%02X ", ((uint8_t*)this)[i]);
		}
		printf("\n");
	}
	void print(void)
	{
		printf("check:%s\n", check() ? "ok" : "failed");
		linkHeader.print();
		userHeader.print();
		payload.print(getPayloadLength());
		getLinkTail().print();
		printf("\n");
	}
	void make(void)
	{
		LinkTail& t = *(LinkTail*)&getLinkTail();
		t.init(checksum());
	}
};


#endif//_3761_FRAME_H_
