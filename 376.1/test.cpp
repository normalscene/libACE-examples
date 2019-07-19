#include "AFN01/AFN01F01.h"


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


	AFN01F01 afn01f01;

	afn01f01.init();
	afn01f01.make();
	afn01f01.printHex();
	afn01f01.print();

	return 0;
}


