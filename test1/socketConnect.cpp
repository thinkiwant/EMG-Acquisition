#include "socketConnect.h"
#pragma warning(disable:4996)
mutex mtx1, mtx2;

void print(const string& s) {
	if (PRINT_TYPE) {
		std::cout << s;
	}

	else
	{
		const char* p = s.data();
		OutputDebugStringA(p);
	}
}
socketConnect::socketConnect(SOCKET clt_sock, const int nC, const int dB, const int sF) :s_accept(clt_sock), numChan(nC), data_bytes(dB), sampFreq(sF) {
	mtx1.lock();
	thisNum = ++this->num;
	mtx1.unlock();

}


socketConnect::~socketConnect() {
	//quit();
}



bool socketConnect::sendCmd() {
	int send_len = 0;
	send_len = send(s_accept, cmd, 2, 0);
	if (send_len < 0) {
		print("控制字发送失败");
		return false;
	}
	else {
		print("控制字发送成功");
		//print("0x%02x0x%02x\n", (unsigned char)send_buf[0], (unsigned char)send_buf[1]); print the command
		return true;
	}

}

bool socketConnect::sendCmd(char* c) {
	int send_len = 0;
	char send_buf[2];
	strcpy(send_buf, c);
	send_len = send(s_accept, send_buf, 2, 0);
	if (send_len < 0) {
		print("控制字发送失败");
		return false;
	}
	else {
		print("控制字发送成功");
		return true;
	}

}

void socketConnect::record() {
	cout << "record" << endl;
	ofstream oFile;
	int frame_bytes = numChan * data_bytes;
	int frames_bytes = frame_bytes * 5;
	int recv_len = 0;

	//第一次设置timer

	int currentTime = 0;


	char* recv_buf;
	recv_buf = new char[frames_bytes];
	char file_name[10];
	sprintf(file_name, "EMG%d.csv", thisNum);
	oFile.open(file_name, ios::out | ios::trunc);
	print("打开文件成功");
	while (!exitSign) {
		if (state != 0)
			break;
		recv_len = recv(s_accept, recv_buf, frames_bytes, 0);
/* set begaining CPU time */
		if (!initiated) {
			mtx2.lock();
			if (!startSign) {
				startSign = true;
				refTime = timer.begin();
			}
			else
			{
				timer.begin(refTime);
			}
			mtx2.unlock();
			initiated = true;
		}
		//currentTime = timer.timeCount();




		//char c[64];
		//sprintf(c, "\n收到%d字节数据.\n", recv_len);
		//print(c);
		//cout << "**************************************************" << endl;

		if (recv_len < 0) {
			print("接收失败");
			break;
		}
		else
		{	
			
			char* ptr = recv_buf;
			for (int i = 0, frames = recv_len / frame_bytes; i < frames; i++) {
				char* ptr = recv_buf + i * frame_bytes;
				for (int j = 0; j < frame_bytes; j++)
					oFile << (unsigned int)(unsigned char)*(ptr++)<<",";
				oFile << endl;
			}
			


		}
	}
	quit();
	oFile.close();
	delete[]recv_buf;//释放动态数组b   

}


void socketConnect::print(const string& s) {
	string temp = "object " + (std::to_string(thisNum) + (": " + s)) + "\n";
	::print(temp);

}

void socketConnect::quit() {
	char exitCmd[2] = { 0,0 };
	setCmd(exitCmd);
	sendCmd();
	WSACleanup();
	closesocket(s_accept);
	print("Disconnected");
}