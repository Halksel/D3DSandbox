#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <string>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// �V�F�[�_�Ǘ��p�N���X�e���v���[�g
template<class T>
class Shader
{
public:
	Shader(void) : m_pShader(NULL) {}
	virtual ~Shader(void) { Finalize(); }

	// ������
	virtual bool Initialize(ComPtr<ID3D11Device> device, const char* pCode, const char* pEntryPoint) { return false; }
	// �I������
	virtual void Finalize(void) {}

	// �V�F�[�_�̎擾
	virtual T* GetShader(void) { return nullptr; }

protected:
	// �R���p�C��
	bool Compile(const WCHAR* pFilename, const char* pEntryPoint, const char* pShaderModel, ID3DBlob** ppBlob);

protected:
	ComPtr<T> m_pShader;	// �V�F�[�_
};

// VertexShader�N���X
class VertexShader : public Shader<ID3D11VertexShader>
{
private:
	ComPtr<ID3DBlob> m_pShaderCode;
public:
	VertexShader(void) {}
	// ������
	virtual bool Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint);
	// �I������
	virtual void Finalize(void);
	virtual ID3D11VertexShader* GetShader();
	// �V�F�[�_�R�[�h�̎擾
	ID3DBlob* GetShaderCode(void) {
		return m_pShaderCode.Get(); 
	}
};

// PixelShader�N���X
class PixelShader : public Shader<ID3D11PixelShader>
{
public:
	// ������
	virtual bool Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint);
	virtual ID3D11PixelShader* GetShader();
};
