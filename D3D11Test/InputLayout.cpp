#include "InputLayout.h"

	// ������
	bool InputLayout::Initialize(ComPtr<ID3D11Device> device, VertexShader& shader, D3D11_INPUT_ELEMENT_DESC* pElements, UINT num)
	{
		auto pCode = shader.GetShaderCode();
		auto pointer = pCode->GetBufferPointer();
		auto size = pCode->GetBufferSize();
		// ���̓��C�A�E�g�̐���
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
	// �I������
	void InputLayout::Finalize(void)
	{
	}
