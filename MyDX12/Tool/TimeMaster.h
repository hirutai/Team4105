#pragma once

namespace XIIlib
{
	// ��萔�ɓ��B���Ă��邩�ǂ����̃J�E���g�֐�
	int IsTimeOut(float* p_time,float add_frame, float limit);

	// �����ɐݒ肵���l�Ŋ����Ă��܂�0�ł����1��Ԃ�
	int HitTime(float prim_time, float division);
}