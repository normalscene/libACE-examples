#ifndef _3761_AFN01_F01_H_
#define _3761_AFN01_F01_H_
#include <stdint.h>
#include "../Frame.h"


class AFN01F01 : public Frame
{
public:
	void init(void)
	{
		linkHeader.init();
		userHeader.init();
		userHeader.control.DIR = Control::DIR_S2M;
		userHeader.control.PRM = Control::PRM_1;
		userHeader.sequence.init();
		userHeader.sequence.FRS = 1;
		userHeader.sequence.FIN = 1;
		userHeader.sequence.CON = 1;
		linkHeader.setLength(sizeof(UserHeader));
	}

};

#endif//_3761_AFN01_F01_H_
