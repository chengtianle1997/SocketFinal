
#include<iostream>

#include<string>

#include"ClientNet.h"



using namespace std;

typedef struct {

	int CameraNum;
	int Framecnt;
	float Res[4096];

}SocketPackage;

SocketPackage Sockpack;


int main()

{

	ClientNet client;

	int rlt = 0;

	int size = 2048;

	string msg;

	//memset(msg, 0, sizeof(msg));

	//SocketPackage Sockpack;

	Sockpack.CameraNum = 1;
	Sockpack.Framecnt = 1;
	//Sockpack.Res = new float(size);

	for (int i = 0; i < 2048; i++)
		Sockpack.Res[i] = 123.0;

	char* buf = (char*)malloc(sizeof(SocketPackage));

	memcpy(buf, &Sockpack, sizeof(SocketPackage));

	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����

	printf("connecting.....\n");

	rlt = client.ClientConnect(8888, "127.0.0.1");




	if (rlt == 0)

		// socket���ӳɹ�

	{

		//������Ϣ

		//printf("	connect successfully. input  q to quit\n");

		//printf("-------------------------\n");

		//while (1)

		//{

			//printf("msg input: ");

			//getline(cin, msg);

			//if (msg == "q")

				//break;

			//else

			//{

		printf("sending msg.....\n");



		rlt = client.ClientSend(buf, sizeof(Sockpack));

		//}



	//}





	// �ر�socket

		printf("closing socket.....\n");

		client.ClientClose();





	}



	system("pause");



	return 0;



}
