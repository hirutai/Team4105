#include "Sound.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

bool Sound::Initialize()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

void Sound::LoadWavFile(const char* filename)
{
	// Wave�t�@�C�����J��
	file.open(filename, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	file.read((char*)&format, sizeof(format));

	// Data�`�����N�̓ǂݍ���
	file.read((char*)&data, sizeof(data));

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Wave�t�@�C�������
	file.close();
}

void Sound::PlayWave(UINT32 loopCount, float Volume)
{
	HRESULT result;


	WAVEFORMATEX wfex{};
	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	buf.LoopCount = loopCount;

	//���ʐݒ��pSourceVoice->SetVolume()�������΂���
	// ���ʂ̈�����͈͂� -2^24 ~ 2^24�� 144.5dB
	// 0������ , 1��100%
	result = pSourceVoice->SetVolume(Volume);

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	result = pSourceVoice->Start();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
}

void Sound::StopWave()
{
	HRESULT result;

	result = pSourceVoice->Stop();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
}