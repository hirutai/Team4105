#include "GameLoader.h"

using namespace XIIlib;

XIIlib::GameLoader::GameLoader()
{}

void XIIlib::GameLoader::LoadFile(const std::string& filename)
{
	FILE* fp = nullptr;
	// �t�@�C���p�X�쐬
	std::string filepath = filename + ".bin";
	// �t�@�C�����J��(�ǂݍ���)
	fopen_s(&fp, filepath.c_str(), "rb");
	// �I�u�W�F�N�g���̓ǂݍ���
	fread(&objSize, sizeof(unsigned int), 1, fp);

	read_data.resize(objSize);
	// �I�u�W�F�N�g���̏�������
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
