#pragma once

#include<winsock.h>
#include<string>
#include<iostream>
#include<Windows.h>
#include<fstream>
#include<mutex>
#include"def.h"
#include"timer.h"

#pragma warning(disable:4996)

using namespace std;
void print(const string& s);
const int scale[] = { 32768, 65536, 8388608, 16777216 };
//const int power[] = { 1,256,65536 };//little endian
const int power[] = { 65536, 256, 1 };

extern mutex mtx1, mtx2;
extern LARGE_INTEGER refTime;
extern bool startSign;
extern bool exitSign;

class socketConnect
{
	SOCKET s_accept;
	SOCKADDR_IN accept_addr;

	int state = 0;
	int thisNum;
	Timer timer;
	bool initiated = false;
	const int numChan, data_bytes, sampFreq;

	ofstream oFile;

public:
	static int num;
	//int syncCount=50;
	static char cmd[2];
	socketConnect(SOCKET clt_sock, string& add_str, const int nC = NUMCHAN, const int dB = DATA_BYTES, const int sF = SAMPFREQ);
	~socketConnect();

	string this_address;
	bool sendCmd();
	bool sendCmd(char* const c);
	void openFile();
	void closeFile();
	void record();
	void print(const string& s);
	void setCmd(char* c) { cmd[0] = c[0]; cmd[1] = c[1]; }
	void setState() { state++; }

	void quit();
	void dataTransform();

};


#pragma once
