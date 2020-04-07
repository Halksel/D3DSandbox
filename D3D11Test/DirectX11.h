#pragma once

#include "Window.h"
#include "InputLayout.h"
#include "Shader.h"
// DirectX11�̃w�b�_�[���܂߂�
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <string>

#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


#if _MSC_VER > 1922
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class VertexShader;

// Framework�N���X
class DirectX11
{
public:
	DirectX11(void);
	~DirectX11(void);

	// ������
	bool Initialize(SimpleWindow& window);
	// �I������
	void Finalize(void);

	void Render();


public:
	// �f�o�C�X�̎擾
	ComPtr<ID3D11Device> GetDevice(void) { return m_pDevice; }
	// �f�o�C�X�R���e�L�X�g�̎擾
	ComPtr<ID3D11DeviceContext> GetContext(void) { return m_pDeviceContext; }
	// �X���b�v�`�F�C���̎擾
	ComPtr<IDXGISwapChain> GetSwapChain(void) { return m_pSwapChain; }

	HRESULT CompileShaderFromFile(const std::wstring& fileName, const std::wstring& profile, ComPtr<ID3DBlob>& shaderBlob, ComPtr<ID3DBlob>& errorBlob);

	bool CompileShaders();

	struct Vertex {
		float Position[3];
		float Color[4];
	};

	bool CreateVertice();

private:
	ComPtr<ID3D11Device> m_pDevice;				// DirectX11�̃f�o�C�X
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;	// �`��p�̃f�o�C�X�R���e�L�X�g
	ComPtr<IDXGISwapChain> m_pSwapChain;			// ��ʏo�͗p�̃X���b�v�`�F�C��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; //RTV
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView; //DSV

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	D3D11_VIEWPORT m_Viewport;
	InputLayout m_Inputlayout;

	UINT m_pStride, m_pOffset;

	// shaders
	VertexShader m_vs;
	PixelShader m_ps;
};
