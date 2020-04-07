#include "Shader.h"
// �V�F�[�_�R���p�C������̋@�\���C���N���[�h
#include <d3dcompiler.h>
// �V�F�[�_�R���p�C���p�̐ÓI���C�u�����������N
#pragma comment(lib, "d3dcompiler.lib")

	// �R���p�C��
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
			// �V�F�[�_�̃G���[���e��\��
			OutputDebugStringA((const char*)errBlob->GetBufferPointer());

			return false;
		}
		return true;
	}

	// ������
	bool VertexShader::Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint)
	{
		ComPtr<ID3DBlob> pBlob;
		if (!Compile(pFilename, pEntryPoint, "vs_5_0", &pBlob))
		{
			return false;
		}
		HRESULT hr;
		// ���_�V�F�[�_�̐���
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
	// �I������
	void VertexShader::Finalize(void)
	{
		Shader::Finalize();
	}

	ID3D11VertexShader* VertexShader::GetShader() {
		return m_pShader.Get();
	}

	// ������
	bool PixelShader::Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint)
	{
		ComPtr<ID3DBlob> pBlob;
		if (!Compile(pFilename, pEntryPoint, "ps_5_0", &pBlob))
		{
			return false;
		}
		HRESULT hr;
		// �s�N�Z���V�F�[�_�̐���
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