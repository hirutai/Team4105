#pragma once
#pragma comment (lib, "xinput.lib")
#include <Windows.h>
#include <Xinput.h>

// 名前簡略化
enum XinputButtonMask
{
	X_UP = XINPUT_GAMEPAD_DPAD_UP,          // 0x0001 
	X_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,      // 0x0002
	X_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,      // 0x0004
	X_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,    // 0x0008
	X_START = XINPUT_GAMEPAD_START,         // 0x0010
	X_BACK = XINPUT_GAMEPAD_BACK,           // 0x0020 
	X_THUMBL = XINPUT_GAMEPAD_LEFT_THUMB,   // 0x0040 Lスティック押し込み
	X_THUMBR = XINPUT_GAMEPAD_RIGHT_THUMB,  // 0x0080 Rスティック押し込み
	X_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,    // 0x0100
	X_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,   // 0x0200 
	X_A = XINPUT_GAMEPAD_A,                 // 0x1000 Aボタン
	X_B = XINPUT_GAMEPAD_B,                 // 0x2000 Bボタン
	X_X = XINPUT_GAMEPAD_X,                 // 0x4000 Xボタン
	X_Y = XINPUT_GAMEPAD_Y,                 // 0x8000 Yボタン
};

class GamePAD_XInput
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(int gamePadNum = 0);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

#pragma region ボタン全般

	/// <summary>
	/// ボタンを押してる間
	/// </summary>
	/// <param name="buttonNumber">確認したいボタンの番号</param>
	/// <returns>成否</returns>
	bool PressButton(int buttonNumber);

	/// <summary>
	/// ボタンを押した瞬間
	/// </summary>
	/// <param name="buttonNumber">確認したいボタンの番号</param>
	/// <returns>成否</returns>
	bool Button_Down(int buttonNumber);

	/// <summary>
	/// ボタンを離した瞬間
	/// </summary>
	/// <param name="buttonNumber">確認したいボタンの番号</param>
	/// <returns>成否</returns>
	bool Button_Up(int buttonNumber);

	/// <summary>
	/// 何らかのボタンが押されているか
	/// </summary>
	/// <returns>成否</returns>
	bool PressButtonAll();

#pragma endregion

#pragma region 右スティック

	// 右スティックが倒れているか
	bool RStickDown();
	// 右スティックを倒した瞬間　前
	bool RStickDownFront();
	// 右スティックを倒した瞬間　後
	bool RStickDownBack();
	// 右スティックを倒した瞬間　左
	bool RStickDownLeft();
	// 右スティックを倒した瞬間　右
	bool RStickDownRight();
	// 右スティックの横軸　-1.0f～1.0f を返す　左 < 0 < 右
	float RStickXaxis();
	// 右スティックの縦軸　-1.0f～1.0f を返す　下 < 0 < 上
	float RStickYaxis();

#pragma endregion

#pragma region 左スティック

	// 左スティックが倒れているか
	bool LStickDown();
	// 左スティックを倒した瞬間　前
	bool LStickDownFront();
	// 左スティックを倒した瞬間　後
	bool LStickDownBack();
	// 左スティックを倒した瞬間　左
	bool LStickDownLeft();
	// 左スティックを倒した瞬間　右
	bool LStickDownRight();

	// 左スティックの横軸　-1.0f～1.0f を返す　左 < 0 < 右
	float LStickXaxis();
	// 左スティックの縦軸　-1.0f～1.0f を返す　下 < 0 < 上
	float LStickYaxis();

#pragma endregion

#pragma region 右トリガー

	// 右トリガー(LT)押している間
	bool TriggerRPress();
	// 右トリガー(RT)押した瞬間
	bool TriggerRDown();
	// 右トリガー(RT)離した瞬間
	bool TriggerRUp();
	// 右トリガー(RT)の押し込み　0～1.0f を返す
	float TriggerRDegree();

#pragma endregion

#pragma region 左トリガー

	// 左トリガー(LT)押している間
	bool TriggerLPress();
	// 左トリガー(LT)押した瞬間
	bool TriggerLDown();
	// 左トリガー(LT)離した瞬間
	bool TriggerLUp();
	// 左トリガー(LT)の押し込み　0～1.0f を返す
	float TriggerLDegree();

#pragma endregion

#pragma region 振動

	// 振動
	void Vibration();
	// 振動 右だけ
	void VibrationR();
	// 振動 左だけ
	void VibrationL();
	// 振動を止める
	void VibrationReset();

	// 振動の大きさ変更
	inline void SetVibrationLevel(int vibLv) { GamePAD_XInput::vibrationLevel = vibLv; }

#pragma endregion

private:
#pragma region 押した瞬間の判定用関数

	float PreStickRX();
	float PreStickRY();
	float PreStickLX();
	float PreStickLY();
	float PreTriggerR();
	float PreTriggerL();

#pragma endregion

private: // メンバ変数
	// 構造体のインスタンス
	XINPUT_STATE XinputState;
	// 前フレームの状態取得
	XINPUT_STATE preXinputState;
	// 振動の大きさ 0～65535(2＾16)
	static int vibrationLevel;
};




