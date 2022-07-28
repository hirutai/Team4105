#include "TexMoveAction.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::TexMoveAction* XIIlib::TexMoveAction::Create(float posX, float posY, int texNum)
{
	TexMoveAction* pTexMoveAction = new TexMoveAction(posX,posY,texNum);

	if (pTexMoveAction == nullptr) {
		return nullptr;
	}

	pTexMoveAction->Initialise();

	return pTexMoveAction;
}

XIIlib::TexMoveAction::TexMoveAction(float posX, float posY, int texNum)
{
	tex = Sprite::Create(texNum, { posX,posY }, { 1,1,1,0 }, {0.5f,0.5f});
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
	// �A�N�V�����I�����Ă��瑦���^�[��
	if (isFinish)return;

	// 1.5�b�œ\���悤�ɂ���
	countFrame += (1.0f / 60.0f);
	if (countFrame >= 1.5f) {
		countFrame = 1.5f;
		isFinish = true;
	}
	// �X�P�[���̌��Z
	float sub;
	const float subSize = 600.0f;
	sub = Easing::InOutCubic(countFrame,0.0f, subSize,1.5f);
	tex->SetSize({ scale_xy - sub,scale_xy - sub });

	float addAlpha = Easing::InOutCubic(countFrame, 0.0f, alpha,1.5f);
	tex->SetAlpha(addAlpha);
}

void XIIlib::TexMoveAction::Draw()
{
	tex->Draw();
}