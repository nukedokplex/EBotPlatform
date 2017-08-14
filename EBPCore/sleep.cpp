#include <unistd.h>
void Sleep(float s)
{
	int sec = int(s*1000);
	usleep(sec);
}