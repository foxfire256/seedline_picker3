#ifndef JDSCOUNTER_H
#define JDSCOUNTER_H

#ifndef _WIN32
#include <time.h>
#include <unistd.h>
#include <sys/time.h> // link -lrt
#else
#include <windows.h> // link -lkernel32
#endif

namespace jds
{

class counter
{
public:
	counter();
	void init();
	double update(); // returns delta t in seconds

private:
#ifndef _WIN32
	struct timespec previous, current, delta;
#else
	LARGE_INTEGER frequency, previous, current;
#endif
};

}

#endif // JDSCOUNTER_H
