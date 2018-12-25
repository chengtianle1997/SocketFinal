
/*

ServerNet.cpp

*/



#include "MSServer.h"



/*服务器初始化*/

int ServerNet::ServerInit(const char* address, int port1 , int port2)

{

	int rlt = 0;



	int iErrorMsg;



	//初始化WinSock

	WSAData wsaData;

	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);



	if (iErrorMsg != NO_ERROR)

	{

		//初始化WinSock失败

		printf("server wsastartup failed with error : %d\n", iErrorMsg);



		rlt = 1;

		return rlt;

	}



	// 创建服务器端socket



	m_sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_sock1 == INVALID_SOCKET)

		// 创建socket出现了异常

	{

		printf("server socket failed with error: %d\n", WSAGetLastError());

		rlt = 2;

		return rlt;

	}

	m_sock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_sock2 == INVALID_SOCKET)

		// 创建socket出现了异常

	{

		printf("server socket failed with error: %d\n", WSAGetLastError());

		rlt = 2;

		return rlt;

	}


	// 声明信息

	SOCKADDR_IN	servaddr1;
	
	servaddr1.sin_family = AF_INET;

	servaddr1.sin_port = port1;

	servaddr1.sin_addr.s_addr = inet_addr(address);

	SOCKADDR_IN	servaddr2;

	servaddr2.sin_family = AF_INET;

	servaddr2.sin_port = port2;

	servaddr2.sin_addr.s_addr = inet_addr(address);

	//绑定

	iErrorMsg = bind(m_sock1, (SOCKADDR*)&servaddr1, sizeof(servaddr1));
	

	if (iErrorMsg < 0)

	{

		//绑定失败

		printf("bind failed with error : %d\n", iErrorMsg);

		rlt = 3;

		return rlt;

	}

	iErrorMsg = bind(m_sock2, (SOCKADDR*)&servaddr2, sizeof(servaddr2));

	if (iErrorMsg < 0)

	{

		//绑定失败

		printf("bind failed with error : %d\n", iErrorMsg);

		rlt = 3;

		return rlt;

	}

	return rlt;

}



void ServerNet::ServerRun()

{

	// 公开连接

	listen(m_sock1, 5);

	listen(m_sock2, 5);

	SOCKADDR_IN tcpAddr;

	int len = sizeof(sockaddr);

	SOCKET newSocket;

	char buf[10000];

	int rval;



	do

	{

		// 接收信息

		newSocket = accept(m_sock1, (sockaddr*)&tcpAddr, &len);



		if (newSocket == INVALID_SOCKET)

		{

			// 非可用socket

			printf("invalid socket occured.\n");

		}

		else

		{

			// 可用的新socket连接

			printf("new socket connect: %d\n", newSocket);



			// 消息处理

			do

			{
				SocketPackage Sockrecv;

				//printf("process\n");

				// 接收数据

				memset(buf, 0, sizeof(buf));

				rval = recv(newSocket, buf, 10000, 0);

				memcpy(&Sockrecv, buf, sizeof(SocketPackage));

				if (Sockrecv.Res[0] && Sockrecv.Res[2047]) {
					printf("Sockpack.Framecnt =%d ,Sockpack.Framecnt =%d ,  Sockpack.Res[0] = %f,Sockpack.Res[2047] = %f\n", Sockrecv.CameraNum, Sockrecv.Framecnt, Sockrecv.Res[0], Sockrecv.Res[2047]);
				}



				if (rval == SOCKET_ERROR)

					// 该异常通常发生在未closeSocket就退出时

				{

					printf("recv socket error.\n");

					break;

				}



				else if (rval == 0)

					// 0表示正常退出

					printf("socket %d connect end.\n", newSocket);

				//else

					// 显示接收到的数据

					//printf("recv msg: %s\n", buf);

			} while (rval != 0);



			// 关于接收的socket

			closesocket(newSocket);

		}

	} while (1);



	// 关闭自身socket

	closesocket(m_sock1);

}
