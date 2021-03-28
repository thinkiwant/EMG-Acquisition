#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include"socketConnect.h"

using namespace std;
extern const int scale[];
extern const int power[];


void dataTransform() {
	ofstream outFile("EMG.csv");
	ifstream ifile1("12EMG.csv");
	ifstream ifile2("13EMG.csv");
	ifstream ifile3("14EMG.csv");
	string line, line1;
	vector<ifstream*> mdList;
	mdList.push_back(&ifile1);
	mdList.push_back(&ifile2);
	mdList.push_back(&ifile3);

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
		outFile << endl;

	}

	for (auto p : mdList)
		p->close();
	outFile.close();
}