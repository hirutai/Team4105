#include "HPBar.h"

//#include "../3D/BillObj.h"
#include "../2D/Sprite.h"
#include "../2D/SpriteLoader.h"

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

	//timerBar = BillObj::Create({}, "timer_bar.png"); // バーのオブジェクトの作成
	//timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	//timerBar->SetSize(barSize); // バーのサイズの設定
	//timerEdge = BillObj::Create({}, "timer_edge.png"); // 縁のオブジェクトの作成
	//timerEdge->SetSize(edgeSize); // 縁のサイズの設定
	//timerBar->SetPosition(hpBarPos.x - barSize.x / 2, hpBarPos.y, hpBarPos.z); // バーの座標の設定(-はズレの修正)
	//timerEdge->SetPosition(hpBarPos.x, hpBarPos.y, hpBarPos.z); // 縁の座標の設定
	timerBar = Sprite::Create(TIMER_BAR, {}); // バーのオブジェクトの作成
	//timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // バーのサイズの設定
	timerEdge = Sprite::Create(TIMER_EDGE, {}); // 灰色のオブジェクトの作成
	timerEdge->SetSize(edgeSize); // 縁のサイズの設定
	timerBar->SetPosition({ hpBarPos.x + 10.0f, hpBarPos.y + 10.0f }); // バーの座標の設定(-はズレの修正)
	timerEdge->SetPosition({ hpBarPos.x, hpBarPos.y}); // 縁の座標の設定
}

void XIIlib::HPBar::Timer(int HP)
{
	decNum = HP * 40; // 減らす量の計算

	currentSize = maxSize - (maxSize - HP * 40); // 現在のサイズを計算

	barSize.x = currentSize; // バーのサイズの設定

	if (barSize.x < 0) {
		barSize.x = 0;
	}

	timerBar->SetSize(barSize); // バーのサイズの反映
}

void HPBar::Draw()
{
	timerEdge->Draw(); // 縁の座標の描画
	timerBar->Draw(); // バーの座標の描画
}

void HPBar::SetPosition(const Vector3& position)
{
	timerBar->SetPosition({ position.x - 1.5f, position.y + 4 }); // バーの座標の設定
	timerEdge->SetPosition({ position.x, position.y + 4 }); // 縁の座標の設定
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