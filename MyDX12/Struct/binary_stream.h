#pragma once
#include <iostream>
#include <string>
#include <fstream>

// 20 * 15
namespace XIIlib
{
	namespace BINARY_STREAM
	{
		namespace CSV
		{
			void
				LoadCSV_MAP(int* arr, std::string filename, int size)
			{
				std::ifstream stream(filename);
				std::string line;
				const std::string delim = ",";

				int row = 0;
				int col;
				while (std::getline(stream, line))
				{
					col = 0;
					// delimを区切り文字として切り分け、intに変換してdata[][]に格納する
					for (std::string::size_type spos, epos = 0;
						(spos = line.find_first_not_of(delim, epos)) != std::string::npos;) {
						std::string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);

						*(arr + row * size + col) = stoi(token);
						col++;
					}
					++row;
				}
			}

		}

		namespace TXT
		{
			void
				FileWrite(std::string& filename)
			{
				const std::string directpath = "Resources/Data/";
				const std::string extension = ".txt";
				std::ofstream writing_file;
				std::string filepath = directpath + filename + extension;

				// filepathのファイルを展開(無い場合は新たに作成)
				writing_file.open(filepath, std::ios::out);

				// 文字列の書き込み
				std::string writing_text = "これはサンプルテキストです。保存できているかを確認してください。";
				writing_file << writing_text << std::endl;

				writing_file.close();
			}

			void
				FileOverWrite(std::string& filename)
			{
				const std::string directpath = "Resources/Data/";
				const std::string extension = ".txt";
				std::ofstream writing_file;
				std::string filepath = directpath + filename + extension;

				// filepathのファイルを展開(無い場合は新たに作成)
				// std::ios::app -> ファイルの末尾から書き込み開始
				writing_file.open(filepath, std::ios::app);

				// 文字列の書き込み
				std::string writing_text = "この文は追加で書き込んだ分になっている想定です。確認してください。";
				writing_file << writing_text << std::endl;

				writing_file.close();
			}

		}
	}
}