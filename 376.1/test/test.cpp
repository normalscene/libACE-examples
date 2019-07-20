#include "AFN01/F01.h"


extern uint8_t g_frame1[];
extern uint8_t g_login[];


int main(int argc, char **argv)
{
	Frame *frame = (Frame*)g_frame1;

	frame->printHex();
	frame->print();

	frame = (Frame*)g_login;

	frame->printHex();
	frame->print();


	AFN01::F01 login;

	login.printHex();
	login.print();

	return 0;
}


