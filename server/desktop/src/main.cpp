#include "stdafx.h"
#include "main.h"
#include "common\file.h"
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
			printf("%s\n",error.what());
			failCount++;
		}
	}

	return 1;
}
