#include "Includes.h"
#include "MainServer.h"

int main()
{
	MainServer S;
	if (false == S.Initialize())
		return -1;
	S.Running();
}
