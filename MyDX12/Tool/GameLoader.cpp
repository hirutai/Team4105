#include "GameLoader.h"

using namespace XIIlib;

XIIlib::GameLoader::GameLoader()
{}

void XIIlib::GameLoader::LoadFile(const std::string& filename)
{
	FILE* fp = nullptr;
	// ファイルパス作成
	std::string filepath = filename + ".bin";
	// ファイルを開く(読み込み)
	fopen_s(&fp, filepath.c_str(), "rb");
	// オブジェクト数の読み込み
	fread(&objSize, sizeof(unsigned int), 1, fp);

	read_data.resize(objSize);
	// オブジェクト情報の書き込み
	fread(read_data.data(), sizeof(ObjectInfomations), read_data.size(), fp);

	fclose(fp);
}

unsigned int XIIlib::GameLoader::GetOJBSize()const
{
	return objSize;
}

ObjectInfomations XIIlib::GameLoader::GetObjects(int index)const
{
	return read_data[index];
}

void XIIlib::GameLoader::ReadClear()
{
	read_data.clear();
}
