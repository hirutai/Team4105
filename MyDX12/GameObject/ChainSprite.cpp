#include "ChainSprite.h"
#include "TexMoveAction.h"
#include "UnitManager.h"
#include "../Audio/Audio.h"

XIIlib::ChainSprite* XIIlib::ChainSprite::Create()
{
	ChainSprite* pChainSprite = new ChainSprite();

	if (pChainSprite == nullptr) {
		return nullptr;
	}
}

XIIlib::ChainSprite::ChainSprite()
{
	pTexs.reserve(128);
	activeNum = 0;
}

XIIlib::ChainSprite::~ChainSprite()
{
	if (pTexs.size() != 0) {
		for (auto tex : pTexs) {
			delete tex;
		}

		pTexs.clear();
	}
}

void XIIlib::ChainSprite::Update()
{
	if (pTexs.size() == 0 || pTexs.size() == activeNum)return;

	pTexs[activeNum]->Update();
	if (pTexs[activeNum]->GetFinish()) {
		activeNum++;
		UnitManager::GetInstance()->GetAudio()->PlaySE("taiko01.wav", 0.9f);
	}
}

void XIIlib::ChainSprite::Draw()
{
	for (auto tex : pTexs) {
		tex->Draw();
	}
}

void XIIlib::ChainSprite::AddMoji(float posX, float posY, float finishSize, int texNum)
{
	pTexs.push_back(TexMoveAction::Create(posX,posY,finishSize,texNum));
}
