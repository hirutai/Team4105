#include "Messenger.h"
#include <iostream>
#include "DebugJISText.h"
#include "Easing.h"
using namespace std;
using namespace Easing;
using namespace XIIlib;

Messenger::Messenger(){}

Messenger* Messenger::GetInstance()
{
	static Messenger instance;
	return &instance;
}

void Messenger::AddPrintOut(const std::string& msg)
{	// �C�ӈʒu�ւ̑}���͕�(vector�ȊO)�̃R���e�i�̂ق�������
	//...���v�f���������܂Ŗ����Ȃ�vector�ł����܂�ς��Ȃ��炵���B
	const float c_init = 0.0f;
	addStr.insert(addStr.begin(), { msg ,c_init,c_init });
}

void Messenger::DrawMessage()
{
	const float fSize = 32.0f;// �����f�t�H���g�T�C�Y
	const float scfont = 0.8f;// �����̃X�P�[���l
	const float wy = 720.0f;// �E�B���h�E�T�C�Yy
	const double endtime = 0.5;
	const float limit = 8.0f;
	// �G���h�^�C����1�𒴂��������
	auto removeIt = std::remove_if(addStr.begin(), addStr.end(), [&](MsgAset itr)
		{
			return itr.s_time >= limit;
		}
	);
	addStr.erase(removeIt, addStr.end());
	// �`�悷��X�v���C�g�̏���
	for (int i = 0; i < addStr.size(); i++)
	{
		// �S�p�������̎擾
		unsigned int strSize = addStr[i].strc.length() / 2;
		// �J�n�ʒu�̌v�Z
		float start = (fSize * scfont) * strSize;

		float m_x;
		// �I�_���Ԃ𒴂�����ʒu��0�ŌŒ�
		if (addStr[i].s_time >= endtime)
			m_x = start;
		// �I�_���Ԃ��߂��Ă��Ȃ����
		else
			m_x = (float)InOutCubic((double)addStr[i].s_time, 0, (double)(start), endtime);

		addStr[i].s_time += cframe;
		// 7�b������ŃG���h�J�E���g���X�^�[�g����
		if (addStr[i].s_time >= 7.5f)
		{
			m_x = (float)InOutCubic((double)addStr[i].e_time, (double)(start), (double)(-start), endtime);
			addStr[i].e_time += cframe;
		}
		// ���Ԃɏ������Ă���
		DebugJISText::GetInstance()->Print(addStr[i].strc, m_x - start, wy - (fSize * scfont * (i + 1)), scfont);
	}
}

Messenger::~Messenger(){}