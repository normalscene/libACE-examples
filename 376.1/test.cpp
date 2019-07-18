#include "LinkFrame.h"

int main(int argc, char **argv)
{
	LinkFrame frame;

	frame.control.DIR = 1;
	frame.control.PRM = 1;
	frame.print();

	printf("frame:");
	for(int i = 0; i < 32; i++)
	{
		printf("%02X ", ((uint8_t*)&frame)[i]);
	}
	printf("\n");

	return 0;
}
