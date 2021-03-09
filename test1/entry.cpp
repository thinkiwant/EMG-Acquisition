#include"socketConnect.h"
#include<thread>

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")

void threadProcess(const string& s, char*);
void exit();

int socketConnect::num = 0;

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

	std::thread t1(threadProcess, "192.168.1.2", command);
	Sleep(1500);
	std::thread t2(threadProcess, "192.168.1.2", command);
	Sleep(1000);
	cout << "Entry q to exit from the program.\n";

	while (1) {
		if (getchar() == 'q')
			exit();
		else
			cout << "Entry q to exit from the program.\n";
	}

	t1.join();
	t2.join();
	return 0;
}

void threadProcess(const string& s, char* cmd) {
	const char* ip = s.data();
	socketConnect s1(ip, 45454, 1);
	s1.setCmd(cmd);
	s1.run();
	return;
}

void exit() {
	exitSign = true;
}