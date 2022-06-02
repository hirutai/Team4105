#pragma once
#include <string>
#include <vector>

namespace XIIlib
{
	// メッセンジャークラス
	class Messenger
	{
	public:// 静的メンバ関数
		static Messenger* GetInstance();
	public:// メンバ関数
		// メッセージの追加
		void AddPrintOut(const std::string& msg);
		void DrawMessage();
	private:
		// コンストラクタ
		Messenger();
		Messenger(const Messenger&) = delete;
		~Messenger();
		Messenger& operator=(const Messenger&) = delete;

		struct MsgAset
		{
			std::string strc;
			float s_time;
			float e_time;
		};

		const float cframe = 1.0f / 60.0f;

	private:
		std::vector<MsgAset> addStr;
	};
}