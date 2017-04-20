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
#ifdef CLIENT
			setlocale(LC_CTYPE, ".1251");
			byte *arr = new byte[6000000];
			char tmp[] = "hello";
			Socket *server = new Socket(2345, "127.0.0.1");
			server->Connect();
			server->Send(tmp, sizeof(tmp));

			size_lt cl = server->RecvAll((char*)arr);
			File::WriteAllBytes("D:\\3.jpg", arr, cl);
#elif SERVER
			byte* arr;
			size_lt s = File::ReadAllBytes("D:\\1.jpg", &arr);
			Socket *server = new Socket(2345, "127.0.0.1");
			char tmp[200];
			server->Bind();
			server->Listen(10);
			Socket *client = (Socket*)server->AcceptSocket();
			client->Recv(tmp, 200);
			if (!strcmp(tmp, "hello"))
				size_lt cl = client->SendAll((char*)arr, s);
#endif
			return 0;
			//serverInstance->Start();
			//serverInstance->Start();
		}
		catch (Exception &error)
		{
			FileException *f = (FileException*)&error;
			printf("%s\n %s", f->what(), ParseException(f->GetErrorCode()));
			failCount++;
		}
	}

	return 1;
}
