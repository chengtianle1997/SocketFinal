#include "MultSocketServer.h"

IocpAccept iocaccept;

int main()
{
	//iocaccept.SetCallback

	iocaccept.AddListenPort(8881);
	iocaccept.AddListenPort(8882);
	iocaccept.AddListenPort(8888);

	iocaccept.Start();
	//iocaccept.Stop();
	return 0;
}