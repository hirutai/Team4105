#include "ModelLoader.h"
#include "../3D/Model.h"
using namespace XIIlib;

ModelLoader* XIIlib::ModelLoader::GetInstance()
{
	static ModelLoader instance;
	return &instance;
}

void XIIlib::ModelLoader::Initialize()
{
	models.resize(10);
}

void XIIlib::ModelLoader::Load(){
	SetModel(MODEL_TILE,    "bord");
	SetModel(MODEL_KING,    "Badboy_Bat_1");
	SetModel(MODEL_YANKEE,  "Badboy_Enemy");
	SetModel(MODEL_ROOK,    "bike2");
	SetModel(MODEL_BISHOP,  "bike");
	SetModel(MODEL_BOSS,    "boss");
}

Model* XIIlib::ModelLoader::GetModel(const ModelNum& modelNum) const
{
	return models[static_cast<int>(modelNum)].get();
}

void XIIlib::ModelLoader::SetModel(const ModelNum& modelNum,const std::string& modelName)
{
	std::unique_ptr<Model> uModel(Model::CreateFromOBJ(modelName));
	models[static_cast<int>(modelNum)] = std::move(uModel);
}
