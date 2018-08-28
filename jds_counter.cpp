#include "jds_counter.hpp"

#include <cstdio>
#include <cstring>

namespace jds
{

//------------------------------------------------------------------------------
counter::counter()
{
	init();
}

//------------------------------------------------------------------------------
void counter::init()
{
#ifndef _WIN32
	clock_gettime(CLOCK_MONOTONIC, &(previous));
	clock_gettime(CLOCK_MONOTONIC, &(current));
	delta.tv_nsec = 0;
	delta.tv_sec = 0;
#else
	if(!QueryPerformanceFrequency(&(frequency)))
	{
		printf("High performance counter not available in this OS!\n");
		return;
	}
	QueryPerformanceCounter(&(previous));
	QueryPerformanceCounter(&(current));
#endif
}

//------------------------------------------------------------------------------
double counter::update() // returns delta t in seconds
{
	double delta_t = 0.0;
	#ifndef _WIN32
		memcpy(&(previous), &(current), sizeof(struct timespec));

		clock_gettime(CLOCK_MONOTONIC, &(current));

		delta.tv_sec = current.tv_sec - previous.tv_sec;
		delta.tv_nsec = current.tv_nsec - previous.tv_nsec;

		delta_t = (double)delta.tv_sec + (double)delta.tv_nsec / 1000000000.0;
	#else
		memcpy(&(previous), &(current), sizeof(LARGE_INTEGER));

		QueryPerformanceCounter(&(current));

		delta_t = (double)(current.QuadPart - previous.QuadPart) /
				  (double)frequency.QuadPart;
	#endif
		return delta_t;
}

//------------------------------------------------------------------------------
}
