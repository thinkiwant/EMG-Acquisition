#include "socketConnect.h"
#pragma warning(disable:4996)

bool socketConnect::createSocket() {
	initialization();
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		print("�׽��ְ�ʧ��!");
		WSACleanup();
		return false;
	}
	else {
		print("�׽��ְ󶨳ɹ���");
		return true;
	}
}

bool socketConnect::listenSocket() {
	if (listen(s_server, SOMAXCONN) < 0) {
		print("���ü���״̬ʧ�ܣ�");
		WSACleanup();
		return false;
	}
	else {
		print("���ü���״̬�ɹ���");
	}
	print("��������ڼ������ӣ����Ժ�....");
	return true;
}



bool socketConnect::acceptSocket(SOCKET& s_accept) {
	int len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		print("����ʧ��");
		cout << "connection fail" << endl;
		WSACleanup();
		return false;
	}
	print("���ӳɹ�");
	return true;
}

bool socketConnect::sendCmd() {
	int send_len = 0;

	send_len = send(s_accept, cmd, 2, 0);
	if (send_len < 0) {
		print("�����ַ���ʧ��");
		return false;
	}
	else {
		print("�����ַ��ͳɹ�");
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
		print("�����ַ���ʧ��");
		return false;
	}
	else {
		print("�����ַ��ͳɹ�");
		return true;
	}

}

void socketConnect::record() {
	ofstream oFile;
	int frame_bytes = numChan * data_bytes * 20;
	int recv_len = 0;

	//��һ������timer

	int currentTime = 0;

	int temp = 0;
	double temp_conv;
	char* recv_buf;
	recv_buf = new char[frame_bytes];
	char file_name[10];
	sprintf(file_name, "EMG%d.csv", thisNum);
	oFile.open(file_name, ios::out | ios::trunc);
	print("���ļ��ɹ�");
	while (!exitSign) {
		if (state != 0)
			break;
		recv_len = recv(s_accept, recv_buf, frame_bytes, 0);
		/* set begaining CPU time */
		if (!initiated) {
			if (!startSign) {
				startSign = true;
				refTime = timer.begin();
			}
			else
			{
				timer.begin(refTime);
			}
			initiated = true;
		}
		currentTime = timer.timeCount();




		char c[64];
		sprintf(c, "\n�յ�%d�ֽ�����.\n", recv_len);
		print(c);
		//cout << "**************************************************" << endl;

		if (recv_len < 0) {
			print("����ʧ��");
			break;
		}
		else
		{
			//cout << "�̣߳�" << this_id << endl;
			for (int i = 0; i < recv_len; i += data_bytes) {

				//printf("0x%02x ", (unsigned char)recv_buf[i]);
				for (int j = 0; j < data_bytes; j++) {
					//printf("0x%02x ", (unsigned int)(unsigned char)recv_buf[i + j]);
					if (i + j == recv_len - 2 - data_bytes)
						temp += ((unsigned int)(unsigned char)recv_buf[i + j] && 0x3f) * power[j];
					else
						temp += (unsigned int)(unsigned char)recv_buf[i + j] * power[j];



				}
				temp = (temp >= scale[HRES * 2] ? temp - scale[HRES * 2 + 1] : temp);//�õ���ֵ

				//����accessory channelsת����λ
				int sign = NUMCHAN - (i / DATA_BYTES) % NUMCHAN;
				if ((sign != 2) && (sign != 1)) {
					temp_conv = temp * ConvFact;
					//printf(" %f ", temp_conv);
					oFile << temp_conv << ",";
				}
				else
				{
					//printf(" %d ", temp);	
					oFile << temp << ",";

				}

				temp = 0;

				/*   (output CPU time)*/
				if (((i - data_bytes) % frame_bytes == 0) && (i > data_bytes)) {
					oFile << currentTime;
					oFile << endl;

				}



			}

		}
	}
	oFile.close();
	delete[]recv_buf;//�ͷŶ�̬����b   

}


void socketConnect::print(const string& s) {
	if (print_type == 1) {
		string temp = "object " + (std::to_string(thisNum) + (": " + s)) + "\n";
		//cout << "object " << thisNum << ": " << s << endl;
		cout << temp;
	}

	else if (print_type == 2)
	{
		const char* p = s.data();
		OutputDebugStringA(p);
	}
}

void socketConnect::initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		print("��ʼ���׽��ֿ�ʧ�ܣ�");
	}
	else {
		print("��ʼ���׽��ֿ�ɹ���");
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		print("�׽��ֿ�汾�Ų�����");
		WSACleanup();
	}
	else {
		print("�׽��ֿ�汾��ȷ��");
	}
	//������˵�ַ��Ϣ

}