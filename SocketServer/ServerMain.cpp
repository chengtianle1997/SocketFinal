
#include"ServerNet.h"
#include"iostream"
#include"thread"

using namespace std;

ServerNet serverNet1;

ServerNet serverNet2;

void ServerRun1()
{
	serverNet1.ServerRun();
}

void ServerRun2()
{
	serverNet2.ServerRun();
}

int main()

{

	int iRlt = serverNet1.ServerInit("127.0.0.1", 8886);

	iRlt = serverNet2.ServerInit("127.0.0.1", 8888);

	if (iRlt == 0)

	{

		printf("server init successful.\n");

		thread th1(ServerRun1);
		thread th2(ServerRun2);

		th1.join();
		th2.join();
		//serverNet1.ServerRun();
		//serverNet2.ServerRun();

	}

	else

		printf("server init failed with error: %d\n", iRlt);

	system("pause");

}
