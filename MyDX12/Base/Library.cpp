#include <time.h>
#include "Library.h"
#include "WindowsApp.h"
#include "DirectX12.h"
#include "../Input/KeyInput.h"
#include "../Input/ControllerInput.h"
#include "../Audio/Sound.h"
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include "../2D/Sprite.h"
#include "../2D/Point2D.h"
#include "../2D/Line2D.h"
#include "../2D/Circle.h"
#include "../Light/LightGroup.h"
#include "../3D/Line.h"
#include "../2D/TextureInstance.h"
#include "../Particle/ParticleManager.h"
#include "../3D/CollCapsule.h"
#include "../3D/CollBox.h"
#include "../3D/CollCircle.h"
#include <sstream>

#pragma comment(lib, "winmm.lib")

using namespace XIIlib;

XIIlib::Library::Library()
{
}

XIIlib::Library::~Library()
{
	//pad_input->Cleanup();

	delete dx12;
	w_app->TerminateWindow();
	delete w_app;
}

int XIIlib::Library::Initialize()
{
	srand(time(NULL));
	// �E�B���h�E�̐���
	w_app = new WindowsApp();
	w_app->GenerateWindow();
	//DirectX����������
	dx12 = new DirectX12();
	dx12->Initialize(w_app);
	// Input������
	keyInput = KeyInput::GetInstance();
	keyInput->Initialize(w_app->GetInstance(), w_app->GetWinHandle());

	// ���̑��ėp�N���X�̏�����
	Sprite::StaticInitialize();
	Point2D::StaticInitialize();
	Line2D::StaticInitialize();
	Circle::StaticInitialize();
	Line::StaticInitialize();
	TextureInstance::StaticInitialize();
	Object3D::StaticInitialize();
	BillObj::StaticInitialize();
	LightGroup::StaticInitialize(DirectX12::GetDevice());
	ParticleManager::GetInstance()->Initialize("effect1.png");
	CollisionCapsule::StaticInitialize();
	CollisionBox::StaticInitialize();
	CollisionCircle::StaticInitialize();

	return 1;
}

int XIIlib::Library::GetQuery()
{
	// ���C�����[�v�ɓ���O�ɐ��x���擾���Ă���
	if (QueryPerformanceFrequency(&timeFreq) == FALSE) { // ���̊֐���0(FALSE)���A�鎞�͖��Ή�
		// ��������QueryPerformanceFrequency���g���Ȃ��l��(�Â�)PC�ł͂ǂ����F�X�L�c�C���낤��
		return(E_FAIL); // �{���͂���ȋA������Ă͍s���Ȃ�(�㑱�̉���������Ă΂�Ȃ�)
	}
	// 1�x�擾���Ă���(����v�Z�p)
	QueryPerformanceCounter(&timeStart);
}

void XIIlib::Library::RunTimer()
{
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // ���Ԃɗ]�T������
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // ����\���グ��(�������Ȃ���Sleep�̐��x�̓K�^�K�^)
		Sleep(sleepTime);   // �Q��
		timeEndPeriod(1);   // �߂�

		// ���T�Ɏ����z��(�������Ȃ���fps���ςɂȂ�?)
		return;
	}

	if (frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		fps = (fps * 0.99f) + (0.01f / frameTime); // ����fps���v�Z
#ifdef _DEBUG
// �f�o�b�O�p(�f�o�b�K��FSP�o��)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << fps << " FPS" << std::endl;
		// �J�E���^�t����10���1��o�́A�Ƃ��ɂ��Ȃ��ƌ��Â炢������
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}

	timeStart = timeEnd; // ����ւ�
}

bool XIIlib::Library::Message()
{
	return w_app->Message();
}

void XIIlib::Library::InputUpdate()
{
	keyInput->Update();
	//pad_input->Update();
}

void XIIlib::Library::D_Start()
{
	dx12->Previous();
}

void XIIlib::Library::D_End()
{
	dx12->PostProcessing();
}
