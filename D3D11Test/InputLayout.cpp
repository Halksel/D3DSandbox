#include "InputLayout.h"

	// 初期化
	bool InputLayout::Initialize(ComPtr<ID3D11Device> device, VertexShader& shader, D3D11_INPUT_ELEMENT_DESC* pElements, UINT num)
	{
		auto pCode = shader.GetShaderCode();
		auto pointer = pCode->GetBufferPointer();
		auto size = pCode->GetBufferSize();
		// 入力レイアウトの生成
		if (FAILED(device->CreateInputLayout(
			pElements,
			num,
			pointer,
			size,
			&m_pInputLayout)))
		{
			return false;
		}
		return true;
	}
	// 終了処理
	void InputLayout::Finalize(void)
	{
	}
