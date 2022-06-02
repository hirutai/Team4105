#pragma once
#include <vector>
#include "../Struct/Point2.h"

namespace XIIlib {

	class Tile;

	class BoardMap {
	private:
		std::vector<std::vector<Tile*>> map_board;

	public:
		static BoardMap* Create();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		BoardMap();
		~BoardMap();

		void Initialize();

		void Update();

		void Draw();

		void SetBackVector(const Math::Point2& element,const Math::Point2& point);

		void ChangeAttackValidTile(const Math::Point2& element,int type_positioning);

		void SetOnTile(const Math::Point2& element);

		bool IsAttackValid(const Math::Point2& element,int type_positioning)const;

		Math::Point2 GetBackVector(const Math::Point2& element)const;

		bool GetOnTile(const Math::Point2& element)const;

	};
}