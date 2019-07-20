#ifndef _3761_AFN00_F01_H_
#define _3761_AFN00_F01_H_
#include <stdint.h>
#include "../Frame.h"


namespace AFN00
{
	class F01 : public Frame
	{
	public:
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
	};
};

#endif//_3761_AFN00_F01_H_
