
#include"server2.h"
#include"iostream"
#include"thread"

using namespace std;

ServerNet serverNet1;
ServerNet serverNet2;
ServerNet serverNet3;
ServerNet serverNet4;
ServerNet serverNet5;
ServerNet serverNet6;
ServerNet serverNet7;
ServerNet serverNet8;
ServerNet serverNet9;
ServerNet serverNet10;

void ServerRun1()
{
	serverNet1.ServerRun();
}

void ServerRun2()
{
	serverNet2.ServerRun();
}

void ServerRun3()
{
	serverNet3.ServerRun();
}

void ServerRun4()
{
	serverNet4.ServerRun();
}

void ServerRun5()
{
	serverNet5.ServerRun();
}

void ServerRun6()
{
	serverNet6.ServerRun();
}

void ServerRun7()
{
	serverNet7.ServerRun();
}

void ServerRun8()
{
	serverNet8.ServerRun();
}

void ServerRun9()
{
	serverNet9.ServerRun();
}

void ServerRun10()
{
	serverNet10.ServerRun();
}

int main()

{

	int iRlt = serverNet1.ServerInit("127.0.0.1", 8001);

	iRlt = serverNet2.ServerInit("127.0.0.1", 8002);

	iRlt = serverNet3.ServerInit("127.0.0.1", 8003);

	iRlt = serverNet4.ServerInit("127.0.0.1", 8004);

	iRlt = serverNet5.ServerInit("127.0.0.1", 8005);

	iRlt = serverNet6.ServerInit("127.0.0.1", 8006);

	iRlt = serverNet7.ServerInit("127.0.0.1", 8007);

	iRlt = serverNet8.ServerInit("127.0.0.1", 8008);

	iRlt = serverNet9.ServerInit("127.0.0.1", 8009);

	iRlt = serverNet10.ServerInit("127.0.0.1", 8010);

	if (iRlt == 0)

	{

		printf("server init successful.\n");

		thread th1(ServerRun1);
		thread th2(ServerRun2);
		thread th3(ServerRun3);
		thread th4(ServerRun4);
		thread th5(ServerRun5);
		thread th6(ServerRun6);
		thread th7(ServerRun7);
		thread th8(ServerRun8);
		thread th9(ServerRun9);
		thread th10(ServerRun10);


		th1.join();
		th2.join();
		th3.join();
		th4.join();
		th5.join();
		th6.join();
		th7.join();
		th8.join();
		th9.join();
		th10.join();

		//serverNet1.ServerRun();
		//serverNet2.ServerRun();

	}

	else

		printf("server init failed with error: %d\n", iRlt);

	system("pause");

}
