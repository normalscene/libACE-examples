#ifndef _3761_LINKHEADER_H_
#define _3761_LINKHEADER_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#pragma pack(1)

class Header
{
public:
	uint8_t header;
public:
	void init(void)
	{
		header = 0x68;
	}
	int size(void)const
	{
		return sizeof(header);
	}
public:
	void set(void)
	{
		header = 0x68;
	}
	const uint8_t get(void)const
	{
		return header;
	}
	bool check(void)const
	{
		return 0x68 == header;
	}
public:
	void print(void)const
	{
		printf("%10s: %02X\n", "header", header);
	}
};

class Length
{
public:
	uint8_t length[2];
public:
	void init(void)
	{
		setLength(0);
		setVersion(2);
	}
	int size(void)const
	{
		return sizeof(length);
	}
public:
	const uint16_t getLength(void)const
	{
		return ((length[1] << 8) | length[0]) >> 2;
	}
	const uint8_t getVersion(void)const
	{
		return length[0] & 3;
	}
public:
	void setLength(const uint16_t v)
	{
		uint16_t l = (v << 2) | getVersion();

		length[0] = 0xff & (l);
		length[1] = 0xff & (l >> 8);
	}
	void setVersion(uint8_t v)
	{
		length[0] &= ~3;
		length[0] |=  (3 & v);
	}
public:
	bool check(const Length& l)const
	{
		return getLength()  == l.getLength()
			&& getVersion() == l.getVersion();
	}
public:
	void print(void)const
	{
		printf("%10s: %02X %02X = <%d,%d>\n", 
				"length",
				length[0],
				length[1],
				getLength(),
				getVersion());
	}
};

class LinkTail
{
public:
	uint8_t checksum;
	uint8_t linktail;
public:
	void init(uint8_t cs)
	{
		checksum = cs;
		linktail = 0x16;
	}
	bool check(uint8_t cs)const
	{
		return checksum == cs
			&& linktail == 0x16;
	}
	const uint8_t size(void)const
	{
		return sizeof(*this);
	}
	void print(void)const
	{
		printf("%10s: %02X\n", "checksum", checksum);
		printf("%10s: %02X\n", "frameend", linktail);
	}
};

class LinkHeader
{
public:
	Header	header1;
	Length	length1;
	Length	length2;
	Header	header2;

public:
	LinkHeader(void)
	{
		init();
	}
public:
	void init(void)
	{
		header1.init();
		length1.init();
		length2.init();
		header2.init();
		setVersion(2);
	}
	uint8_t size(void)const
	{
		return sizeof(*this);
	}
	bool check(void)const
	{
		return header1.check()
			&& length1.check(length2)
			&& header2.check();
	}
	void print(void)const
	{
		header1.print();
		length1.print();
		length2.print();
		header2.print();
	}

public:
	const uint16_t getLength(void)const
	{
		return length1.getLength();
	}
	const uint8_t  getVersion(void)const
	{
		return length1.getVersion();
	}

public:
	void setLength(uint16_t l)
	{
		length1.setLength(l);
		length2.setLength(l);
	}
	void setVersion(uint8_t v)
	{
		length1.setVersion(v);
		length2.setVersion(v);
	}
};

#pragma pack(0)

#endif//_3761_LINKHEADER_H_
