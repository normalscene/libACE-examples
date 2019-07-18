#ifndef _3761_LINKFRAME_H_
#define _3761_LINKFRAME_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#pragma pack(1)

class LinkHeader
{
public:
	uint8_t m_header;
public:
	LinkHeader(void)
	{
		m_header = 0x68;
	}
	void set(void)
	{
		m_header = 0x68;
	}
	const uint8_t get(void)const
	{
		return m_header;
	}
	bool check(void)const
	{
		return 0x68 == m_header;
	}
	void print(void)
	{
		printf("header: %02X\n", m_header);
	}
};

class LinkLength
{
public:
	uint8_t m_length[2];
public:
	LinkLength(void)
	{
		setLength(0);
		setVersion(2);
	}
	const uint16_t getLength(void)const
	{
		return (m_length[1] << 6) | (m_length[0] >> 2);
	}
	void setLength(const uint16_t v)
	{
		uint16_t l = (v << 2);

		m_length[0] = 0xff & (l);
		m_length[1] = 0xff & (l >> 8);
	}
	void setVersion(uint8_t v)
	{
		m_length[0] &= ~3;
		m_length[0] |=  3 & v;
	}
	uint8_t getVersion(void)const
	{
		return m_length[0] & 3;
	}
	void print(void)
	{
		printf("length: %02X %02X = <%d,%d>\n", 
				m_length[0],
				m_length[1],
				getLength(),
				getVersion());
	}
	bool operator==(const LinkLength& l)
	{
		return m_length[0] == l.m_length[0]
			&& m_length[1] == l.m_length[1];
	}
};

class LinkControl
{
public:
	uint8_t DIR:1;
	uint8_t PRM:1;
	uint8_t FBC:1;
	uint8_t FCV:1;
	uint8_t LFN:4;
public:
	enum
	{
		LFN_PRM0_0  = 0,
		LFN_PRM0_1  = 1,
		LFN_PRM0_2  = 2,
		LFN_PRM0_3  = 3,
		LFN_PRM0_4  = 4,
		LFN_PRM0_5  = 5,
		LFN_PRM0_6  = 6,
		LFN_PRM0_7  = 7,
		LFN_PRM0_8  = 8,
		LFN_PRM0_9  = 9,
		LFN_PRM0_10 = 10,
		LFN_PRM0_11 = 11,
		LFN_PRM0_12 = 12,
		LFN_PRM0_13 = 13,
		LFN_PRM0_14 = 14,
		LFN_PRM0_15 = 15,
	};
	enum
	{
		LFN_PRM1_0  = 0,
		LFN_PRM1_1  = 1,
		LFN_PRM1_2  = 2,
		LFN_PRM1_3  = 3,
		LFN_PRM1_4  = 4,
		LFN_PRM1_5  = 5,
		LFN_PRM1_6  = 6,
		LFN_PRM1_7  = 7,
		LFN_PRM1_8  = 8,
		LFN_PRM1_9  = 9,
		LFN_PRM1_10 = 10,
		LFN_PRM1_11 = 11,
		LFN_PRM1_12 = 12,
		LFN_PRM1_13 = 13,
		LFN_PRM1_14 = 14,
		LFN_PRM1_15 = 15,
	};
public:
	void print(void)
	{
		printf("control: %02X "
			   "{DIR=%d, PRM=%d, FBC=%d FCV=%d, LFN=%d}\n", 
				get(), DIR, PRM, FBC, FCV, LFN);
	}
	void set(uint8_t v)
	{
		*((uint8_t*)this) = v;
	}
	uint8_t get(void)
	{
		return *((uint8_t*)this);
	}
};

class LinkAddress
{
public:
	uint8_t m_region[2];
	uint8_t m_terminal[2];
	uint8_t m_station[1];
public:
	void setRegion(const char s[])
	{
		setRegion(strtol(s ? s : "0", 0, 16));	
	}
	void setRegion(const uint16_t v)
	{
		m_region[0] = 0xff & (v);
		m_region[1] = 0xff & (v >> 8);
	}
	const uint16_t getRegion(void)const
	{
		return (m_region[1] << 8) | m_region[0];
	}
	void printRegion(void)const
	{
		printf("region:  %02X %02X = %02X%02X\n",
			   m_region[0],
			   m_region[1],
			   m_region[1],
			   m_region[0]);
	}
public:
	void setTerminal(const uint16_t v)
	{
		m_terminal[0] = 0xff & v;
		m_terminal[1] = 0xff & (v >> 8);
	}
	const uint16_t getTerminal(void)const
	{
		return (m_terminal[1] << 8) | m_terminal[0];
	}
	void printTerminal(void)const
	{
		printf("termial: %02X %02X = %d\n",
			   m_terminal[0],
			   m_terminal[1],
			   getTerminal());
	}
public:
	void setStation(const uint8_t v)
	{
		m_station[0] = v;
	}
	const uint8_t getStation(void)const
	{
		return m_station[0];
	}
	void printStation(void)const
	{
		printf("station: %02X = %d\n",
			   m_station[0],
			   getStation());
	}
public:
	void print(void)const
	{
		printRegion();
		printTerminal();
		printStation();
	}
};

class LinkData 
{
public:
	uint8_t m_data[4096];
public:
	LinkData(void)
	{
		for(int i = 0; i < sizeof(m_data); i++)
		{
			m_data[i] = 0;
		}
	}
	bool check(const LinkLength& length)
	{
		uint8_t  cs  = 0;
		uint16_t len = length.getLength();

		if( len > sizeof(m_data)-2 )
		{
			return false;
		}

		for(uint16_t i = 0; i < len; i++)
		{
			cs += m_data[i];
		}
		return (cs == m_data[len])
			&& (0x16 == m_data[len+1]);
	}
	void print(const LinkLength& length)
	{
		int len = length.getLength();

		if(len > sizeof(m_data))
		{
			return;
		}
		printf("userdata: ");
		for(int i = 0; i < len; i++)
		{
			printf("%02X ", m_data[i]);
		}
		printf("\ncs: %02X\n", m_data[len]);
		printf("tail: %02X\n", m_data[len+1]);
	}
};

class LinkFrame
{
public:
	LinkHeader	header1;
	LinkLength	length1;
	LinkLength	length2;
	LinkHeader	header2;
	LinkControl control;
	LinkAddress address;
	LinkData	linkdata;
public:
	bool check(void)
	{
		return header1.check()
			&& length1 == length2
			&& header2.check()
			&& linkdata.check(length2);
	}
	void print(void)
	{
		header1.print();
		length1.print();
		length2.print();
		header2.print();
		control.print();
		address.print();
		linkdata.print(length1);
	}
};

#endif//_3761_LINKFRAME_H_
