#pragma once
#include <string>
#include "../Struct/Point2.h"

namespace Common {
	// �X�ɃR�����g��ǉ�

	// �N���X�l�[���̎��o���֐�
	std::pair<std::string, std::string> SeparateFilePath(const std::string& path);

	// �}�X�̍��W����ʒu���W���擾
	float ConvertTilePosition(int point);

	// ��O�̌��m
	bool GetExceptionPoint(int point);

	int OffsetTilePosition(int point);
	Math::Point2 OffsetTilePosition2(const Math::Point2& point);



}