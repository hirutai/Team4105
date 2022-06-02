#pragma once

namespace XIIlib
{
	// 一定数に到達しているかどうかのカウント関数
	int IsTimeOut(float* p_time,float add_frame, float limit);

	// 引数に設定した値で割ってあまり0であれば1を返す
	int HitTime(float prim_time, float division);
}