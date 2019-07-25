#ifndef _3761_FRAME_H_
#define _3761_FRAME_H_
#include <stdio.h>

#pragma pack(1)
class Header
{
public:
	uint8_t H1;
	uint16_t V1:2;
	uint16_t L1:14;
	uint16_t V2:2;
	uint16_t L2:14;
	uint8_t H2;
};
class ControlHeader
{
public:
	uint8_t LFN:4;
	uint8_t FCV:1;
	uint8_t FCB:1;
	uint8_t PRM:1;
	uint8_t DIR:1;
};
class AddressHeader
{
public:
	uint16_t region;
	uint16_t terminal;
	uint8_t  tream:1;
	uint8_t  master:7;
};
class AfnHeader
{
	uint8_t AFN;
};
class SeqHeader
{
public:
	uint8_t SEQ:4;
	uint8_t CON:1;
	uint8_t FIN:1;
	uint8_t FIR:1;
	uint8_t TpV:1;
};

class CommonHeader
{
public:
	//Header
	uint8_t  H1;
	uint16_t V1:2;
	uint16_t L1:14;
	uint16_t V2:2;
	uint16_t L2:14;
	uint8_t  H2;
	//Control
	uint8_t LFN:4;
	uint8_t FCV:1;
	uint8_t FCB:1;
	uint8_t PRM:1;
	uint8_t DIR:1;
	//Address
	uint16_t region;
	uint16_t terminal;
	uint8_t  team:1;
	uint8_t  master:7;
	//AFN
	uint8_t AFN;
	//SEQ
	uint8_t SEQ:4;
	uint8_t CON:1;
	uint8_t FIN:1;
	uint8_t FIR:1;
	uint8_t TpV:1;
public:
	void print(void)
	{
		printf("L=%d, DIR=%d, PRM=%d, FCB=%d, FCV=%d,"
			   "region=%04X, terminal=%d, master=%d,"
			   "team=%d, AFN=%02X, TpV=%d, FIR=%d,"
			   "FIN=%d,CON=%d,SEQ=%d\n",
			   L1, DIR, PRM, FCB, FCV, region, terminal, 
			   master, team, AFN, TpV, FIR, FIN, CON, SEQ);
	}
};
#pragma pack(0)

#endif//_3761_FRAME_H_
