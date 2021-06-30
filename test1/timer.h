#pragma once
#pragma once

#include<Windows.h>

class Timer
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nCurrentTime;
public:
	Timer();

	LARGE_INTEGER begin();
	LARGE_INTEGER begin(LARGE_INTEGER);
	LARGE_INTEGER getCurrentTime();
	int timeCount();//返回自开始以来的时间间隔，输出单位为100us

};

