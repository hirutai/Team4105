#include "CSVLoader.h"

#include <fstream>
#include <sstream>
#include <cassert>

using namespace std;

// CSV���u���Ă���t�H���_
const string XIIlib::CSVLoader::directoryPath = "Resources/CSV/";
const string XIIlib::CSVLoader::extension = ".csv";
int XIIlib::CSVLoader::csvArray[BOARD_NUM][BOARD_NUM] = { {} };
int XIIlib::CSVLoader::x = 0;
int XIIlib::CSVLoader::y = 0;

void XIIlib::CSVLoader::CSVLoad(const std::string& fileName, int newLine)
{
	std::string name = fileName;
	if (name == "")
	{
		std::string name = "test1";
	}
	
	// �t���p�X
	const string fullPath = directoryPath + fileName + extension;
	// �t�@�C���X�g���[��
	ifstream file;
	// �t�@�C�����J��
	file.open(fullPath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}
	// 1�s���i�[
	string line;
	// �Ō�̍s�܂Ń��[�v
	while (getline(file, line))
	{
		int count = 0;
		// 1�s���i�[
		vector<string> strvec = Split(line, ',');

		// �c�����Z��
		++y;
		
		// �\��
		for (int i = 0; i < strvec.size(); i++) 
		{
			count++;
			printf("%d", stoi(strvec.at(i)));
			printf(",");
			if (count >= newLine)
			{
				printf("\n");
				count = 0;
			}
		}
	}
	file.close();
}

std::vector<std::string> XIIlib::CSVLoader::Split(std::string& line, char delimiter)
{
	// �X�g���[���ɕϊ�
	istringstream streamLine(line);
	// 1�s�i�[�p
	string str;
	// �z��i�[�p
	vector<string> strDate;
	// ����������
	x = 0;
	// �J���}��؂��
	while (getline(streamLine, str, delimiter))
	{
		csvArray[y][x] = stoi(str);
		strDate.push_back(str);
		// �������Z��
		++x;
	}
	return strDate;
}

std::vector<std::vector<int>> XIIlib::CSVLoader::GetMapVector()
{
	std::vector<std::vector<int>>mapBoard(BOARD_NUM, std::vector<int>(BOARD_NUM));
	for (int y = 0; y < BOARD_NUM; ++y)
	{
		for (int x = 0; x < BOARD_NUM; ++x)
		{
			int temp = 7 - y;
			mapBoard[temp][x] = csvArray[y][x];
		}
	}
	
	return mapBoard;
}

int** XIIlib::CSVLoader::GetMapPointer()
{
	int mapBoard[8][8] = { {} };
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			int temp = BOARD_NUM - y;
			mapBoard[temp][x] = csvArray[y][x];
		}
	}
	return reinterpret_cast<int**>(mapBoard);
}