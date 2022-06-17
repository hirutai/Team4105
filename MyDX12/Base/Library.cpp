#include <time.h>
#include "Library.h"
#include "WindowsApp.h"
#include "DirectX12.h"
#include "../Input/KeyInput.h"
#include "../Input/ControllerInput.h"
#include "../Audio/Sound.h"
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include "../2D/Sprite.h"
#include "../2D/Point2D.h"
#include "../2D/Line2D.h"
#include "../2D/Circle.h"
#include "../Light/LightGroup.h"
#include "../3D/Line.h"
#include "../2D/TextureInstance.h"
#include "../Particle/ParticleManager.h"
#include "../3D/CollCapsule.h"
#include "../3D/CollBox.h"
#include "../3D/CollCircle.h"
#include <sstream>

#pragma comment(lib, "winmm.lib")

using namespace XIIlib;

XIIlib::Library::Library()
{
}

XIIlib::Library::~Library()
{
	//pad_input->Cleanup();

	delete dx12;
	w_app->TerminateWindow();
	delete w_app;
}

int XIIlib::Library::Initialize()
{
	srand(time(NULL));
	// ウィンドウの生成
	w_app = new WindowsApp();
	w_app->GenerateWindow();
	//DirectX初期化処理
	dx12 = new DirectX12();
	dx12->Initialize(w_app);
	// Input初期化
	keyInput = KeyInput::GetInstance();
	keyInput->Initialize(w_app->GetInstance(), w_app->GetWinHandle());

	// その他汎用クラスの初期化
	Sprite::StaticInitialize();
	Point2D::StaticInitialize();
	Line2D::StaticInitialize();
	Circle::StaticInitialize();
	Line::StaticInitialize();
	TextureInstance::StaticInitialize();
	Object3D::StaticInitialize();
	BillObj::StaticInitialize();
	LightGroup::StaticInitialize(DirectX12::GetDevice());
	ParticleManager::GetInstance()->Initialize("effect1.png");
	CollisionCapsule::StaticInitialize();
	CollisionBox::StaticInitialize();
	CollisionCircle::StaticInitialize();

	return 1;
}

int XIIlib::Library::GetQuery()
{
	// メインループに入る前に精度を取得しておく
	if (QueryPerformanceFrequency(&timeFreq) == FALSE) { // この関数で0(FALSE)が帰る時は未対応
		// そもそもQueryPerformanceFrequencyが使えない様な(古い)PCではどうせ色々キツイだろうし
		return(E_FAIL); // 本当はこんな帰り方しては行けない(後続の解放処理が呼ばれない)
	}
	// 1度取得しておく(初回計算用)
	QueryPerformanceCounter(&timeStart);
}

void XIIlib::Library::RunTimer()
{
	// 今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // 時間に余裕がある
		// ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // 分解能を上げる(こうしないとSleepの精度はガタガタ)
		Sleep(sleepTime);   // 寝る
		timeEndPeriod(1);   // 戻す

		// 次週に持ち越し(こうしないとfpsが変になる?)
		return;
	}

	if (frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		fps = (fps * 0.99f) + (0.01f / frameTime); // 平均fpsを計算
#ifdef _DEBUG
// デバッグ用(デバッガにFSP出す)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << fps << " FPS" << std::endl;
		// カウンタ付けて10回に1回出力、とかにしないと見づらいかもね
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}

	timeStart = timeEnd; // 入れ替え
}

bool XIIlib::Library::Message()
{
	return w_app->Message();
}

void XIIlib::Library::InputUpdate()
{
	keyInput->Update();
	//pad_input->Update();
}

void XIIlib::Library::D_Start()
{
	dx12->Previous();
}

void XIIlib::Library::D_End()
{
	dx12->PostProcessing();
}
