#include "srvSock.h"
#include <ctime>
using namespace std;

void data_transfer(socketConnect&);
bool greaterThan(const socketConnect& a, const socketConnect& b);
bool findIndex(int& index1, int& index2, char*** targetTime, char* refTime,int );

srvSock::srvSock(const char * ipAddr) :cltsock_num(0), cltsocks({}) {
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
	//srv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
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
								if (i > initCount - 1)
									i = i;
									cltsocks[i - initCount]->record();
							}
						}
					}
				}
			}
				
}

void srvSock::sendCmd(char* p_cmd) {
	for (auto i = cltsocks.begin(); i != cltsocks.end(); i++) {
		(*i)->sendCmd(p_cmd);
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
	//state = getsockopt(srvsock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, &len);

	if (!state)
		printf("***** Nagle is enabled ? %d\n ", optval);
}

void srvSock::disconnect() {
	for (auto i : cltsocks)
		i->quit();
}

void srvSock::dataTransform() {

	int index1Group[DEVICES_NUM]{ 0,0,0 }, index2Group[DEVICES_NUM]{ 0,0,0 };
	int temp = 0,final_data=0;

	int lestDeviceId=cltsocks[0]->deviceNum,lestDeviceIndex=0;
	socketConnect* ptrLestSockCon = cltsocks[0];
	printf("checking max seconds of %d :%d\n", cltsocks[0]->deviceNum, cltsocks[0]->index1);
	for (int i = 1; i < DEVICES_NUM ; i++) {
			printf("checking max seconds of %d :%d\n", cltsocks[i]->deviceNum, cltsocks[i]->index1);
		if (greaterThan(*ptrLestSockCon, *cltsocks[i])) {
			ptrLestSockCon = cltsocks[i];
			lestDeviceId = cltsocks[i]->deviceNum;
			lestDeviceIndex = i;
		}
	}
	std::cout << "The last device is " << ptrLestSockCon->deviceNum << endl;
	std::cout << "The maxseconds: " << ptrLestSockCon->index1<<", time series: "<<ptrLestSockCon->index2 << endl;
	string time1{ to_string(ptrLestSockCon->init_time.wYear) };
	time1 = time1 + "-" + to_string(ptrLestSockCon->init_time.wMonth) + "-" + to_string(ptrLestSockCon->init_time.wDay) + "-" + to_string(ptrLestSockCon->init_time.wHour) + "-" + to_string(ptrLestSockCon->init_time.wMinute) + "-" + to_string(ptrLestSockCon->init_time.wSecond)+"-"+to_string(ptrLestSockCon->init_time.wMilliseconds);
	time1 += ".csv";
	std::ofstream f1(time1.c_str());
	/*
		for (int i = 0; i < DEVICES_NUM; i++)
			for(int j=0;j<100;j++)
				cout <<"device"<<i<<"'s"<<j<<" th: "<< &dataa[i][0][j][NUMCHAN * DATA_BYTES]<<endl;
	*/


	for (int i = 0; i < DEVICES_NUM; i++) {
		const int drop_time1 = 0, drop_time2 = 200; // Drop unstable preceding data 
		if (i == lestDeviceId-INDEXDIFF) {
			index1Group[i] =  drop_time1;
			index2Group[i] =  drop_time2;
		}
		else
		{
			if (!findIndex(index1Group[i], index2Group[i], dataa[i], &dataa[lestDeviceId-INDEXDIFF][drop_time1][drop_time2][NUMCHAN*DATA_BYTES], ptrLestSockCon->index1))
				std::cout << "Device " << i << " Find Index error" << endl;
			else
				std::cout <<"Device "<< i << " Index found successfully" << endl;
		}

	}

	int temp_i=0, temp_j=0;
	std::cout << "rectify device:  ";
	for (int i = 0; i < DEVICES_NUM; i++) {
		printf("\b\b%2d", i);
		temp_i = index1Group[i];
		temp_j = index2Group[i];
		for (int newIdx1 = 0;newIdx1<ptrLestSockCon->index1-1;newIdx1++)
			for (int newIdx2 = 0; newIdx2 < SAMPFREQ; newIdx2++) {
				dataa[i][newIdx1][newIdx2] = dataa[i][temp_i][temp_j];
				if (++temp_j >= SAMPFREQ) {
					temp_j = 0;
					temp_i++;
				}
			}
	}
	std::cout << endl;


	std::cout << "Data transforming\n"<<"\tProgressing second:  ";
	
	for (int second = 0; second < ptrLestSockCon->index1; second++) {
		printf("\b\b%2d", second);
		for (int freq = 0; freq < SAMPFREQ; freq++) {
			for (int device = 0; device < DEVICES_NUM; device++)
				for (int chan = 0; chan < NUMCHAN - 4; chan++) {
					for (int byte = 0; byte < DATA_BYTES; byte++)
						temp += (unsigned int)(unsigned char)dataa[device][second][freq][chan * DATA_BYTES + byte] * power[byte];
					final_data = (temp >= scale[HRES * 2] ? temp - scale[HRES * 2 + 1] : temp);
					f1 << final_data * ConvFact << ",";
					temp = 0;
				}
			f1 << endl;
		}

	}
	f1.close();
	std::cout << endl;

}

bool greaterThan(const socketConnect& a, const socketConnect& b) {
	if (a.index1 > b.index1)
		return 1;
	else if (a.index1 == b.index1) {
		if (a.index2 > b.index2)
			return 1;	
	}	
	return 0;
}

bool findIndex(int& index1, int& index2,char*** targetTime,char* refTime, int maxSeconds ) {
	double acc=0;
	char refchar, tarchar;
	for (int i = 0; i <= maxSeconds; i++) {
		for (int j = 0; j < SAMPFREQ; j++) {
			acc = 0;
			for (int k = 0; k < TIMEBITS; k++) {
				refchar = *(refTime + k);
				tarchar = *(&targetTime[i][j][NUMCHAN * DATA_BYTES] + k);

				acc += (refchar - tarchar) * pow(0.1, k);
				if ((refchar == '\0') && (tarchar == '\0')) {
					//std::cout << "同长度 acc = " << acc << endl;
					//printf("dataa[%d][%d][n+%d]refchar: %c, tarchar: %c\n ", i, j, k, refchar, tarchar);
					if (acc * pow(10, k - 1) < 20) {// Timestamp difference threshold is set as 20us
						index1 = i;
						index2 = j;
						return 1;
					}
					else
						break;
				}
				else if ((tarchar == '\0') || (refchar == '\0')) {
					break;
				}

			}
		}
	}
		
	return 0;
}