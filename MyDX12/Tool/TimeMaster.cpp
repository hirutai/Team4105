#include "TimeMaster.h"
#include <math.h>

int XIIlib::IsTimeOut(float* p_time, float add_frame, float limit)
{
	// �������ԂɒB���Ă��邩
	if (*p_time >= limit){
		// time��0�ɂ��Ă���1��Ԃ�
		*p_time = 0.0f;
		return 1;
	}
	// �w��t���[�����i�߂�
	*p_time += add_frame;
	return 0;
}

int XIIlib::HitTime(float prim_time, float division)
{
	if (fmodf(prim_time,division) == 0.0f)return 1;

	return 0;
}
