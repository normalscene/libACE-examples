#ifndef _3761_AFN05_F31_H_
#define _3761_AFN05_F31_H_
#include "Frame.h"


namespace AFN05
{
	class TimeDate
	{
	public:
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t mon:5;
		uint8_t week:3;
		uint8_t year;
	public:
		void print(void)const
		{
			uint8_t *p = (uint8_t*)this;
			printf("%10s: %02X %02X %02X %02X %02X %02X = "
				   "{%02X/%02X/%02X %02X:%02X:%02X, week=%d}\n",
				   "time", p[0], p[1], p[2], p[3], p[4], p[5],
				   year, mon, day, hour, min, sec, week);
		}
	};

	class PassWord
	{
	public:
		uint8_t data[16];
	public:
		void print(void)const
		{
			printf("%10s: ", "password");
			for(int i = 0; i < sizeof(data); i++)
			{
				printf("%02x ", data[i]);
			}
			printf("\n");
		}
	};

	class F31 :public Frame
	{
	public:
		DataPoint	*dataPoint;
		TimeDate	*timeDate;
		PassWord	*password;
	public:
		F31(void):dataPoint(0),timeDate(0),password(0)
		{
		}
	public:
		void setPointers(void)
		{
			uint8_t *p = (uint8_t*)&payload;

			dataPoint = (DataPoint*)p;
			p += sizeof(DataPoint);
			timeDate = (TimeDate*)p;
			p += sizeof(TimeDate);
			password = (PassWord*)p;
		}

		void print(void)
		{
			setPointers();
			linkHeader.print();
			userHeader.print();
			dataPoint->print();
			timeDate->print();
			password->print();
			getLinkTail().print();
		}
	};
};




#endif//_3761_AFN05_F31_H_
