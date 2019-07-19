#ifndef _3761_PAYLOAD_H_
#define _3761_PAYLOAD_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma pack(1)

class PnFn
{
public:
	uint8_t  DA1;
	uint8_t  DA2;
	uint8_t  DT1;
	uint8_t  DT2;
public:
	void init(void)
	{
		DA1 = DA2 = DT1 = DT2 = 0;
	}
public:
	uint16_t getPn(void)const
	{
		uint8_t n = 0;

		switch(DA1)
		{
			case (1 << 0): n = 1; break;
			case (1 << 1): n = 2; break;
			case (1 << 2): n = 3; break;
			case (1 << 3): n = 4; break;
			case (1 << 4): n = 5; break;
			case (1 << 5): n = 6; break;
			case (1 << 6): n = 7; break;
			case (1 << 7): n = 8; break;
			case 0x00:break;
			case 0xFF:break;
		}

		return (DA2 - 1) * 8 + n;
	}
	uint16_t getFn(void)const
	{
		uint8_t n = 0;

		switch(DT1)
		{
			case (1 << 0): n = 1; break;
			case (1 << 1): n = 2; break;
			case (1 << 2): n = 3; break;
			case (1 << 3): n = 4; break;
			case (1 << 4): n = 5; break;
			case (1 << 5): n = 6; break;
			case (1 << 6): n = 7; break;
			case (1 << 7): n = 8; break;
			case 0x00:break;
			case 0xFF:break;
		}

		return (DT2 - 1) * 8 + n;
	}
public:
	void setPn(const uint16_t pn)
	{
		DA1 = (pn + 7) / 8;
		DA2 = 1 << ((pn + 7) % 8);
	}
	void setFn(const uint16_t fn)
	{
		DT1 = (fn + 7) / 8;
		DT2 = 1 << ((fn + 7) % 8);
	}
public:
	void print(void)const
	{
		printf("Pn:%02X %02X = %d\n"
			   "Fn:%02X %02X = %d\n", 
			   DA1, DA2, getPn(),
			   DT1, DT2, getFn());
	}
};

class TimeTag
{
public:
	uint8_t pfc;	//frame count
	uint8_t sec;	//Tp.BCD-code for second
	uint8_t min;	//Tp.BCD-code for minute
	uint8_t hour;	//Tp.BCD-code for hour
	uint8_t day;	//Tp.BCD-code for day
	uint8_t delay;	//frame live time
public:
	void init(void)
	{
		pfc = 0;
		sec = min = hour = day = 0;
		delay = 0;
	}
	void print(void)const
	{
		printf("Tp: %02X %02X %02X %02X %02X %02X"
			   " = {PFC=%d, Day=%02X, T=%02X:%02X:%02X, Delay=%d}\n",
			   pfc, sec, min, hour, day, delay,
			   pfc, day, hour, min, sec, delay);
	}
};


class Payload 
{
public:
	uint8_t  data[4096];
	uint16_t length;
public:
	void init(void)
	{
		length = 0;
		memset(data, 0, sizeof(data));
	}
	uint16_t maxPayload(void)const
	{
		return sizeof(data) - 2;
	}
	void print(int len)const
	{
		int l = len % maxPayload();
		printf("payload(%d):", l);
		for(int i = 0; i < l; i++)
		{
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
};

#pragma pack(0)

#endif//_3761_PAYLOAD_H_
