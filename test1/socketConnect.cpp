#include "socketConnect.h"
#pragma warning(disable:4996)
mutex mtx1, mtx2, mtx3;

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
socketConnect::socketConnect(SOCKET clt_sock,string & add_str, const int nC, const int dB, const int sF) :s_accept(clt_sock), numChan(nC), data_bytes(dB), sampFreq(sF) {
	this_address = add_str;
	mtx1.lock();
	thisNum = ++this->num;
	deviceNum = this_address.back()-'0';
	mtx1.unlock();
	openFile();


}


socketConnect::~socketConnect() {
	quit();
}

void socketConnect::openFile() {
	char file_name[10];
	this_address.erase(0, 10);
	this_address += "EMG.csv";
	sprintf(file_name, "EMG%d.csv", thisNum);
	oFile.open(this_address.c_str(), ios::out | ios::trunc);
	print("打开文件成功");
}

void socketConnect::closeFile() {
	oFile.close();
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
	static int frame_bytes = numChan * data_bytes;
	static int frames_bytes = frame_bytes;
	int recv_len = 0;

	/* set beginning CPU time */
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


	recv_len = recv(s_accept, dataa[deviceNum-2][index1][index2], frames_bytes, 0);
	int currentTime = timer.timeCount();
	itoa(currentTime, &dataa[deviceNum - 2][index1][index2][frames_bytes],10);
	if (++index2 == SAMPFREQ) {
		index2 = 0;
		index1++;
	}


		//char c[64];
		//sprintf(c, "\n收到%d字节数据.\n", recv_len);
		//print(c);
		//cout << "**************************************************" << endl;

		if (recv_len < 0) {
			print("接收失败");
			//break;
		}

		/*
		else{	
			for (int i = 0, frames = recv_len / frame_bytes; i < frames; i++) {
				char* ptr = recv_buf + i * frame_bytes;
				for (int j = 0; j < frame_bytes; j++)
					oFile << (unsigned int)(unsigned char)*(ptr++)<<",";
				oFile << currentTime << endl;
			}

		}
		*/



}


void socketConnect::print(const string& s) {
	string temp = "object " + (std::to_string(thisNum) + (": " + s)) + "\n";
	::print(temp);

}

void socketConnect::quit() {
	char exitCmd[2] = { 0,0 };
	sendCmd(exitCmd);
	//WSACleanup();
	closesocket(s_accept);
	print("Disconnected");
	closeFile();
}