#include "Base/Library.h"
#include "Scene/GameApp.h"
#include "2D/PostEffect.h"

#include "Input/KeyInput.h"
#include <sstream>

// �^�[�Q�b�g��Debug�̎�
#ifdef _DEBUG
int main()
// �^�[�Q�b�g��Debug�łȂ���
#else

// Windows �A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	XIIlib::Library* dxXii = nullptr;
	dxXii = new XIIlib::Library();
	dxXii->Initialize();

	dxXii->SetFPS();

	GameApp* app = nullptr;
	PostEffect* postEffect = nullptr;
	//// win_app�̃A�h���X
	////printf("win:%x\n", (unsigned long int)win_app);

	// �|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();

	// �Q�[���̏�����
	app = new GameApp();
	app->Initialize();
	Math::Vector2 windowSize = { WindowsApp::window_width, WindowsApp::window_height };

	postEffect->SetPosition(windowSize / 2);

	DWORD timeBefore = GetTickCount();
	DWORD fps = 0;

	while (true)
	{
		// ���b�Z�[�W����
		if (dxXii->Message()) { break; }

		++fps;

		if ((GetTickCount() - timeBefore) >= 1000) { // 1�b�ȏ�o�߂��Ă���
#ifdef _DEBUG // �f�o�b�O�p(�f�o�b�K��FSP�o��)
#ifdef UNICODE
			std::wstringstream stream;
#else
			std::stringstream stream;
#endif
			stream << fps << " FPS" << std::endl;
			OutputDebugString(stream.str().c_str());
			std::wcout << stream.str()<< std::endl;
#endif

			// 0�Ƀ��Z�b�g
			fps = 0;
			// ���Z�b�g
			timeBefore = GetTickCount();
		}

		// ���͏��� : �X�V
		dxXii->InputUpdate();

		// �Q�[���̖��t���[������(Update)
		app->Update();

		// �`��
		// �ʏ�`��V�[��
		postEffect->PreDrawScene(DirectX12::GetCmdList());
		app->LineDraw3D();

		//app->Draw();
		app->ObjMDraw();
		postEffect->PostLDrawScene(DirectX12::GetCmdList());
		postEffect->PostDrawScene(DirectX12::GetCmdList());

		// �J�n
		dxXii->D_Start();
		app->BackSc();
		 //�|�X�g�G�t�F�N�g�̕`��
		postEffect->Draw(DirectX12::GetCmdList());
		app->DrawImGui();
		app->FrontSc();
		// �I��
		dxXii->D_End();
		app->DestoryAfter();

		// ESC�ŕ���
		if (XIIlib::KeyInput::GetInstance()->Trigger(DIK_ESCAPE))break;
	}

	dxXii->ReleaseTimer();

	// �e����
	delete postEffect;
	delete app;

	delete dxXii;

	return 0;
}