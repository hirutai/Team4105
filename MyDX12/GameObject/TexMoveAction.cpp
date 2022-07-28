#include "TexMoveAction.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::TexMoveAction* XIIlib::TexMoveAction::Create(float posX, float posY, float finishSize, int texNum)
{
	TexMoveAction* pTexMoveAction = new TexMoveAction(posX,posY,finishSize,texNum);

	if (pTexMoveAction == nullptr) {
		return nullptr;
	}

	pTexMoveAction->Initialise();

	return pTexMoveAction;
}

XIIlib::TexMoveAction::TexMoveAction(float posX, float posY, float finishSize, int texNum)
{
	tex = Sprite::Create(texNum, { posX,posY }, { 1,1,1,0 }, {0.5f,0.5f});
	this->finishSize = finishSize;
}

XIIlib::TexMoveAction::~TexMoveAction()
{
	delete tex;
}

void XIIlib::TexMoveAction::Initialise()
{
	countFrame = 0.0f;
	
	tex->SetSize({ scale_xy, scale_xy });
}

void XIIlib::TexMoveAction::Update()
{
	// アクション終了してたら即リターン
	if (isFinish)return;

	// 1.5秒で貼れるようにする
	countFrame += (1.0f / 60.0f);
	if (countFrame >= 0.5f) {
		countFrame = 0.5f;
		isFinish = true;
	}
	// スケールの減算
	float sub;
	float subSize = scale_xy - finishSize;
	sub = Easing::InOutCubic(countFrame,0.0f, subSize,0.5f);
	tex->SetSize({ scale_xy - sub,scale_xy - sub });

	float addAlpha = Easing::InOutCubic(countFrame, 0.0f, alpha,0.5f);
	tex->SetAlpha(addAlpha);
}

void XIIlib::TexMoveAction::Draw()
{
	tex->Draw();
}
