#include"srvSock.h"

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")//依赖项



void exit();
void threadprocess(string);
//void dataTransform();

int socketConnect::num = 0;
char socketConnect::cmd[]{};
LARGE_INTEGER refTime;
bool startSign = false;
bool exitSign = false;

char** data1[MAX_SECONDS], ** data2[MAX_SECONDS], ** data3[MAX_SECONDS], **data4[MAX_SECONDS];
char*** dataa[DEVICES_NUM];



int main() {
	dataa[0] = data1;
	dataa[1] = data2;
	dataa[2] = data3;
	dataa[3] = data4;
	//设置控制字
	char command[2] = { 0,0 };
	command[1] |= GO; //控制字1
	command[1] |= REC * 2;
	command[1] |= TRIG * 4;
	command[1] |= EXTEN * 16;
	command[1] |= HPF * 64;
	command[1] |= HRES * 128;
	command[0] |= MODE;//控制字0
	command[0] |= NCH * 8;
	command[0] |= FSAMP * 32;
	socketConnect::cmd[0] = command[0];
	socketConnect::cmd[1] = command[1];

	for (int i = 0; i < DEVICES_NUM; i++)
		for (int j = 0; j < MAX_SECONDS; j++) {
			dataa[i][j] = new char* [SAMPFREQ];
			for (int k = 0; k < SAMPFREQ; k++)
				dataa[i][j][k] = new char[NUMCHAN * DATA_BYTES+TIMEBITS];//TIMEBITS for timestamp
		}

	thread t1(threadprocess, "192.168.1.2");

	bool alternate = false;



	cout << "Entry q to exit from the program.\n";

	while (1) {
		if (getchar() == 'q') {
			exit();
			break;
		}
		else if (getchar() != '\r');
			cout << "Entry q to exit from the program.\n";
	}
	t1.join();
	cout << "Enter S to tranform the data.\n";
	char temp = getchar();
		
	if (getchar() == 's') {
		cout << "Transforming data\n";
	}

	//free dynamic arrays
	/*
		for (int i=0;i<DEVICES_NUM;i++)
		for (int j = 0; j < MAX_SECONDS; j++) {
			for (int k = 0; k < SAMPFREQ; k++)
				delete dataa[i][j][k];
			delete dataa[i][j];
		}
	*/



	return 0;
}

void threadprocess(string ipAddr) {
	//srvSock server1();
	srvSock server1(ipAddr.c_str());
	server1.quit();
	server1.dataTransform();
}

void exit() {
	exitSign = true;
}