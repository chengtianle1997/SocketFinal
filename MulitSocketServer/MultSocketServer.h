#pragma once
#include <vector>
#include "afxwin.h"


struct LISTEN_SOCKET_INFO
{
	UINT16   listenPort;  //�����˿�
	SOCKET   listenSocket;//���
	WSAEVENT netEvent;    //socket��Ӧ�¼�
};

class IAcceptCallback
{
public:
	virtual void OnAcceptClient(SOCKET hSocketClient, UINT16 nListenPort) = 0;
};

class IocpAccept
{
public:
	IocpAccept();
	~IocpAccept();

	void SetCallback(IAcceptCallback* callback);

	void AddListenPort(UINT16 port);
	int CreateListenInfo();

	BOOL Start();
	void Stop();
private:
	static  DWORD WINAPI ListenRoutine(LPVOID lp_param);
	DWORD Deal_ListenRoutine();
	WSAEVENT* CreateNetEventArray(int *nEventTotal);

	UINT AcceptListenPort(SOCKET hListenSocket, UINT16 nListenPort);
	void OnAcceptClient(SOCKET hSocketClient, UINT16 nListenPort);
private:
	BOOL m_bServerStart;
	DWORD m_dwThreadID;
	IAcceptCallback* m_callback;

	std::vector<UINT16>   m_listListenPort;
	std::vector<LISTEN_SOCKET_INFO>   m_listListenInfo;
};

#pragma once
