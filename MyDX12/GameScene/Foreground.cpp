#include "Foreground.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

void XIIlib::Foreground::Initialize()
{
	// サイズを設定
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;

	foreground.resize(s_y);
	for (int i = 0; i < s_y; i++) {
		foreground.at(i).resize(s_x);
	}

	// テクスチャも含めて初期化
	foreground_tex.resize(s_y * s_x);
	time_foreground.resize(s_y * s_x);
	for (int i = 0; i < s_y; ++i)
	{
		for (int j = 0; j < s_x; ++j) {
			foreground[i][j] = 0;
			foreground_tex[j + (i * s_x)] =
				Sprite::Create(4, { (float)(j * tex_size) + (float)(tex_size / 2.0f),(float)(i * tex_size) + (float)(tex_size / 2.0f) }, { 1,1,1,1 }, { 0.5f,0.5f });
			foreground_tex[j + (i * s_x)]->SetSize({ 0.0f,0.0f });
			time_foreground[j + (i * s_x)] = 0.0f;
		}
	}
}

XIIlib::Foreground::Foreground()
{}

XIIlib::Foreground::~Foreground()
{
	for (auto x : foreground_tex) {
		delete x;
	}
}

unsigned int XIIlib::Foreground::ChangeFront(unsigned int _change)
{
	const unsigned int tex_size = 128;
	const unsigned int s_y = 768 / tex_size;
	const unsigned int s_x = 1280 / tex_size;

	const int in_change = 1;
	const int closed = 0;
	const int opened = 2;
	unsigned int completed = 1, incomplete = 0;
	// クローズ
	if (!_change) {
		struct Index2 {
			int y, x;// 要素数の格納用構造体
		};
		std::vector<Index2> indecis;

		for (int i = 0; i < s_y; ++i)
		{
			for (int j = 0; j < s_x; ++j) {
				if (foreground[i][j] == closed) {
					indecis.push_back({ i,j });
				}
			}
		}

		int arraysize = indecis.size();
		if (arraysize != 0) {
			Index2 ind2 = indecis[rand() % arraysize];
			foreground[ind2.y][ind2.x] = in_change;
			// デフォルトカラー
			foreground_tex[ind2.x + (ind2.y * s_x)]->SetColor(1, 1, 1, 1);
		}

		const float c_frame = 1.0f / 60.0f;

		for (int i = 0; i < s_y; ++i) {
			for (int j = 0; j < s_x; ++j) {
				if (foreground[i][j] == in_change)
				{
					const float start = 0.0f;
					const float end = 0.5f;
					float newSize = (float)Easing::EaseOutBack((double)time_curtain[j + (i * s_x)], (double)start, (double)tex_size, (double)end);
					// 1フレーム分進める
					time_curtain[j + (i * s_x)] += c_frame;
					foreground_tex[j + (i * s_x)]->SetSize({ newSize ,newSize });
					if (time_curtain[j + (i * s_x)] >= end) {
						time_curtain[j + (i * s_x)] = start;
						foreground[i][j] = opened;
					}
				}
			}
		}

		for (auto y : foreground) {
			for (auto x : y) {
				if (x == closed || x == in_change)return incomplete;
			}
		}
		return completed;
	}
	// オープン
	else {
		struct Index2 {
			int y, x;// 要素数の格納用構造体
		};
		std::vector<Index2> indecis;

		for (int i = 0; i < s_y; ++i)
		{
			for (int j = 0; j < s_x; ++j) {
				if (foreground[i][j] == opened) {
					indecis.push_back({ i,j });
				}
			}
		}

		int arraysize = indecis.size();
		if (arraysize != 0) {
			Index2 ind2 = indecis[rand() % arraysize];
			foreground[ind2.y][ind2.x] = in_change;
		}

		const float c_frame = 1.0f / 60.0f;

		for (int i = 0; i < s_y; ++i) {
			for (int j = 0; j < s_x; ++j) {
				if (foreground[i][j] == in_change)
				{
					const float start = 0.0f;
					const float end = 0.5f;
					float newSize = (float)Easing::EaseInBack((double)time_curtain[j + (i * s_x)], (double)start, (double)tex_size, (double)end);
					// 1フレーム分進める
					time_curtain[j + (i * s_x)] += c_frame;
					float result = (float)tex_size - newSize;
					foreground_tex[j + (i * s_x)]->SetSize({ result ,result });
					if (time_curtain[j + (i * s_x)] >= end) {
						time_curtain[j + (i * s_x)] = start;
						// 透過色
						foreground_tex[j + (i * s_x)]->SetColor(0, 0, 0, 0);
						foreground[i][j] = closed;
					}
				}
			}
		}

		for (auto y : foreground) {
			for (auto x : y) {
				if (x == in_change || x == opened)return incomplete;
			}
		}

		return completed;
	}
}

void XIIlib::Foreground::Draw()
{
	for (auto tex : foreground_tex) {
		tex->Draw();
	}
}