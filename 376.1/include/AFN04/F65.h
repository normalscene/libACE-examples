#ifndef _3761_AFN04_F65_H_
#define _3761_AFN04_F65_H_
#include <stdint.h>
#include "../Frame.h"

namespace AFN04
{

	class UploadCycle
	{
	public:
		uint8_t cycle:6;
		uint8_t type:2;
	public:
		void print(void)
		{
			printf("%10s: %02x {type=%d, cycle=%d}\n",
					"uploadCycle", type, cycle);
		}
	};
	class UploadTime
	{
	};
	class F65 : public Frame
	{
	public:
		F65(void)
		{
		}
		void init(uint16_t region, uint16_t terminal, uint8_t station)
		{
			linkHeader.init();
			userHeader.init();
			payload.init();

			userHeader.control.DIR = Control::DIR_S2M;
			userHeader.control.PRM = Control::PRM_1;
			userHeader.control.LFN = 9;

			userHeader.address.setRegion(region);
			userHeader.address.setTerminal(terminal);
			userHeader.address.setStation(station);

			userHeader.function.AFN = 2;

			userHeader.sequence.FRS = 1;
			userHeader.sequence.FIN = 1;

			DataPoint &point = payload.dataPoint(0);

			point.setPn(0);
			point.setFn(1);

			payload.length += sizeof(point);

			linkHeader.setLength(sizeof(UserHeader) + payload.length);
		}
	};
};

#endif//_3761_AFN04_F65_H_
