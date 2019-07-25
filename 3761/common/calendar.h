#ifndef _CALENDER_H_
#define _CALENDER_H_
#include <time.h>
#include <string>

class Calendar
{
protected:
	struct tm now;
public:
	Calendar(void)
	{
		update();
		print();
	}
	bool update(void)
	{
		time_t t = time(0);
		return !!gmtime_r(&t, &now);
	}
	int second(void)const
	{
		return now.tm_sec;
	}
	int minute(void)const
	{
		return now.tm_min;
	}
	int hour(void)const
	{
		return now.tm_hour;
	}
	int day(void)const
	{
		return now.tm_mday;
	}
	int weekday(void)const
	{
		return now.tm_wday;
	}
	int monthday(void)const
	{
		return now.tm_mday;
	}
	int yearday(void)const
	{
		return now.tm_yday;
	}
	int month(void)const
	{
		return now.tm_mon + 1;
	}
	int year(void)const
	{
		return now.tm_year - 100;
	}
	void print(void)
	{
		printf("calendar:%d-%d-%d %d:%d:%d\n",
				year(),
				month(),
				day(),
				hour(),
				minute(),
				second());
	}
};

#endif//_CALENDER_H_
