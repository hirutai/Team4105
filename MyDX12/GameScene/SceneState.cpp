#include "SceneState.h"
#include "GameScene.h"
using namespace XIIlib;
StageNumber SceneState::stageNum = StageNumber::EASY;
GamePAD_XInput* SceneState::gamePad_ = nullptr;

void XIIlib::SceneState::CommonUpdate(GameScene* p_game_scene)
{
	if (movingScene) {
		Update(p_game_scene);
	}
	else {
		// ƒV[ƒ“‚Ì‘JˆÚ‚ªŠ®—¹‚µ‚Ä‚¢‚é‚©H
		if (p_game_scene->DrawScreen(true)) {
			movingScene = true;
		}
	}
}