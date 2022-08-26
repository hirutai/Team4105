#pragma once
#include <string>
#include <vector>
#include <memory>

class Model;

namespace XIIlib {

	enum ModelNum {
		MODEL_TILE = 0,
		MODEL_KING,
		MODEL_YANKEE,
		MODEL_ROOK,
		MODEL_BISHOP,
		MODEL_BOSS,
		MODEL_BOSS2,
		MODEL_STAGE_A,
		MODEL_STAGE_B,
		MODEL_STAGE_C,
		MODEL_STAGE_D,
		MODEL_STAGE_E,
		MODEL_GUARDMAN,
		MODEL_BOSSCAR
	};

	class ModelLoader
	{
	public:
		static ModelLoader* GetInstance();

		void Initialize();
		void Load();

		Model* GetModel(const ModelNum& modelNum)const;
	private:
		ModelLoader() = default;
		ModelLoader(const ModelLoader&) = delete;
		~ModelLoader() = default;
		ModelLoader& operator=(const ModelLoader&) = delete;

		void SetModel(const ModelNum& modelNum,const std::string& modelName);
	private:
		std::vector<std::unique_ptr<Model>> models;
	};

}