
#include"srvSock.h"


#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")//依赖项

void exit();
void threadprocess();

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
	thread t1(threadprocess);


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
	return 0;
}

void threadprocess() {
	srvSock server1;
}

void exit() {
	exitSign = true;
}