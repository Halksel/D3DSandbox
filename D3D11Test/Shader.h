#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <string>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// シェーダ管理用クラステンプレート
template<class T>
class Shader
{
public:
	Shader(void) : m_pShader(NULL) {}
	virtual ~Shader(void) { Finalize(); }

	// 初期化
	virtual bool Initialize(ComPtr<ID3D11Device> device, const char* pCode, const char* pEntryPoint) { return false; }
	// 終了処理
	virtual void Finalize(void) {}

	// シェーダの取得
	virtual T* GetShader(void) { return nullptr; }

protected:
	// コンパイル
	bool Compile(const WCHAR* pFilename, const char* pEntryPoint, const char* pShaderModel, ID3DBlob** ppBlob);

protected:
	ComPtr<T> m_pShader;	// シェーダ
};

// VertexShaderクラス
class VertexShader : public Shader<ID3D11VertexShader>
{
private:
	ComPtr<ID3DBlob> m_pShaderCode;
public:
	VertexShader(void) {}
	// 初期化
	virtual bool Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint);
	// 終了処理
	virtual void Finalize(void);
	virtual ID3D11VertexShader* GetShader();
	// シェーダコードの取得
	ID3DBlob* GetShaderCode(void) {
		return m_pShaderCode.Get(); 
	}
};

// PixelShaderクラス
class PixelShader : public Shader<ID3D11PixelShader>
{
public:
	// 初期化
	virtual bool Initialize(ComPtr<ID3D11Device> device, const WCHAR* pFilename, const char* pEntryPoint);
	virtual ID3D11PixelShader* GetShader();
};
