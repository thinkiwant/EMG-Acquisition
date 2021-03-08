#pragma once

#include<winsock.h>
#include<string>
#include<iostream>
#include<Windows.h>
#include<fstream>
#include"def.h"
#include"timer.h"

#pragma warning(disable:4996)

using namespace std;

const int scale[] = { 32768, 65536, 8388608, 16777216 };
//const int power[] = { 1,256,65536 };//little endian
const int power[] = { 65536, 256, 1 };

extern LARGE_INTEGER refTime;
extern bool startSign;
extern bool exitSign;

class socketConnect
{

	SOCKET s_server, s_accept;
	SOCKADDR_IN server_addr, accept_addr;
	char cmd[2];
	int state = 0;
	int thisNum;
	Timer timer;
	bool initiated = false;
	const int print_type, numChan, data_bytes, sampFreq;

public:
	static int num;
	socketConnect(const char* ptr_ad, const u_short p, const int t, const int nC = NUMCHAN, const int dB = DATA_BYTES, const int sF = SAMPFREQ) :print_type(t), numChan(nC), data_bytes(dB), sampFreq(sF) {
		thisNum = ++this->num;
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.S_un.S_addr = inet_addr(ptr_ad);
		server_addr.sin_port = htons(p);
	}

	~socketConnect() {
		char exitCmd[2] = { 0,0 };
		setCmd(exitCmd);
		sendCmd();
		closesocket(s_server);
		closesocket(s_accept);
		WSACleanup();
		print("Disconnected");
	}

	void initialization();
	bool createSocket();
	bool listenSocket();
	bool acceptSocket();
	bool sendCmd();
	bool sendCmd(char* c);
	void record();
	void print(const string& s);
	void setCmd(char* c) { cmd[0] = c[0]; cmd[1] = c[1]; }
	void setState() { state++; }
	void run() {
		if (createSocket()) {
			if (listenSocket()) {
				if (acceptSocket()) {
					if (sendCmd())
						record();
					return;
				}
			}
		}


	}

};
#pragma once
