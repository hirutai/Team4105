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
					// delim����؂蕶���Ƃ��Đ؂蕪���Aint�ɕϊ�����data[][]�Ɋi�[����
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

				// filepath�̃t�@�C����W�J(�����ꍇ�͐V���ɍ쐬)
				writing_file.open(filepath, std::ios::out);

				// ������̏�������
				std::string writing_text = "����̓T���v���e�L�X�g�ł��B�ۑ��ł��Ă��邩���m�F���Ă��������B";
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

				// filepath�̃t�@�C����W�J(�����ꍇ�͐V���ɍ쐬)
				// std::ios::app -> �t�@�C���̖������珑�����݊J�n
				writing_file.open(filepath, std::ios::app);

				// ������̏�������
				std::string writing_text = "���̕��͒ǉ��ŏ������񂾕��ɂȂ��Ă���z��ł��B�m�F���Ă��������B";
				writing_file << writing_text << std::endl;

				writing_file.close();
			}

		}
	}
}