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
	private: // 静的メンバ定数
		// CSVが置いてあるフォルダ
		static const std::string directoryPath;
		// 拡張子
		static const std::string extension;
		// 格納用
		static std::vector<std::string> stringMap;
		// 2次元配列
		static int csvArray[BOARD_NUM][BOARD_NUM];
		static std::vector<std::vector<int>> csvVector[BOARD_NUM][BOARD_NUM];
		static int x , y ;
	public:
		// CSVの読み込み
		static void CSVLoad(const std::string& fileName,int newLine = 64);
		// 文字列分割
		static std::vector<std::string> Split(std::string& line, char delimiter);

	public:
		static std::vector<std::vector<int>> GetMapVector();
		static int** GetMapPointer();

	};
}
