#pragma once
#include"socketConnect.h"
#include<vector>

class srvSock
{
public:
	srvSock();
private:

	sockaddr_in srv_addr, clt_addr;
	SOCKET srvsock, cltsock;
	int cltsock_num, addr_sz = sizeof(clt_addr);
	vector<socketConnect*> cltsocks;
	int initialization();
	int srvbind();
	int srvlisten();
	int srvaccept();
	void quit() {
		WSACleanup();
		closesocket(srvsock);
		closesocket(cltsock);
	}
public:
	~srvSock()
	{
		quit();
		(**(cltsocks.end() - 1)).quit();
		for (auto ptr : cltsocks)
			delete ptr;
	}
};







