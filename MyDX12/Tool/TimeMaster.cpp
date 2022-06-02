#include "TimeMaster.h"
#include <math.h>

int XIIlib::IsTimeOut(float* p_time, float add_frame, float limit)
{
	// 制限時間に達しているか
	if (*p_time >= limit){
		// timeを0にしてから1を返す
		*p_time = 0.0f;
		return 1;
	}
	// 指定フレーム数進める
	*p_time += add_frame;
	return 0;
}

int XIIlib::HitTime(float prim_time, float division)
{
	if (fmodf(prim_time,division) == 0.0f)return 1;

	return 0;
}
