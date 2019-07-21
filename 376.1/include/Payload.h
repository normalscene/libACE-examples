#ifndef _3761_PAYLOAD_H_
#define _3761_PAYLOAD_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#pragma pack(1)

#define printObjectHex()\
do{\
	for(int i = 0; i < sizeof(*this); i++)\
	{\
		printf("%02X ");\
	}\
}while(0)

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
			return 0;
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
		if(pn >= PnMin && pn <= PnMax)
		{
			DA1 = 1 << (pn % 8);
			DA2 = 1 +  (pn / 8);
		}
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
		printf("%10s: ", "PnFn");
		printObjectHex();
		printf("= {P%u, F%u}%s", 
				getPn(), getFn(),
			   ((getFn() > FnMax) ? "<F_undefined>\n" : "\n"));
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

class A1_Value//T:4[14], ss:mm:hh_DD-MM-WW-YY
{
public:
	uint8_t s1:4;
	uint8_t s2:4;
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
	uint8_t M1:4;
	uint8_t M2:1;
	uint8_t WW:3;
	uint8_t Y1:4;
	uint8_t Y2:4;
public:
	uint8_t getYear(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getMonth(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getDay(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getWeek(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getHour(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getMinute(void)const
	{
		return (Y2 * 10) + Y1;
	}
	uint8_t getSecond(void)const
	{
		return (Y2 * 10) + Y1;
	}
public:
	void setYear(uint8_t v)
	{
		Y1 = v % 10;
		Y2 = v / 10;
	}
	void setMonth(uint8_t v)
	{
		M1 = v % 10;
		M2 = v / 10;
	}
	void setDay(uint8_t v)
	{
		D1 = v % 10;
		D2 = v / 10;
	}
	void setWeek(uint8_t v)
	{
		WW = v & 7;
	}
	void setHour(uint8_t v)
	{
		h1 = v % 10;
		h2 = v / 10;
	}
	void setMinute(uint8_t v)
	{
		m1 = v % 10;
		m2 = v / 10;
	}
	void setSecond(uint8_t v)
	{
		s1 = v % 10;
		s2 = v / 10;
	}
public:
	void print(void)
	{
		printf("%10s: ", "A1.time");
		printObjectHex();
		printf(" = {%d-%d-%d %d:%d:%d %d}\n",
				getYear(),
				getMonth(),
				getDay(),
				getHour(),
				getMinute(),
				getSecond(),
				getWeek());
	}
};

class A2_Value//(V:4[3], S:1, G:3), +-799 * {10K, 1K, 100, 10, 1, 0.1, 0.01, 0.001}
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t S:1;
	uint8_t G:3;
public:
	float getValue(void)const
	{
		return getG() * getV() * getS();
	}
public:
	float getG(void)const
	{
		switch(G)
		{
			case 0: return powf(10, 4);
			case 1: return powf(10, 3);
			case 2: return powf(10, 2);
			case 3: return powf(10, 1);
			case 4: return powf(10, 0);
			case 5: return powf(10, -1);
			case 6: return powf(10, -2);
			case 7: return powf(10, -3);
		}
		return 0.000000;
	}
	uint16_t getV(void)const
	{
		return (V3 * 100) + (V2 * 10) + V1;
	}
	int getS(void)const
	{
		return S ? -1 : 1;
	}
public:
	void setG(int v)
	{
		switch(v)
		{
			case 4:  G = 0; return;
			case 3:  G = 1; return;
			case 2:  G = 2; return;
			case 1:  G = 3; return;
			case 0:  G = 4; return;
			case -1: G = 5; return;
			case -2: G = 6; return;
			case -3: G = 7; return;
		}
	}
	void setS(float v)
	{
		S = (v == fabsf(v));
	}
	void setV(uint16_t v)
	{
		V1 = v % 10;
		V2 = (v / 10) % 10;
		V3 = (v / 100) % 10;
	}
public:
	void print(void)
	{
		printf("%10s: ", "A2.value");
		printObjectHex();
		printf(" = {value=%06X, S=%d, (G3=%X, G2=%X, G0=%X)=%f}\n",
				getV(), S, (G>>2)&1, (G>>1)&1, (G>>0)&1, getG());
	}
};

class A3_Value//(V:4[7], S:1, G:1) (+-7'999'999) * {1000, 1000k}
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t S:1;
	uint8_t XXX:1;
	uint8_t G:1;
	uint8_t xxx:1;
public:
	uint32_t getV(void)const
	{
		return (V1 * 1)
			+  (V2 * 10)
			+  (V3 * 100)
			+  (V4 * 1000)//K
			+  (V5 * 10000)//10K
			+  (V6 * 100000)//100K
			+  (V7 * 1000000);//1M
	}
	uint32_t getG(void)const
	{
		return G ? (1000*1000) : 1000;
	}
	int getS(void)const
	{
		return S ? -1 : 1;
	}
public:
	void setV(uint32_t v)
	{
		V1 = (v / 1) % 10;
		V2 = (v / 10) % 10;
		V3 = (v / 100) % 10;
		V4 = (v / 1000) % 10;//1k
		V5 = (v / 10000) % 10;//10k
		V6 = (v / 100000) % 10;//100k
		V7 = (v / 1000000) % 10;//1M
	}
public:
	void print(void)
	{
		printf("%10s: ", "A3.value");
		printObjectHex();
		printf(" = {value=%06X, S=%d, (G3=%X, G2=%X, G0=%X)=%f}\n",
				getV(), S, (G>>2)&1, (G>>1)&1, (G>>0)&1, getG());
	}
};

class A4_Value//(V:4[2], S:1) (+-79)
{
public:
	uint8_t V1:4;
	uint8_t V2:3;
	uint8_t S:1;
public:
	int getValue(void)const
	{
		return getV() * getS();
	}
	uint8_t getV(void)const
	{
		return (V2 * 10) + V1;
	}
	void setV(uint8_t v)
	{
		V1 = v % 10;
		V2 = v / 10;
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
	void print(void)
	{
		printf("%10s: ", "A4.value");
		printObjectHex();
		printf(" = {value=%06X, S=%d}\n", getV(), S);
				
	}
};

class A5_Value//(V:4[4], S:1) +-799.9
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:3;
	uint8_t S:1;
public:
	float getValue(void)const
	{
		return getV() * getS();
	}
	void setValue(float v)
	{
		setV(v);
		S = (v == fabsf(v));
	}
	float getV(void)const
	{
		return (V1 * 0.1)
		   	+  (V2 * 1.0)
		   	+  (V3 * 10.0)
		   	+  (V4 * 100.0);
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 10);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
	void print(void)
	{
		printf("%10s: ", "A5.value");
		printObjectHex();
		printf(" = {value=%06X, S=%d}\n", getV(), S);
				
	}
};

class A6_Value//(V:4[4], S[1])-+79.99
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:3;
	uint8_t S:1;
public:
	float getValue(void)const
	{
		return getV() * getS();
	}
	void setValue(float v)
	{
		setV(v);
		S = (v == fabsf(v));
	}
	float getV(void)const
	{
		return (V1 * 0.01)
		   	+  (V2 * 0.10)
		   	+  (V3 * 1.00)
		   	+  (V4 * 10.0);
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 100);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
	void print(void)
	{
		printf("%10s: ", "A6.value");
		printObjectHex();
		printf(" = {value=%.2f, S=%d}\n", getV(), S);
				
	}
};

class A7_Value//V:4[4], 999.9
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
public:
	float getV(void)const
	{
		return (V1 * 0.1)
		   	+  (V2 * 1.0)
		   	+  (V3 * 10.0)
		   	+  (V4 * 100.0);
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 10);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A7.value");
		printObjectHex();
		printf(" = {value=%.1f}\n", getV());
				
	}
};

class A8_Value//V:4[4], 9999
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
public:
	uint32_t getV(void)const
	{
		return (V1 * 1)
		   	+  (V2 * 10)
		   	+  (V3 * 100)
		   	+  (V4 * 1000);
	}
	void setV(uint32_t v)
	{
		V1 = (v / 1) % 10;
		V2 = (v / 10) % 10;
		V3 = (v / 100) % 10;
		V4 = (v / 1000) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A8.value");
		printObjectHex();
		printf(" = {value=%u}\n", getV());
				
	}
};

class A9_Value//V:4[6], +-79.9999
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:3;
	uint8_t S:1;
public:
	float getValue(void)const
	{
		return getV() * getS();
	}
	void setValue(float v)
	{
		setV(v);
		S = (v != fabsf(v));
	}
	float getV(void)const
	{
		return (V1 * 0.0001)
		   	+  (V2 * 0.001)
		   	+  (V3 * 0.01)
		   	+  (V4 * 0.1)
		   	+  (V5 * 1)
		   	+  (V6 * 10);
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 10000);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
		V5 = (x / 10000) % 10;
		V6 = (x / 100000) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A9.value");
		printObjectHex();
		printf(" = {value=%.4f, S=%d}\n", getV(), S);
				
	}
};

class A10_Value//V:4[6] 999,999=100K
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
public:
	uint32_t getV(void)const
	{
		return (V1 * 1)
		   	+  (V2 * 10)
		   	+  (V3 * 100)
		   	+  (V4 * 1000)//k
		   	+  (V5 * 10000)//10k
		   	+  (V6 * 100000);//100k
	}
	void setV(uint32_t v)
	{
		V1 = (v / 1) % 10;
		V2 = (v / 10) % 10;
		V3 = (v / 100) % 10;
		V4 = (v / 1000) % 10;//k
		V5 = (v / 10000) % 10;//10k
		V6 = (v / 100000) % 10;//100k
	}
	void print(void)
	{
		printf("%10s: ", "A10.value");
		printObjectHex();
		printf(" = {value=%u}\n", getV());
				
	}
};

class A11_Value//V:4[8] 999'999.99 = 100K.99
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t V8:4;
public:
	float getV(void)const
	{
		return (V1 * 0.01)
		   	+  (V2 * 0.10)
		   	+  (V3 * 1)
		   	+  (V4 * 10)
		   	+  (V5 * 100)
		   	+  (V6 * 1000)//k
		   	+  (V7 * 10000)//10k
		   	+  (V8 * 100000);//100k
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 100);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;//k
		V5 = (x / 10000) % 10;//10k
		V6 = (x / 100000) % 10;//100k
		V7 = (x / 1000000) % 10;//1M
		V8 = (x / 10000000) % 10;//10M
	}
	void print(void)
	{
		printf("%10s: ", "A11.value");
		printObjectHex();
		printf(" = {value=%.2f}\n", getV());
				
	}
};

class A12_Value//V:4[12], 999'999'999'999 = 100G
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t V8:4;
	uint8_t V9:4;
	uint8_t VA:4;
	uint8_t VB:4;
	uint8_t VC:4;
public:
	uint64_t getV(void)const
	{
		return (V1 * 1LL)
		   	+  (V2 * 10LL)
		   	+  (V3 * 100LL)
		   	+  (V4 * 1000LL)//K
		   	+  (V5 * 10000LL)//10K
		   	+  (V6 * 100000LL)//100k
		   	+  (V7 * 1000000LL)//1M
		   	+  (V8 * 10000000LL)//10M
		   	+  (V9 * 100000000LL)//100M
		   	+  (VA * 1000000000LL)//1G
		   	+  (VB * 10000000000LL)//10G
		   	+  (VC * 100000000000LL);//100G
	}
	void setV(uint64_t v)
	{
		V1 = (v / 1LL) % 10LL;
		V2 = (v / 10LL) % 10LL;
		V3 = (v / 100LL) % 10LL;
		V4 = (v / 1000LL) % 10LL;//k
		V5 = (v / 10000LL) % 10LL;//10k
		V6 = (v / 100000LL) % 10LL;//100k
		V7 = (v / 1000000LL) % 10LL;//1M
		V8 = (v / 10000000LL) % 10LL;//10M
		V9 = (v / 100000000LL) % 10LL;//100M
		VA = (v / 1000000000LL) % 10LL;//1G
		VB = (v / 10000000000LL) % 10LL;//10G
		VC = (v / 100000000000LL) % 10LL;//100G
	}
	void print(void)
	{
		printf("%10s: ", "A12.value");
		printObjectHex();
		printf(" = {value=%llu}\n", getV());
				
	}
};

class A13_Value//V:4[8] 9999.9999
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t V8:4;
public:
	float getV(void)const
	{
		return (V1 * 0.0001)
			+  (V2 * 0.001)
			+  (V3 * 0.01)
			+  (V4 * 0.1)
			+  (V5 * 1)
			+  (V6 * 10)
			+  (V7 * 100)
			+  (V8 * 1000);
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 10000);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;//k
		V5 = (x / 10000) % 10;//10k
		V6 = (x / 100000) % 10;//100k
		V7 = (x / 1000000) % 10;//1M
		V8 = (x / 10000000) % 10;//10M
	}
	void print(void)
	{
		printf("%10s: ", "A13.value");
		printObjectHex();
		printf(" = {value=%.4f}\n", getV());
				
	}
};

class A14_Value//V:4[12] 99'999'999.9999 = 100K_p9999
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t V8:4;
	uint8_t V9:4;
	uint8_t VA:4;
	uint8_t VB:4;
public:
	float getV(void)const
	{
		return (V1 * 0.0001)
			+  (V2 * 0.001)
			+  (V3 * 0.01)
			+  (V4 * 0.1)
			+  (V5 * 1)
			+  (V6 * 10)
			+  (V7 * 100)
			+  (V8 * 1000)//k
			+  (V9 * 10000)//10k
			+  (VA * 100000);//100k
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 10000);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;//k
		V5 = (x / 10000) % 10;//10k
		V6 = (x / 100000) % 10;//100k
		V7 = (x / 1000000) % 10;//1M
		V8 = (x / 10000000) % 10;//10M
		V9 = (x / 100000000) % 10;//100M
		VA = (x / 1000000000) % 10;//1G
	}
	void print(void)
	{
		printf("%10s: ", "A14.value");
		printObjectHex();
		printf(" = {value=%.4f}\n", getV());
				
	}
};

class A15_Value//t:4[10], mmhh_DDMMYY
{
public:
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
	uint8_t M1:4;
	uint8_t M2:4;
	uint8_t Y1:4;
	uint8_t Y2:4;
public:
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
	uint8_t getMonth(void)const
	{
		return (M2 * 10) + M1;
	}
	uint8_t getYear(void)const
	{
		return (Y2 * 10) + Y1;
	}
	void print(void)
	{
		printf("%10s: ", "A15.value");
		printObjectHex();
		printf(" = {%d-%d-%d %d:%d:00}\n", 
				getYear(),
				getMonth(),
				getDay(),
				getHour(),
				getMinute());
	}
};

class A16_Value//T:4[8], ssmmhh_DD
{
public:
	uint8_t s1:4;
	uint8_t s2:4;
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
public:
	uint8_t getSecond(void)const
	{
		return (s2 * 10) + s1;
	}
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
	void print(void)
	{
		printf("%10s: ", "A17.value");
		printObjectHex();
		printf(" = {day=%d %d:%d:%d}\n", 
				getDay(),
				getHour(),
				getMinute(),
				getSecond());
	}
};

class A17_Value//T:4[8], mmhh_DDMM
{
public:
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
	uint8_t M1:4;
	uint8_t M2:4;
public:
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
	uint8_t getMonth(void)const
	{
		return (M2 * 10) + M1;
	}
	void print(void)
	{
		printf("%10s: ", "A17.value");
		printObjectHex();
		printf(" = {mon=%d day=%d %d:%d:00}\n", 
				getMonth(),
				getDay(),
				getHour(),
				getMinute());
	}
};

class A18_Value//T:4[4], mmhh
{
public:
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
public:
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	void print(void)
	{
		printf("%10s: ", "A18.value");
		printObjectHex();
		printf(" = {%d:%d:00}\n", 
				getHour(),
				getMinute());
	}
};

class A19_Value//T:4[6], mmhhDD
{
public:
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
public:
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
	void print(void)
	{
		printf("%10s: ", "A18.value");
		printObjectHex();
		printf(" = {day=%d %d:%d:%d}\n", 
				getDay(),
				getHour(),
				getMinute());
	}
};

class A20_Value//T:4[6], DDMMYY
{
public:
	uint8_t D1:4;
	uint8_t D2:4;
	uint8_t M1:4;
	uint8_t M2:4;
	uint8_t Y1:4;
	uint8_t Y2:4;
public:
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
	uint8_t getMonth(void)const
	{
		return (M2 * 10) + M1;
	}
	uint8_t getYear(void)const
	{
		return (Y2 * 10) + Y1;
	}
	void print(void)
	{
		printf("%10s: ", "A20.value");
		printObjectHex();
		printf(" = {%d-%d-%d}\n", 
				getYear(),
				getMonth(),
				getDay());
	}
};

class A21_Value//T:4[4], MMYY
{
public:
	uint8_t M1:4;
	uint8_t M2:4;
	uint8_t Y1:4;
	uint8_t Y2:4;
public:
	uint8_t getMonth(void)const
	{
		return (M2 * 10) + M1;
	}
	uint8_t getYear(void)const
	{
		return (Y2 * 10) + Y1;
	}
	void print(void)
	{
		printf("%10s: ", "A21.value");
		printObjectHex();
		printf(" = {%d-%d-dd}\n", 
				getYear(),
				getMonth());
	}
};

class A22_Value//T:4[2], 99
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
public:
	void setV(uint8_t v)
	{
		V1 = v % 10;
		V2 = v / 10;
	}
	uint8_t getV(void)const
	{
		return (V2 * 10) + V1;
	}
	void print(void)
	{
		printf("%10s: ", "A21.value");
		printObjectHex();
		printf(" = {value = %d}\n", getV());
	}
};

class A23_Value//T:4[6], 99.9999
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
public:
	void setV(float v)
	{
		uint32_t x = fabsf(v);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
		V5 = (x / 10000) % 10;
		V6 = (x / 100000) % 10;
	}
	float getV(void)const
	{
		return (V1 * 0.0001)
			+  (V2 * 0.001)
			+  (V3 * 0.01)
			+  (V4 * 0.1)
			+  (V5 * 1)
			+  (V6 * 10);
	}
	void print(void)
	{
		printf("%10s: ", "A23.value");
		printObjectHex();
		printf(" = {value = %.4f}\n", getV());
	}
};

class A24_Value//T:4[4], hh_DD
{
public:
	uint8_t h1:4;
	uint8_t h2:4;
	uint8_t D1:4;
	uint8_t D2:4;
public:
	uint8_t getHour(void)const
	{
		return (h2 * 10) + h1;
	}
	uint8_t getDay(void)const
	{
		return (D2 * 10) + D1;
	}
public:
	void setHour(uint8_t v)
	{
		h1 = v % 10;
		h2 = v / 10;
	}
	void setDay(uint8_t v)
	{
		D1 = v % 10;
		D2 = v / 10;
	}
	void print(void)
	{
		printf("%10s: ", "A24.value");
		printObjectHex();
		printf(" = {hour=%d, day=%d}\n", getHour(), getDay());
	}
};

class A25_Value//V:4[6], -+799.999 
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:3;
	uint8_t S:1;
public:
	float getValue(void)const
	{
		return getV() * getS();
	}
	float getV(void)const
	{
		return (V1 * 0.001)
			+  (V2 * 0.01)
			+  (V3 * 0.1)
			+  (V4 * 1)
			+  (V5 * 10)
			+  (V6 * 100);
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
public:
	void setValue(float v)
	{
		setV(v);
		S = (v == fabs(v));
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 1000);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
		V5 = (x / 10000) % 10;
		V6 = (x / 100000) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A25.value");
		printObjectHex();
		printf(" = {V = %.3f, S = %d}\n", getV(), S);
	}
};

class A26_Value//V:4[4], -+7.999 
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:3;
	uint8_t S:1;
public:
	float getValue(void)const
	{
		return getV() * getS();
	}
	float getV(void)const
	{
		return (V1 * 0.001)
			+  (V2 * 0.01)
			+  (V3 * 0.1)
			+  (V4 * 1);
	}
	int getS(void)const
	{
		return S ? 1 : -1;
	}
public:
	void setValue(float v)
	{
		setV(v);
		S = (v == fabs(v));
	}
	void setV(float v)
	{
		uint32_t x = fabsf(v * 1000);

		V1 = (x / 1) % 10;
		V2 = (x / 10) % 10;
		V3 = (x / 100) % 10;
		V4 = (x / 1000) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A26.value");
		printObjectHex();
		printf(" = {V = %.3f, S = %d}\n", getV(), S);
	}
};

class A27_Value//V:4[8], 99'999'999 
{
public:
	uint8_t V1:4;
	uint8_t V2:4;
	uint8_t V3:4;
	uint8_t V4:4;
	uint8_t V5:4;
	uint8_t V6:4;
	uint8_t V7:4;
	uint8_t V8:4;
public:
	uint32_t getValue(void)const
	{
		return getV();
	}
	uint32_t getV(void)const
	{
		return (V1 * 1)
			+  (V2 * 10)
			+  (V3 * 100)
			+  (V4 * 1000)//k
			+  (V5 * 10000)//10k
			+  (V6 * 100000)//100k
			+  (V7 * 1000000)//1M
			+  (V8 * 10000000);//10M;
	}
public:
	void setValue(uint32_t v)
	{
		setV(v);
	}
	void setV(uint32_t v)
	{
		V1 = (v / 1) % 10;
		V2 = (v / 10) % 10;
		V3 = (v / 100) % 10;
		V4 = (v / 1000) % 10;//k
		V5 = (v / 10000) % 10;//10k
		V6 = (v / 100000) % 10;//100k
		V7 = (v / 1000000) % 10;//1M
		V8 = (v / 10000000) % 10;//10M
	}
	void print(void)
	{
		printf("%10s: ", "A26.value");
		printObjectHex();
		printf(" = {V = %u}\n", getV());
	}
};

class A28_Value//T:4[7], Fwww_mmss 
{
public:
	uint8_t s1:4;
	uint8_t s2:4;
	uint8_t m1:4;
	uint8_t m2:4;
	uint8_t w1:4;
	uint8_t w2:4;
	uint8_t w3:4;
	uint8_t R:3;
	uint8_t F:1;
public:
	uint8_t getSecond(void)const
	{
		return (s2 * 10) + s1;
	}
	uint8_t getMinute(void)const
	{
		return (m2 * 10) + m1;
	}
	uint8_t getWeidu(void)const
	{
		return (w1 * 1)
			+  (w2 * 10)
			+  (w3 * 100);
	}
public:
	void setSecond(uint8_t v)
	{
		s1 = v % 10;
		s2 = v / 10;
	}
	void setMinute(uint8_t v)
	{
		m1 = v % 10;
		m2 = v / 10;
	}
	void setWeidu(uint8_t v)
	{
		w1 = (v / 1) % 10;
		w2 = (v / 10) % 10;
		w3 = (v / 100) % 10;
	}
	void print(void)
	{
		printf("%10s: ", "A28.value");
		printObjectHex();
		printf(" = {second=%d, minute=%u, weidu=%d}\n", 
				getSecond(),
				getMinute(),
				getWeidu());
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
