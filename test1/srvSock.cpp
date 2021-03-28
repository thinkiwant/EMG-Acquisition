#include "srvSock.h"

void data_transfer(socketConnect&);

srvSock::srvSock(const char * ipAddr) :cltsock_num(0), cltsocks({}) {
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
	srv_addr.sin_port = htons(atoi("45454"));
	print("The server socket is created.\n");
	run();
}

void srvSock::run() {


	if (!initialization())
		if (!srvbind())
			if (!srvlisten()) {
				FD_ZERO(&reads);
				FD_SET(srvsock, &reads);
				int initCount = reads.fd_count;
				while (!exitSign) {
					cpyReads = reads;
					timeout.tv_sec = 5;
					timeout.tv_usec = 0;
					if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR) {
						print("select Error\n");
						break;
					}
					if (fdNum == 0) {
						string temp{ inet_ntoa(srv_addr.sin_addr) };
						temp.append(" Time out\n");
						print(temp.c_str());
						continue;
					}
					for (i = 0; i < (int)reads.fd_count; i++) {
						if (FD_ISSET(reads.fd_array[i], &cpyReads)) {
							if (reads.fd_array[i] == srvsock)
								srvaccept();
							else {
								if (i > initCount-1)
									cltsocks[i - initCount]->record();
							}
						}
					}
				}
			}
				
}

int srvSock::initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		print("初始化套接字库失败！\n");
		return -1;
	}
	else {
		print("初始化套接字库成功！\n");
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		print("套接字库版本号不符！\n");
		return -1;
	}
	else {
		print("套接字库版本正确！\n");
	}
	//填充服务端地址信息
	return 0;
}

int srvSock::srvbind() {
	srvsock = socket(PF_INET, SOCK_STREAM, 0);
	if ((bind(srvsock, (sockaddr*)&srv_addr, sizeof(srv_addr))==SOCKET_ERROR)) {
		print("Server socket binding fails.\n");
		return -1;
	}
	print("Server socket bindind succeeds\n");
	return 0;
}

int srvSock::srvlisten() {
	setSockOpt();
	if (listen(srvsock, SOMAXCONN)) {
		print("Fails to make the server socket listen.\n");
		return -1;
	}

	print("The Server socket is listening.\n");
	return 0;
}

int srvSock::srvaccept() {
	cltsock = accept(srvsock, (sockaddr*)&clt_addr, &addr_sz);
	string add_str = inet_ntoa(clt_addr.sin_addr);
	FD_SET(cltsock, &reads);
	print("Client socket is connected.\n");
	socketConnect* ptr = new socketConnect(cltsock, add_str);
	cltsocks.push_back(ptr);
	cltsock_num = cltsocks.size();
	ptr->sendCmd();


	return 1;
}

void srvSock::setSockOpt() {
	int state, optval = 1;

	int len = sizeof(optval);

	state = setsockopt(srvsock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, len);
	state = getsockopt(srvsock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, &len);

	if (!state)
		printf("***** Nagle is enabled ? %d\n ", optval);
}
