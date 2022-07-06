#pragma once
#include <string>
#include <memory>

class DebugCamera;
class LightGroup;
class DebugText;
class DebugJISText;
class DigitalNumberText;
class ParticleManager;
class Line2D;
class Line;
class Circle;

class Object3D;

namespace XIIlib
{
	class UnitManager;
	class GameScene;
	class Messenger;
	class AttackAreaManager;
	class HPBar;
}

class GameApp
{
private:
	static const int debugTextTexNumber = 0;
	static const int debugJISTextTexNumber = 1;

public:
	GameApp();
	~GameApp();

	void Initialize();

	void VariableInit();

	void Update();

	void Draw3D();

	void LineDraw3D();

	void DrawImGui();

	void BackSc();
	void FrontSc();

	void Draw();
	void ObjMDraw();

	void DestoryAfter();

private:
	LightGroup* lightGroup = nullptr;
	DebugText* debugText = nullptr;
	DebugJISText* debugJIS = nullptr;
	DigitalNumberText* digitalN = nullptr;
	XIIlib::Messenger* msger = nullptr;
	DebugCamera* d_camera = nullptr;
	ParticleManager* particles = nullptr;

	float fighterPos[3] = { 1,0.0f,0 };

	bool m_move = 0;

	Line2D* line = nullptr;
	Line2D* lx = nullptr;

	Line* l3x = nullptr;
	Line* l3y = nullptr;
	Line* l3z = nullptr;

	Circle* circle = nullptr;
	
	XIIlib::UnitManager* manager = nullptr;
	XIIlib::HPBar* hpBar = nullptr;
	XIIlib::AttackAreaManager* attackAreaManager = nullptr;
	std::string destoryName;
	XIIlib::GameScene* game_scene = nullptr;

	// LookatRange格納(x,y,z)
	float old_lookatrangeX = 0.0f;
	float old_lookatrangeY = 0.0f;
	float old_lookatrangeZ = 0.0f;
	// LookAt(x,y,z)
	float old_lookatX = 0.0f;
	float old_lookatY = 0.0f;
	float old_lookatZ = 0.0f;
	// Phi(float)
	float old_phi = 0.0f;
	// Theta(float)
	float old_theta = 0.0f;
	// MoveRange(float)
	float old_moverange = 0.0f;
	// Position(x,y,z)
	float old_positionX = 0.0f;
	float old_positionY = 0.0f;
	float old_positionZ = 0.0f;

	unsigned int f_shake = 0;// on : 1,off : 0
	int shakingTime = 0;
	int numberContainer_shake[5] = { 0,2,-2,1,-1};
};