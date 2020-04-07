#pragma once

#include "Window.h"
#include "DirectX11.h"
#include "Shader.h"

struct FrameworkDesc
{
	LPCTSTR Name;
	int X, Y;
	int Width, Height;
};

// Framework�N���X
class Framework
{
public:
	Framework(void);
	~Framework(void);

	// ������
	bool Initialize(const FrameworkDesc& desc);
	// �I������
	void Finalize(void);

	// �E�B���h�E�̎擾
	SimpleWindow& GetWindow(void) { return m_Window; }
	// DirectX11�̎擾
	DirectX11& GetDirectX11(void) { return m_DirectX11; }

	void MainLoop();

	static Framework& GetInstance(void) { return Instance; }
private:
	SimpleWindow m_Window;	// �E�B���h�E
	DirectX11 m_DirectX11;	// DirectX11

	// �V�F�[�_�[
	VertexShader m_vs;
	PixelShader m_ps;

	static Framework Instance;	// �C���X�^���X
};
