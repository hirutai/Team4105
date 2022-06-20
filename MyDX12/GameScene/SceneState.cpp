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
		// シーンの遷移が完了しているか？
		if (p_game_scene->DrawScreen(true)) {
			movingScene = true;
		}
	}
}
