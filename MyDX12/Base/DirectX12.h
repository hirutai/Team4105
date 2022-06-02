#pragma once
#include <iostream>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include "../ImGui/Docking/imgui.h"
#include <chrono>
#include <map>
#include <DirectXTex.h>
#include<functional>
#include <d3dcompiler.h>

#include "WindowsApp.h"

class DirectX12
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	// �����o�ϐ�
	// �E�B���h�E�Y�A�v���P�[�V�����Ǘ�
	WindowsApp* winApp;
	// IDXGI _ ID3D12
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//ID3D12Resource* backBuffers;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	static ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	ComPtr<ID3D12DescriptorHeap> imguiHeap;
	float deltaTime = 0.0f;
	static float frameRate;
	static float cpuUsage;
	float commandWaitTime = 0.0f;
	std::chrono::steady_clock::time_point lastUpdate;

	static float clearColor[4];
	using Loadlambda_t = std::function<HRESULT(const std::wstring&, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
	static std::map<std::string, Loadlambda_t> _loadLambdaTable;
	static std::map<std::string, ComPtr<ID3D12Resource>> _textureTable;



private:
	// �����o�֐�
	/// <summary>
	/// DXGI�f�o�C�X������
	/// </summary>
	bool InitDXGIDevice();

	/// <summary>
	/// �X���b�v�`�F�[���̐���
	/// </summary>
	bool GenerateSwapChain();

	/// <summary>
	/// �R�}���h�֘A������
	/// </summary>
	bool InitCommand();

	/// <summary>
	/// �����_�[�^�[�Q�b�g����
	/// </summary>
	bool GenerateRenderTargets();

	bool CreateDepthBuffer();

	/// <summary>
	/// �t�F���X����
	/// </summary>
	bool GenerateFence();

	/// <summary>
	/// imgui������
	/// </summary>
	/// <returns>����</returns>
	bool InitImgui();
public:
	enum { FRAME_BUFFER_COUNT = 2 };
public:
	DirectX12(){}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DirectX12();

	// �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WindowsApp* win);

	/// <summary>
	/// ��n��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �`��O����
	/// </summary>
	void Previous();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostProcessing();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃N���A
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
	static void ClearDepthBuffer();

	static void SetClearColor(const float& r = 1.0f, const float& g = 1.0f, const float& b = 1.0f);

	static void SetClearColorR(const float& r = 1.0f);
	static void SetClearColorG(const float& g = 1.0f);
	static void SetClearColorB(const float& b = 1.0f);

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
	UINT BackBufferSize() {return swapchain->GetCurrentBackBufferIndex();}

	static float GetFPS_Second() { return frameRate; }
	static float GetCPU_Usage() { return cpuUsage; }

	static ID3D12Device* GetDevice(){return device.Get();}
	static ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
	static void CreateTextureLoaderTable();
	static ID3D12Resource* CreateTextureFromFile(const char* texpath);
	static ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);
};