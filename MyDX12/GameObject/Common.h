#pragma once
#include <string>
#include "../Struct/Point2.h"

namespace Common {
	// 個々にコメントを追加

	// クラスネームの取り出し関数
	std::pair<std::string, std::string> SeparateFilePath(const std::string& path);

	// マスの座標から位置座標を取得
	float ConvertTilePosition(int point);

	// 例外の検知
	bool GetExceptionPoint(int point);

	int OffsetTilePosition(int point);
	Math::Point2 OffsetTilePosition2(const Math::Point2& point);



}