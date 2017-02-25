/*
	TODO: logger.h
*/
#include "inc/exception.h"
#include "server.h"

int main()
{
	Server* serverInstance = new Server;
	
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