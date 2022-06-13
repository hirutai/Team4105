#include "GameApp.h"
#include "../Light/LightGroup.h"
#include "../2D/Sprite.h"
#include "../Tool/DebugText.h"
#include "../Tool/DebugJISText.h"
#include "../Tool/DigitalNumberText.h"
#include "../Tool/Messenger.h"
#include "../Camera/DebugCamera.h"
#include "../3D/Object3D.h"
#include "../Particle/ParticleManager.h"
#include "../2D/Line2D.h"
#include "../3D/Line.h"
#include "../GameObject/UnitManager.h"
#include "../GameObject/AttackAreaManager.h"
#include "../3D/CollCapsule.h"
#include "../3D/CollBox.h"
#include "../3D/CollCircle.h"
#include "../GameScene/GameScene.h"
#include "../GameScene/SceneState.h"
/*仮置き*/
#include "../System/RenderTarget.h"
#include "../2D/Point2D.h"
#include "../2D/Circle.h"
#include "../3D/Tessellation.h"
#include "../2D/TextureInstance.h"
#include "../2D/ShadowMap.h"
#include <iostream>
#include <string>

#include "../3D/Object3D.h"
/*仮置き(ここまで)*/

using namespace Math;
using namespace XIIlib;

//int		MouseInput;
//int		MouseInputLeft;
//Math::Vector2 MousePosInScreen;
//Math::Vector2 MousePos;

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
	delete game_scene;
	delete circle;
	delete l3z;
	delete l3y;
	delete l3x;
	delete lx;
	delete line;
	delete lightGroup;
	delete d_camera;
}

void GameApp::Initialize()
{
	manager = UnitManager::GetInstance();
	attackAreaManager = AttackAreaManager::GetInstance();

	DirectX12::SetClearColor(0.2f, 0.3f, 0.7f);

	//camera = new Camera();
	//camera->_Initialize(100.0f, 0.05f, 10.0f);
	//// カメラ注視点をセット
	//camera->SetTarget(Math::Vector3(0, 1, 0));

	d_camera = new DebugCamera();
	d_camera->_Initialize(100.0f, 0.05f, 10.0f);
	d_camera->SetLookAtRange(0, 1, 0);

	// ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	Object3D::SetDebugCamera(d_camera);
	Object3D::SetLightGroup(lightGroup);

	//lightGroup->SetSpotLightActive(0, true);

	Line::SetDebugCamera(d_camera);
	CollisionCapsule::SetDebugCamera(d_camera);
	CollisionBox::SetDebugCamera(d_camera);
	CollisionCircle::SetDebugCamera(d_camera);
	Tessellation::SetDebugCamera(d_camera);

	Sprite::LoadTexture(debugTextTexNumber, L"Resources/ASCIItex_ver2.png");

	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	particles = ParticleManager::GetInstance();
	particles->SetDebugCamera(d_camera);
	d_camera->_Update();
	int texind = 2;
	Sprite::LoadTexture(debugJISTextTexNumber, L"Resources/JIS_Code_ver.1.02.png");
	Sprite::LoadTexture(texind, L"Resources/digitalNum.png");					texind++;
	Sprite::LoadTexture(texind, L"Resources/effect1.png");						texind++;
	Sprite::LoadTexture(texind, L"Resources/timer.png"); texind++;
	Sprite::LoadTexture(texind, L"Resources/timer_bar.png"); texind++;

	debugJIS = DebugJISText::GetInstance();
	debugJIS->Initialize(debugJISTextTexNumber);

	digitalN = DigitalNumberText::GetInstance();
	digitalN->Initialize(2);

	msger = XIIlib::Messenger::GetInstance();

	// 3Dオブジェクト生成とモデルのセット
	float wid = WindowsApp::window_width;
	float hig = WindowsApp::window_height;
	Math::Vector2 st0 = { wid / 2,0 };
	Math::Vector2 st1 = { 0,hig / 2 };
	Math::Vector2 ed0 = { wid / 2,hig };
	Math::Vector2 ed1 = { wid,hig / 2 };

	line = Line2D::Create(st0, ed0);
	lx = Line2D::Create(st1, ed1);

	Math::Vector3 st3_0 = { 100,0.0f,0.0f };
	Math::Vector3 st3_1 = { 0.0f,100,0.0f };
	Math::Vector3 st3_2 = { 0.0f,0.0f,100 };
	Math::Vector3 ed3_0 = { -100,0.0f,0.0f };
	Math::Vector3 ed3_1 = { 0.0f,-100,0.0f };
	Math::Vector3 ed3_2 = { 0.0f,0.0f,-100 };

	l3x = Line::Create(st3_0, ed3_0);
	l3y = Line::Create(st3_1, ed3_1);
	l3z = Line::Create(st3_2, ed3_2);

	circle = Circle::Create();

	manager->Initialize();
	attackAreaManager->Initialize();

	VariableInit();
	game_scene = new XIIlib::GameScene();
	game_scene->Initialize();

	//リソース
	attack = Object3D::Create(Model::CreateFromOBJ("Attack"));
}

void GameApp::VariableInit()
{
	d_camera->SetPosition(0.0f, 100.0f, 0.0f);
	d_camera->SetLookAtRange(0, -1, 0);
}

void GameApp::Update()
{
	lightGroup->Update();

	// カメラの動きの処理
	Math::Vector3 c_pos = { 0,48,0 };
	d_camera->SetPosition(c_pos.x, c_pos.y, c_pos.z);
	d_camera->SetLookAtRange(0,-1, 0);

	// タグネーム「Player」が存在するかどうか
	// カメラ更新
	d_camera->_Update();

	particles->Update();
	// Messageの表示(中身はDebugTextのPrint)
	XIIlib::Messenger::GetInstance()->DrawMessage();

	game_scene->Update();
	circle->DrawCircle(10,32 * 16 + 16,10,0,255,255,255);
	circle->DrawCircle(10,32 * 17 + 16,10,0,255,0,255);
	circle->DrawCircle(10,32 * 18 + 16,10,255,255,0,255);

	attack->Update();
}

void GameApp::Draw3D()
{
	//game_scene->Draw();
	
	// パーティクルの描画
	particles->Draw();
}

void GameApp::LineDraw3D()
{
	if (!m_move)
	{
		Line::PreDraw();

		Line::PostDraw();
	}
}

void GameApp::DrawImGui()
{
	// ImGui処理
}

void GameApp::BackSc()
{
	Sprite::PreDraw();


	Sprite::PostDraw();
	DirectX12::ClearDepthBuffer();
}

void GameApp::FrontSc()
{
	// 二次元の点
	Point2D::PreDraw();
	
	Point2D::PostDraw();

	// 二次元の線
	Line2D::PreDraw();
	
	Line2D::PostDraw();

	// 円
	Circle::PreDraw();
	circle->Draw();
	Circle::PostDraw();

	// 文字スプライト
	Sprite::PreDraw();
	
	debugText->DrawAll();
	debugJIS->DrawAll();
	digitalN->DrawAll();
	game_scene->DrawSprite();

	Sprite::PostDraw();

	// インスタンステクスチャ
	TextureInstance::PreDraw();
	
	TextureInstance::PostDraw();
}

void GameApp::Draw()
{
	//Draw3D();
}

void GameApp::ObjMDraw()
{
	game_scene->Draw();
	Object3D::PreDraw();
	//attack->Draw();
	Object3D::PostDraw();
}

void GameApp::DestoryAfter()
{
	manager->DeadUnit();
}