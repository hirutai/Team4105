#include "Curtain.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::Curtain* XIIlib::Curtain::Create(unsigned int textureNum, float x, float y)
{
	Curtain* pCurtain = new Curtain(textureNum,x,y);
	if (pCurtain == nullptr) {
		return nullptr;
	}

	pCurtain->Initialize();

	return pCurtain;
}

XIIlib::Curtain::Curtain(unsigned int textureNum, float x, float y)
{
	tex = Sprite::Create(textureNum, { x,y }, { 1,1,1,1 }, { 0.5f,0.5f });
	tex->SetSize({0.0f, 0.0f});
}

XIIlib::Curtain::~Curtain()
{
	delete tex;
}

void XIIlib::Curtain::Initialize()
{
	ResetTime();
	state = OPEN;
}

void XIIlib::Curtain::Draw()
{
	// テクスチャの描画
	tex->Draw();
}

void XIIlib::Curtain::AddFrame()
{
	// タイマーの進行
	time += (1.0f / 60.0f);
}

void XIIlib::Curtain::ResetTime()
{
	// 零にリセット
	time = 0.0f;
}

void XIIlib::Curtain::Open()
{
	// ACTIVEで無ければ飛ばす
	if (GetState() != ACTIVE)return;
	// サイズの値を計算
	float newSize = (float)Easing::EaseInBack((double)GetTime(), 0, (double)texSize, (double)maxTime);
	// 時間を進める
	AddFrame();
	// 結果を取得
	float result = (float)texSize - newSize;
	// サイズを更新
	SetSize(result, result);

	// 時間が立たない限りは飛ばす
	if (GetTime() < maxTime)return;
	ResetTime();
	SetState(OPEN);
	SetAlpha(0.0f);
}

void XIIlib::Curtain::Close()
{
	// ACTIVEで無ければ飛ばす
	if (GetState() != ACTIVE)return;
	// サイズの値を計算
	float newSize = (float)Easing::EaseOutBack((double)GetTime(), 0, (double)texSize, (double)maxTime);
	// 時間を進める
	AddFrame();
	// サイズを更新
	SetSize(newSize, newSize);
	// 時間が立たない限りは飛ばす
	if (GetTime() < maxTime)return;
	ResetTime();
	SetState(CLOSE);
}

void XIIlib::Curtain::SetState(const TexState& cState)
{
	// 状態の更新
	state = cState;
}

void XIIlib::Curtain::SetSize(float x, float y)
{
	// テクスチャサイズの更新
	tex->SetSize({x,y});
}

void XIIlib::Curtain::SetAlpha(float a)
{
	tex->SetColor(1,1,1,a);
}
