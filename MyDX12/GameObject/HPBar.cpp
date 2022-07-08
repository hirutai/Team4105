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
}

void HPBar::Initialize()
{
	currentSize = maxSize;

	timerBar = BillObj::Create({}, "timer_bar.png"); // バーのオブジェクトの作成
	timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // バーのサイズの設定
	timerEdge = BillObj::Create({}, "timer_edge.png"); // 縁のオブジェクトの作成
	timerEdge->SetSize(edgeSize); // 縁のサイズの設定
	timerBar->SetPosition(hpBarPos.x - barSize.x / 2, hpBarPos.y, hpBarPos.z); // バーの座標の設定(-はズレの修正)
	timerEdge->SetPosition(hpBarPos.x, hpBarPos.y, hpBarPos.z); // 縁の座標の設定
}

void XIIlib::HPBar::Timer(float damage)
{
	decNum = damage; // 減らす量の計算

	currentSize = currentSize - decNum; // 現在のサイズを計算

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