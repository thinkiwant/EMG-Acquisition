#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include"socketConnect.h"
#include "entry.cpp"
#include"srvSock.h"

using namespace std;
extern const int scale[];
extern const int power[];

bool greaterThan(char* a, char* b);
bool greaterThan(socketConnect, socketConnect);

void dataTransform() {
	ofstream outFile("EMG.csv");
	string line, line1;


	int startTime = -1, temp = -1;


	string number;
	for (auto p : mdList) {
		for (int i = 0; i < 500; i++)
			getline(ifile2, line);
		getline(*p, line);
		istringstream readstr(line);
		for (int i = 0; i < 204; i++) {
			getline(readstr, number, ',');
		}
		getline(readstr, number, ',');
		if (startTime < atoi(number.c_str()))
			startTime = atoi(number.c_str());
	}

	for (auto p : mdList) {
		temp = -1;
		int row = 0;
		while ((startTime - temp) > 20)
		{
			getline(*p, line);
			istringstream readstr(line);
			for (int i = 0; i < 204; i++) {
				getline(readstr, number, ',');
			}
			getline(readstr, number, ',');
			temp = atoi(number.c_str());
			row++;
		}
		cout << "file's row is " << row << endl;
	}

	int temp1 = 0;
	while (!ifile1.eof()) {
		for (auto md : mdList) {
			getline(*md, line1);
			istringstream readstr1(line1);
			for (int j = 0; j < 64; j++) {
				for (int i = 0; i < 3; i++) {
					getline(readstr1, number, ',');
					temp1 += atoi(number.c_str()) * power[i];
				}
				temp1 = (temp1 >= scale[HRES * 2] ? temp1 - scale[HRES * 2 + 1] : temp1);
				outFile << temp1 * ConvFact << ",";
				temp1 = 0;
			}
		}


///
		char*** refData = NULL;
		for (int i = 0; i < DEVICES_NUM; i++)
			;
			refData = greaterThan(&dataa[i][MAX_SECONDS][SAMPFREQ][NUMCHAN * DATA_BYTES], &dataa[i + 1][MAX_SECONDS][SAMPFREQ][NUMCHAN * DATA_BYTES]) ? dataa[i]: dataa[i + 1];





		outFile << endl;

	}

	for (auto p : mdList)
		p->close();
	outFile.close();
}

bool greaterThat(char* a, char* b) {
	for (int i = 0; i < 4; i++) {
		if (a[i] > b[i])
			return 1;
	}
	return 0;
}

bool greaterThan(const socketConnect& a, const socketConnect& b) {
	if (a.index1 > b.index1)
		return 1;
	else if (a.index1 == b.index1) {
		if (a.index2 > b.index2)
			return 1;
		else if (a.index2 == b.index2) {
			if (a.samples > b.samples)
				return 1;
		}
		return 0;
			
	}		
}
