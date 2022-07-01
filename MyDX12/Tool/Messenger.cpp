#include "Messenger.h"
#include <iostream>
#include "DebugJISText.h"
#include "Easing.h"
#include <algorithm>
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
{	// 任意位置への挿入は別(vector以外)のコンテナのほうが早い
	//...が要素数がそこまで無いならvectorでもあまり変わらないらしい。
	const float c_init = 0.0f;
	addStr.insert(addStr.begin(), { msg ,c_init,c_init });
}

void Messenger::DrawMessage()
{
	const float fSize = 32.0f;// 文字デフォルトサイズ
	const float scfont = 0.8f;// 文字のスケール値
	const float wy = 720.0f;// ウィンドウサイズy
	const double endtime = 0.5;
	const float limit = 8.0f;
	// エンドタイムが1を超えたら消す
	auto removeIt = std::remove_if(addStr.begin(), addStr.end(), [&](MsgAset itr)
		{
			return itr.s_time >= limit;
		}
	);
	addStr.erase(removeIt, addStr.end());
	// 描画するスプライトの準備
	for (int i = 0; i < addStr.size(); i++)
	{
		// 全角文字数の取得
		unsigned int strSize = addStr[i].strc.length() / 2;
		// 開始位置の計算
		float start = (fSize * scfont) * strSize;

		float m_x;
		// 終点時間を超えたら位置を0で固定
		if (addStr[i].s_time >= endtime)
			m_x = start;
		// 終点時間を過ぎていなければ
		else
			m_x = (float)InOutCubic((double)addStr[i].s_time, 0, (double)(start), endtime);

		addStr[i].s_time += cframe;
		// 7秒あたりでエンドカウントをスタートする
		if (addStr[i].s_time >= 7.5f)
		{
			m_x = (float)InOutCubic((double)addStr[i].e_time, (double)(start), (double)(-start), endtime);
			addStr[i].e_time += cframe;
		}
		// 順番に情報を入れていく
		DebugJISText::GetInstance()->Print(addStr[i].strc, m_x - start, wy - (fSize * scfont * (i + 1)), scfont);
	}
}

Messenger::~Messenger(){}