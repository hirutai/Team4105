#pragma once
#include <string>
#include <vector>

namespace XIIlib
{
	// ���b�Z���W���[�N���X
	class Messenger
	{
	public:// �ÓI�����o�֐�
		static Messenger* GetInstance();
	public:// �����o�֐�
		// ���b�Z�[�W�̒ǉ�
		void AddPrintOut(const std::string& msg);
		void DrawMessage();
	private:
		// �R���X�g���N�^
		Messenger();
		Messenger(const Messenger&) = delete;
		~Messenger();
		Messenger& operator=(const Messenger&) = delete;

		struct MsgAset
		{
			std::string strc;
			float s_time;
			float e_time;
		};

		const float cframe = 1.0f / 60.0f;

	private:
		std::vector<MsgAset> addStr;
	};
}