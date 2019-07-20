#ifndef _3761_PAYLOAD_H_
#define _3761_PAYLOAD_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma pack(1)

class DataPoint
{
public:
	uint8_t  DA1;
	uint8_t  DA2;
	uint8_t  DT1;
	uint8_t  DT2;
public:
	void init(void)
	{
		setPn(0);
		setFn(FnMin);
	}
public:
	uint16_t getPn(void)const
	{
		uint8_t n = (uint8_t)PnUnknown;

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
			case 0xFF:return (DA2 == 0) ? PnAll : PnUnknown;
			case 0x00:return (DA2 == 0) ? Pn0 : PnUnknown;
		}
		if(DA2 < 1)
		{
			return PnUnknown;
		}

		return (DA2 - 1) * 8 + n;
	}
	uint16_t getFn(void)const
	{
		uint8_t n = FnMax + 1;

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
		}

		return (DT2 * 8) + n;
	}
public:
	void setPn(const uint16_t pn)
	{
		switch(pn)
		{
			case Pn0:
				DA1 = 0;
				DA2 = 0;
				return;
				break;
			case PnAll:
				DA1 = 0xFF;
				DA2 = 0x00;
				return;
				break;
		}
		DA1 = 1 << (pn % 8);
		DA2 = 1 +  (pn / 8);
	}
	void setFn(const uint8_t fn)
	{
		uint8_t n = FnMin;

		if(fn < FnMin)
		{
			n = FnMin;
		}

		DT1 = 1 << ((n-1) % 8);
		DT2 = 0  + (n / 8);
	}
public:
	void print(void)const
	{
		printf("%10s: %02X %02X = %d\n"
			   "%10s: %02X %02X = %d%s", 
			   "Pn", DA1, DA2, getPn(),
			   "Fn", DT1, DT2, getFn(),
			   ((getFn() > FnMax) ? "<undefined>\n" : "\n"));
	}
public:
	enum PnType
	{
		FnMin = 1,
		FnMax = 248,
	};
	enum FnType
	{
		Pn0   = 0,
		PnMin = 1,
		PnMax = 2040,
		PnAll = 0xFF00,
		PnUnknown,
	};
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
		printf("%10s: %02X %02X %02X %02X %02X %02X = "
			   "{PFC=%d, Day=%02X, T=%02X:%02X:%02X, Delay=%d}\n",
			   "Tp",
			   pfc, sec, min, hour, day, delay,
			   pfc, day, hour, min, sec, delay);
	}
};

class Payload 
{
public:
	uint8_t  data[8192];
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
	DataPoint& dataPoint(uint16_t pos)
	{
		return *(DataPoint*)
			&data[pos % (sizeof(data)-sizeof(DataPoint))];
	}
	const DataPoint& getDataPoint(uint16_t pos)const
	{
		return *(const DataPoint*)
			&data[pos % (sizeof(data)-sizeof(DataPoint))];
	}
	void print(int len)const
	{
		int l = len % maxPayload();
		printf("%10s: <len=%d>", "payload", l);
		for(int i = 0; i < l; i++)
		{
			if((i + 0) % 32 == 0)
			{
				printf("\n%12s", " ");
			}
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
};

#pragma pack(0)

#endif//_3761_PAYLOAD_H_
