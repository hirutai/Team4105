#pragma once
#include <string>
#include <vector>

namespace XIIlib
{
	class CSVLoader
	{
	private:
		enum class Type
		{
			NORMAL,
			MAP_BORAD,
		};
	public:
		const static int BOARD_NUM = 8;
	private: // �ÓI�����o�萔
		// CSV���u���Ă���t�H���_
		static const std::string directoryPath;
		// �g���q
		static const std::string extension;
		// �i�[�p
		static std::vector<std::string> stringMap;
		// 2�����z��
		static int csvArray[BOARD_NUM][BOARD_NUM];
		static std::vector<std::vector<int>> csvVector[BOARD_NUM][BOARD_NUM];
		static int x , y ;
	public:
		// CSV�̓ǂݍ���
		static void CSVLoad(const std::string& fileName,int newLine = 64);
		// �����񕪊�
		static std::vector<std::string> Split(std::string& line, char delimiter);

	public:
		static std::vector<std::vector<int>> GetMapVector();
		static int** GetMapPointer();

	};
}
