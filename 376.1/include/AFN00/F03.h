#ifndef _3761_AFN00_F03_H_
#define _3761_AFN00_F03_H_
#include <stdint.h>
#include "../Frame.h"


namespace AFN00
{
	class OneByOne
	{
	public:
		uint8_t		AFN;
		DataPoint	dataPoint;
		uint8_t		errorCode;
	public:
		void print(void)
		{
			printf("%10s: %02X\n", "1By1.AFN", AFN);
			dataPoint.print();
			printf("%10s: %02X\n", "errorCode", errorCode);
		}
	};	

	class F03 : public Frame
	{
	public:
		DataPoint* dataPoint;
		OneByOne* oneByOne;

	public:
		void setPointers(void)
		{
			uint8_t *p = (uint8_t*)&payload;

			dataPoint = (DataPoint*)p;
			p += sizeof(DataPoint);
			oneByOne = (OneByOne*)p;
		}
		void init(uint16_t region, uint16_t terminal, uint8_t station)
		{
			linkHeader.init();
			userHeader.init();
			payload.init();

			userHeader.control.DIR = Control::DIR_S2M;
			userHeader.control.PRM = Control::PRM_0;
			userHeader.control.LFN = 11;

			userHeader.address.setRegion(region);
			userHeader.address.setTerminal(terminal);
			userHeader.address.setStation(station);

			userHeader.function.AFN = 00;
			userHeader.sequence.FRS = 1;
			userHeader.sequence.FIN = 1;

			DataPoint &point = payload.dataPoint(0);

			point.setPn(0);
			point.setFn(1);

			payload.length += sizeof(point);

			linkHeader.setLength(sizeof(UserHeader) + payload.length);
		}
		void print(void)
		{
			setPointers();
			linkHeader.print();
			userHeader.print();
			dataPoint[0].print();
			oneByOne[0].print();
			getLinkTail().print();
		}
	};
};

#endif//_3761_AFN00_F03_H_
