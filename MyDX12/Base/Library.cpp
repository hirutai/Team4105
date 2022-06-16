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
