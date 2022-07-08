#include "HPBar.h"

#include "../3D/BillObj.h"

using namespace XIIlib;
using namespace Math;

HPBar* XIIlib::HPBar::GetInstance()
{
	static XIIlib::HPBar instance;
	return &instance;
}

HPBar::HPBar()
{
	//if (type == CountType::FRAME)
	//{
	//	countingNum = countingNum_; // カウントしたい数の入力
	//}
	//else if (type == CountType::SECOND)
	//{
	//	countingNum = second * countingNum_; // カウントしたい数の入力
	//}

}

void HPBar::Initialize()
{
	//decNum = maxSize / countingNum; //  // 減らす量の計算
	timerNum = 0;
	currentSize = maxSize;

	timerBar = BillObj::Create({}, "timer_bar.png"); // バーのオブジェクトの作成
	timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // バーのサイズの設定
	timerEdge = BillObj::Create({}, "timer_edge.png"); // 縁のオブジェクトの作成
	timerEdge->SetSize(edgeSize); // 縁のサイズの設定
	timerBar->SetPosition(-15, 0, 30); // バーの座標の設定
	timerEdge->SetPosition(0, 0, 30); // 縁の座標の設定
}

void XIIlib::HPBar::Timer(float HP)
{
	//sumPoint += HP / 2.0f;
	//if (SizeZeroFlag()) // サイズが０になった時
	//{
	//}
	currentSize = maxSize - (60 - HP) / 2.0f; // 現在のサイズを計算

	barSize.x = currentSize; // バーのサイズの設定

	timerBar->SetSize(barSize); // バーのサイズの反映
}

void HPBar::Draw()
{
	timerBar->Draw(); // バーの座標の描画
	timerEdge->Draw(); // 縁の座標の描画
}

void HPBar::SetPosition(const Vector3& position)
{
	timerBar->SetPosition(position.x - 1.5f, position.y + 4, position.z - 1); // バーの座標の設定
	timerEdge->SetPosition(position.x, position.y + 4, position.z - 1); // 縁の座標の設定
}

bool XIIlib::HPBar::SizeZeroFlag()
{
	return currentSize <= 0;
}
bool XIIlib::HPBar::SizeThirdFlag()
{
	return currentSize == maxSize / 3;
}
bool XIIlib::HPBar::SizeThirdBelowFlag()
{
	return currentSize <= maxSize / 3;
}