#pragma once
#include "Shader.h"


	// InputLayout�N���X
	class InputLayout
	{
	public:
		InputLayout(void) : m_pInputLayout(NULL) {}
		~InputLayout(void) {}
		// ������
		bool Initialize(ComPtr<ID3D11Device> device, VertexShader& shader, D3D11_INPUT_ELEMENT_DESC* pDesc, UINT size);
		// �I������
		void Finalize(void);

	public:
		// ���_���̓��C�A�E�g�̎擾
		ID3D11InputLayout* GetInputLayout(void) { return m_pInputLayout.Get(); }

	private:
		ComPtr<ID3D11InputLayout> m_pInputLayout; // ���_���̓��C�A�E�g
	};
