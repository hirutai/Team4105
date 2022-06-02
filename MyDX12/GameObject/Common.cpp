#include "Common.h"

std::pair<std::string, std::string> Common::SeparateFilePath(const std::string& path)
{
	// �󂯎M��p��
	std::pair<std::string, std::string> result;

	size_t pos1;
	// ��؂蕶��'\\'���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('::');
	if (pos1 != std::string::npos) {
		result.first = path.substr(0, pos1 + 1);
		result.second = path.substr(pos1 + 1, path.size() - pos1 - 1);
		return result;
	}

	// ��؂蕶��'\'���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind(': ');
	if (pos1 != std::string::npos) {
		result.first = path.substr(0, pos1 + 1);
		result.second = path.substr(pos1 + 1, path.size() - pos1 - 1);
		return result;
	}

	result.first = "";
	result.second = path;

	return result;
}

// �}�X���W����ʒu���W�ւ̕ϊ�
float Common::ConvertTilePosition(int point)
{
	const float pos_start = -17.5f;
	const float pos_distance = 5.0f;

	return pos_start + (point * pos_distance);
}

bool Common::GetExceptionPoint(int point)
{
	const int start = 0, end = 7;
	int result = point;

	if (point < start)return true;
	else if (point > end)return true;
	return false;
}

int Common::OffsetTilePosition(int point)
{
	const int start = 0, end = 7;
	int result = point;

	if (point < start)result = start;
	else if (point > end)result = end;

	return result;
}

Math::Point2 Common::OffsetTilePosition2(const Math::Point2& point)
{
	Math::Point2 result = {
		OffsetTilePosition(point.a),
		OffsetTilePosition(point.b)
	};
	return result;
}
