
/*

ServerNet.cpp

*/



#include "MSServer.h"



/*��������ʼ��*/

int ServerNet::ServerInit(const char* address, int port1 , int port2)

{

	int rlt = 0;



	int iErrorMsg;



	//��ʼ��WinSock

	WSAData wsaData;

	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);



	if (iErrorMsg != NO_ERROR)

	{

		//��ʼ��WinSockʧ��

		printf("server wsastartup failed with error : %d\n", iErrorMsg);



		rlt = 1;

		return rlt;

	}



	// ������������socket



	m_sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_sock1 == INVALID_SOCKET)

		// ����socket�������쳣

	{

		printf("server socket failed with error: %d\n", WSAGetLastError());

		rlt = 2;

		return rlt;

	}

	m_sock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_sock2 == INVALID_SOCKET)

		// ����socket�������쳣

	{

		printf("server socket failed with error: %d\n", WSAGetLastError());

		rlt = 2;

		return rlt;

	}


	// ������Ϣ

	SOCKADDR_IN	servaddr1;
	
	servaddr1.sin_family = AF_INET;

	servaddr1.sin_port = port1;

	servaddr1.sin_addr.s_addr = inet_addr(address);

	SOCKADDR_IN	servaddr2;

	servaddr2.sin_family = AF_INET;

	servaddr2.sin_port = port2;

	servaddr2.sin_addr.s_addr = inet_addr(address);

	//��

	iErrorMsg = bind(m_sock1, (SOCKADDR*)&servaddr1, sizeof(servaddr1));
	

	if (iErrorMsg < 0)

	{

		//��ʧ��

		printf("bind failed with error : %d\n", iErrorMsg);

		rlt = 3;

		return rlt;

	}

	iErrorMsg = bind(m_sock2, (SOCKADDR*)&servaddr2, sizeof(servaddr2));

	if (iErrorMsg < 0)

	{

		//��ʧ��

		printf("bind failed with error : %d\n", iErrorMsg);

		rlt = 3;

		return rlt;

	}

	return rlt;

}



void ServerNet::ServerRun()

{

	// ��������

	listen(m_sock1, 5);

	listen(m_sock2, 5);

	SOCKADDR_IN tcpAddr;

	int len = sizeof(sockaddr);

	SOCKET newSocket;

	char buf[10000];

	int rval;



	do

	{

		// ������Ϣ

		newSocket = accept(m_sock1, (sockaddr*)&tcpAddr, &len);



		if (newSocket == INVALID_SOCKET)

		{

			// �ǿ���socket

			printf("invalid socket occured.\n");

		}

		else

		{

			// ���õ���socket����

			printf("new socket connect: %d\n", newSocket);



			// ��Ϣ����

			do

			{
				SocketPackage Sockrecv;

				//printf("process\n");

				// ��������

				memset(buf, 0, sizeof(buf));

				rval = recv(newSocket, buf, 10000, 0);

				memcpy(&Sockrecv, buf, sizeof(SocketPackage));

				if (Sockrecv.Res[0] && Sockrecv.Res[2047]) {
					printf("Sockpack.Framecnt =%d ,Sockpack.Framecnt =%d ,  Sockpack.Res[0] = %f,Sockpack.Res[2047] = %f\n", Sockrecv.CameraNum, Sockrecv.Framecnt, Sockrecv.Res[0], Sockrecv.Res[2047]);
				}



				if (rval == SOCKET_ERROR)

					// ���쳣ͨ��������δcloseSocket���˳�ʱ

				{

					printf("recv socket error.\n");

					break;

				}



				else if (rval == 0)

					// 0��ʾ�����˳�

					printf("socket %d connect end.\n", newSocket);

				//else

					// ��ʾ���յ�������

					//printf("recv msg: %s\n", buf);

			} while (rval != 0);



			// ���ڽ��յ�socket

			closesocket(newSocket);

		}

	} while (1);



	// �ر�����socket

	closesocket(m_sock1);

}
