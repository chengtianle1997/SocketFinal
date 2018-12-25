//#include "stdafx.h"
#include "MultSocketServer.h"
#include <assert.h>


IocpAccept::IocpAccept()
{
	m_callback = NULL;
}


IocpAccept::~IocpAccept()
{
}

void IocpAccept::SetCallback(IAcceptCallback* callback)
{
	m_callback = callback;
}

void IocpAccept::AddListenPort(UINT16 port)
{
	std::vector<UINT16>::iterator begin = m_listListenPort.begin();
	for (; begin != m_listListenPort.end(); ++begin)
	{
		if (*begin == port)
			return;
	}

	m_listListenPort.push_back(port);
}

int IocpAccept::CreateListenInfo()
{
	//m_listListenPort�洢Ҫ�����Ķ˿ڣ��ܸ���������64��
	std::vector<UINT16>::iterator pos = m_listListenPort.begin();
	for (; pos != m_listListenPort.end(); ++pos)
	{
		//����socket
		UINT16 listenPort = *pos;
		LISTEN_SOCKET_INFO socketInfo;
		socketInfo.listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		socketInfo.listenPort = listenPort;

		//�󶨶˿�
		sockaddr_in InetAddr;
		InetAddr.sin_family = AF_INET;
		InetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		InetAddr.sin_port = htons(listenPort);

		int ret = bind(socketInfo.listenSocket, (SOCKADDR *)&InetAddr, sizeof(InetAddr));
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			//��ʧ��
			continue;
		}

		//�����¼�
		socketInfo.netEvent = WSACreateEvent();

		//��socket������¼�����������ֻ����socket��accept��close��Ϣ
		ret = WSAEventSelect(socketInfo.listenSocket, socketInfo.netEvent, FD_ACCEPT | FD_CLOSE);
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			continue;
		}

		// ��������
		ret = listen(socketInfo.listenSocket, 1000);
		if (SOCKET_ERROR == ret)
		{
			::closesocket(socketInfo.listenSocket);
			continue;
		}

		m_listListenInfo.push_back(socketInfo);
	}
	return 0;
}

BOOL IocpAccept::Start()
{
	m_bServerStart = TRUE;
	CreateListenInfo();
	if (m_listListenInfo.size() != m_listListenPort.size())
	{
		for (int i = 0; i < m_listListenInfo.size(); i++)
		{
			LISTEN_SOCKET_INFO item = m_listListenInfo[i];
			closesocket(item.listenSocket);
		}
		m_listListenInfo.clear();
		return FALSE;
	}

	//���������߳�
	CreateThread(NULL, 0, ListenRoutine, (PVOID)this, 0, &m_dwThreadID);
	return TRUE;
}

DWORD IocpAccept::ListenRoutine(LPVOID lp_param)
{
	IocpAccept *pNetServer = (IocpAccept*)lp_param;
	pNetServer->Deal_ListenRoutine();
	return 0;
}

WSAEVENT* IocpAccept::CreateNetEventArray(int *nEventTotal)
{
	*nEventTotal = m_listListenInfo.size();
	WSAEVENT *pResult = new WSAEVENT[*nEventTotal];
	for (int i = 0; i < m_listListenInfo.size(); i++)
	{
		pResult[i] = m_listListenInfo[i].netEvent;
	}
	return pResult;
}

DWORD IocpAccept::Deal_ListenRoutine()
{
	//�����¼���ַָ��
	int nEventTotal;
	WSAEVENT* pEventArray = CreateNetEventArray(&nEventTotal);
	if (nEventTotal == 0)
		return 0;
	assert(nEventTotal <= WSA_MAXIMUM_WAIT_EVENTS);

	MSG msg;
	while (m_bServerStart)
	{
		// ͬʱ�Զ���¼�������
		DWORD index = WSAWaitForMultipleEvents(nEventTotal,
			pEventArray,
			FALSE,
			10000,
			FALSE);
		if (!m_bServerStart)
			return 0;

		//�鿴���ĸ��¼�������������
		index = index - WSA_WAIT_EVENT_0;
		//�ͻ��������¼�
		if ((index != WSA_WAIT_FAILED) && (index != WSA_WAIT_TIMEOUT))
		{
			//pEventArray������m_listListenInfoһ�������Կ��Ը���index�ҵ���Ӧ��socket��
			//���Ǹ�socket���º�������
			LISTEN_SOCKET_INFO socketInfo = m_listListenInfo[index];

			//�鿴������ʲô�¼����º�������
			WSANETWORKEVENTS NetworkEvents;
			WSAEnumNetworkEvents(socketInfo.listenSocket, pEventArray[index], &NetworkEvents);

			//�����accept�¼���˵���пͻ������Ӵ˶˿�
			if (NetworkEvents.lNetworkEvents == FD_ACCEPT
				&& NetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0)
			{
				//��ʱ����accept����������������
				AcceptListenPort(socketInfo.listenSocket, socketInfo.listenPort);
			}
			if (NetworkEvents.lNetworkEvents == FD_CLOSE
				&& NetworkEvents.iErrorCode[FD_CLOSE_BIT] == 0)
			{
				assert(false);
			}

		}
		else
		{
			printf("��Ϊ��ʱ������ԭ������������");
			//��Ϊ��ʱ������ԭ������������
		}
	}
	return 0;
}

UINT IocpAccept::AcceptListenPort(SOCKET hListenSocket, UINT16 nListenPort)
{
	SOCKET hClient = 0;
	SOCKADDR_IN localAddr;
	int iaddrSize = sizeof(SOCKADDR_IN);

	hClient = accept(hListenSocket, (struct sockaddr *)&localAddr, &iaddrSize);
	if (INVALID_SOCKET == hClient)
	{
		int nAccepetError = WSAGetLastError();
		if (nAccepetError == WSAECONNRESET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//��ȡ��һ���ͻ�������
		OnAcceptClient(hClient, nListenPort);
	}
	return 0;
}


void IocpAccept::OnAcceptClient(SOCKET hSocketClient, UINT16 nListenPort)
{
	m_callback->OnAcceptClient(hSocketClient, nListenPort);
}


void IocpAccept::Stop()
{
	if (!m_bServerStart)
		return;
	m_bServerStart = FALSE;

	for (int i = 0; i < m_listListenInfo.size(); i++)
	{
		closesocket(m_listListenInfo[i].listenPort);
	}
}


