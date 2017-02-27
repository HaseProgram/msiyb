#include "stdafx.h"
#include "main.h"

using namespace std;

int main()
{
	auto_ptr<Server> serverInstance(new Server);
	
	int failCount = 0;
	while (failCount < 5)
	{
		try
		{
			serverInstance->Start();
			return 0;
		}
		catch (Exception& error)
		{
			/*
				Error output
			*/

			failCount++;
		}
	}
	
	return 1;
}
