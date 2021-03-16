#include "srvSock.h"

void data_transfer(socketConnect&);

srvSock::srvSock() :cltsock_num(0), cltsocks({}) {
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(atoi("45454"));
	print("The server socket is created.\n");
	if (!initialization())
		if (!srvbind())
			if (!srvlisten())
				while (!exitSign) {
					cout << "Accept function" << endl;
					srvaccept();

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
	if (listen(srvsock, SOMAXCONN)) {
		print("Fails to make the server socket listen.\n");
		return -1;
	}

	print("The Server socket is listening.\n");
	return 0;
}

int srvSock::srvaccept() {
	cltsock = accept(srvsock, (sockaddr*)&clt_addr, &addr_sz);
	print("Client socket is connected.\n");
	socketConnect* ptr = new socketConnect(cltsock);
	cltsocks.push_back(ptr);
	cltsock_num = cltsocks.size();
	thread client_thread(data_transfer, ref(**(cltsocks.end()-1)));
	client_thread.detach();
	return 1;
}

void data_transfer(socketConnect& s) {
	s.run();
}

