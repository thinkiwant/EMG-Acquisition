
#include"srvSock.h"


#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")//依赖项

void exit();
void threadprocess(string);
void dataTransform();

int socketConnect::num = 0;
char socketConnect::cmd[]{};
LARGE_INTEGER refTime;
bool startSign = false;
bool exitSign = false;

int main() {
	//设置控制字
	char command[2] = { 0,0 };
	command[1] |= GO; //控制字1
	command[1] |= TRIG * 4;
	command[1] |= EXTEN * 16;
	command[1] |= HPF * 64;
	command[1] |= HRES * 128;
	command[0] |= MODE;//控制字0
	command[0] |= NCH * 8;
	command[0] |= FSAMP * 32;
	socketConnect::cmd[0] = command[0];
	socketConnect::cmd[1] = command[1];
	thread t1(threadprocess, "192.168.1.2");
	thread t2(threadprocess, "192.168.1.100");
	thread t3(threadprocess, "192.168.1.101");


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
	t2.join();
	t3.join();
	cout << "Enter S to tranform the data.\n";
	char temp = getchar();
		
	if (getchar() == 's') {
		cout << "Transforming data\n";
		dataTransform();
	}

	return 0;
}

void threadprocess(string ipAddr) {
	srvSock server1(ipAddr.c_str());
}

void exit() {
	exitSign = true;
}