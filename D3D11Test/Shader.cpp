#include "Shader.h"
// シェーダコンパイル周りの機能をインクルード
#include <d3dcompiler.h>
// シェーダコンパイル用の静的ライブラリをリンク
#pragma comment(lib, "d3dcompiler.lib")

	// コンパイル
	template<class T>
	bool Shader<T>::Compile(const WCHAR* pFilename, const char* pEntryPoint, const char* pShaderModel, ID3DBlob** ppBlob)
	{
		ID3DBlob* errBlob;
		HRESULT hr = D3DCompileFromFile(
			pFilename,
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			pEntryPoint,
			pShaderModel,
			0,
			0,
			ppBlob,
			&errBlob
		);
		if (FAILED(hr))
		{
			// シェーダのエラー内容を表示
			OutputDebugStringA((const char*)errBlob->GetBufferPointer());

			return false;
		}
		return true;
	}

	// 初期化
	bool VertexShader::Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint)
	{
		ComPtr<ID3DBlob> pBlob;
		if (!Compile(pFilename, pEntryPoint, "vs_5_0", &pBlob))
		{
			return false;
		}
		HRESULT hr;
		// 頂点シェーダの生成
		hr = device->CreateVertexShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&m_pShader
		);
		if (FAILED(hr)) {
			return false;
		}
		m_pShaderCode = pBlob;

		return true;
	}
	// 終了処理
	void VertexShader::Finalize(void)
	{
		Shader::Finalize();
	}

	ID3D11VertexShader* VertexShader::GetShader() {
		return m_pShader.Get();
	}

	// 初期化
	bool PixelShader::Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint)
	{
		ComPtr<ID3DBlob> pBlob;
		if (!Compile(pFilename, pEntryPoint, "ps_5_0", &pBlob))
		{
			return false;
		}
		HRESULT hr;
		// ピクセルシェーダの生成
		hr = device->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&m_pShader
		);

		return SUCCEEDED(hr);
	}

	ID3D11PixelShader* PixelShader::GetShader() {
		return m_pShader.Get();
	}