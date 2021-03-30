#pragma once
#include"socketConnect.h"
#include<vector>

class srvSock
{
public:
	srvSock(const char * ipAddr = "0,0,0,0");
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
	void quit() {
		WSACleanup();
		closesocket(srvsock);
	}
	void setSockOpt();
public:
	~srvSock()
	{
		for (auto ptr : cltsocks) {
			delete ptr;
		}
		quit();
	}
	void dataTransform();

};







