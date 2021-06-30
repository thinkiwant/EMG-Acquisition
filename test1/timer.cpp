#include "timer.h"


Timer::Timer() {
	QueryPerformanceFrequency(&nFreq);
}
LARGE_INTEGER Timer::begin() {
	QueryPerformanceCounter(&nBeginTime);
	return nBeginTime;
}

LARGE_INTEGER Timer::begin(LARGE_INTEGER t) {
	return nBeginTime = t;
}

LARGE_INTEGER Timer::getCurrentTime() {
	QueryPerformanceCounter(&nCurrentTime);
	return nCurrentTime;
}

int Timer::timeCount() { 
	QueryPerformanceCounter(&nCurrentTime);
	return ((double)(nCurrentTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart) * 10000;
}