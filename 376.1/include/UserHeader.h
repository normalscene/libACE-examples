#ifndef _3761_APPHEADER_H_
#define _3761_APPHEADER_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#pragma pack(1)

class Control
{
public:
	uint8_t LFN:4;
	uint8_t FCV:1;
	uint8_t FBC:1;
	uint8_t PRM:1;
	uint8_t DIR:1;

public:
	enum//DIR
	{
		DIR_M2S = 0,
		DIR_S2M = 1,
	};
	enum//PRM
	{
		PRM_0 = 0,
		PRM_1 = 1,
	};
	enum//PRM0.LFN
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
	enum//PRM1.LFN
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
	void init(void)
	{
		*(uint8_t*)this = 0;
	}
	void print(void)const
	{
		printf("%10s: %02X    = "
			   "{DIR=%d, PRM=%d, FBC=%d FCV=%d, LFN=%d}\n", 
			   "control", 
			   getControl(), 
			   DIR, 
			   PRM, 
			   FBC, 
			   FCV, 
			   LFN);
	}
	void setControl(const uint8_t v)
	{
		*((uint8_t*)this) = v;
	}
	const uint8_t getControl(void)const
	{
		return *((uint8_t*)this);
	}
	uint8_t size(void)const
	{
		return sizeof(*this);
	}
};

class Address
{
public:
	uint8_t region[2];
	uint8_t terminal[2];
	uint8_t station[1];

public:
	void init(void)
	{
		region[0]	= 0;
		region[1]	= 0;
		terminal[0]	= 0;
		terminal[1]	= 0;
		station[0]	= 0;
	}
	void setRegion(const char s[])
	{
		setRegion(strtol(s ? s : "0", 0, 16));	
	}
	void setRegion(const uint16_t v)
	{
		region[0] = 0xff & (v);
		region[1] = 0xff & (v >> 8);
	}
	const uint16_t getRegion(void)const
	{
		return (region[1] << 8) | region[0];
	}
	void printRegion(void)const
	{
		printf("%10s: %02X %02X = %02X%02X\n",
			   "region",
			   region[0],
			   region[1],
			   region[1],
			   region[0]);
	}
public:
	void setTerminal(const uint16_t v)
	{
		terminal[0] = 0xff & v;
		terminal[1] = 0xff & (v >> 8);
	}
	const uint16_t getTerminal(void)const
	{
		return (terminal[1] << 8) | terminal[0];
	}
	void printTerminal(void)const
	{
		printf("%10s: %02X %02X = %d\n",
			   "termial",
			   terminal[0],
			   terminal[1],
			   getTerminal());
	}
public:
	void setStation(const uint8_t v)
	{
		station[0] = v;
	}
	const uint8_t getStation(void)const
	{
		return station[0];
	}
	void printStation(void)const
	{
		printf("%10s: %02X    = %d\n",
			   "station",
			   station[0],
			   getStation());
	}
public:
	void print(void)const
	{
		printRegion();
		printTerminal();
		printStation();
	}
	uint8_t size(void)const
	{
		return sizeof(region)
			+  sizeof(terminal)
			+  sizeof(station);
	}
};

class Funtion
{
public:
	uint8_t AFN;
public:
	void init(void)
	{
		AFN = 0;
	}
	const uint8_t getFunction(void)const
	{
		return AFN ;
	}
	void setFunction(uint8_t v)
	{
		AFN = v;
	}
	void print(void)const
	{
		printf("%10s: %02X\n", "AFN", AFN);
	}
	const uint8_t size(void)const
	{
		return sizeof(*this);
	}
public:
	enum
	{
		AFN_0 = 0,
		AFN_1 = 1,
		AFN_2 = 2,
		AFN_3 = 3,
		AFN_4 = 4,
		AFN_5 = 5,
		AFN_6 = 6,
		AFN_7 = 7,
		AFN_8 = 8,
		AFN_9 = 9,
		AFN_A = 10,
		AFN_B = 11,
		AFN_C = 12,
		AFN_D = 13,
		AFN_E = 14,
		AFN_F = 15,
	};
};

class Sequence
{
public:
	uint8_t SEQ:4;
	uint8_t CON:1;
	uint8_t FIN:1;
	uint8_t FRS:1;
	uint8_t TpV:1;

public:
	void init(void)
	{
		*(uint8_t*)this = 0;
	}
	const void print(void)const
	{
		printf("%10s: %02X    = "
			   "{TpV=%d, FRS=%d, FIN=%d, CON=%d, PSEQ/RSEQ=%d}\n",
			   "SEQ", *(uint8_t*)this, TpV, FRS, FIN, CON, SEQ);
	}
	const uint8_t size(void)const
	{
		return sizeof(*this);
	}
};

class UserHeader
{
public:
	Control		control;
	Address		address;
	Funtion		function;
	Sequence	sequence;

public:
	UserHeader(void)
	{
		init();
	}

public:
	void init(void)
	{
		control.init();
		address.init();
		function.init();
		sequence.init();
	}
	void print(void)const
	{
		control.print();
		address.print();
		function.print();
		sequence.print();
	}
};

#pragma pack(0)

#endif//_3761_APPHEADER_H_
