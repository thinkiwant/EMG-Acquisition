#pragma once
#include"socketConnect.h"
#include<vector>

class srvSock
{
public:
	srvSock(const char * ipAddr = "0,0,0,0");
	void sendCmd(char *);

	void quit() {
		disconnect();
		WSACleanup();
		closesocket(srvsock);

	}

private:

	sockaddr_in srv_addr, clt_addr;
	SOCKET srvsock, cltsock;
	int cltsock_num, addr_sz = sizeof(clt_addr);
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	int fdNum, i;

	vector<socketConnect*> cltsocks;
	int initialization();
	int srvbind();
	int srvlisten();
	int srvaccept();
	void run();
	void disconnect();
	void setSockOpt();
public:
	~srvSock()
	{
		//quit();
		for (auto i : cltsocks)
			delete i;
	}
	void dataTransform();

};
