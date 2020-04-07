#pragma once
#include "Shader.h"


	// InputLayoutクラス
	class InputLayout
	{
	public:
		InputLayout(void) : m_pInputLayout(NULL) {}
		~InputLayout(void) {}
		// 初期化
		bool Initialize(ComPtr<ID3D11Device> device, VertexShader& shader, D3D11_INPUT_ELEMENT_DESC* pDesc, UINT size);
		// 終了処理
		void Finalize(void);

	public:
		// 頂点入力レイアウトの取得
		ID3D11InputLayout* GetInputLayout(void) { return m_pInputLayout.Get(); }

	private:
		ComPtr<ID3D11InputLayout> m_pInputLayout; // 頂点入力レイアウト
	};
