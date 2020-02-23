#include "MySandbox.h"
#include <stdexcept>

void MySandbox::Prepare() {
	const DirectX::XMFLOAT4 red     (1.0f, 0.0f, 0.0f, 1.0f);
	const DirectX::XMFLOAT4 green   (0.0f, 1.0f, 0.0f, 1.0f);
	const DirectX::XMFLOAT4 blue    (0.0f, 0.0f, 1.0f, 1.0);
	const DirectX::XMFLOAT4 white   (1.0f, 1.0f, 1.0f, 1.0f);
	const DirectX::XMFLOAT4 black   (0.0f, 0.0f, 0.0f, 1.0f);
	const DirectX::XMFLOAT4 yellow  (1.0f, 1.0f, 0.0f, 1.0f);
	const DirectX::XMFLOAT4 magenta (1.0f, 0.0f, 1.0f, 1.0f);
	const DirectX::XMFLOAT4 cyan    (0.0f, 1.0f, 1.0f, 1.0f);


	Vertex pointVertices[] = {
{ {  0.0f, 0.0f, 0.0f }, magenta},
{ { 0.5f,0.0f, 0.0f }, white },
{ { 0.5f,0.5f, 0.0f }, cyan},
	};
	uint32_t indices[] = { 0,1,2 ,1, 3 ,2  };


	m_vertexBuffer = CreateBuffer(sizeof(pointVertices), pointVertices);
	m_indexBuffer = CreateBuffer(sizeof(indices), indices);
	m_indexCount = _countof(indices);

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = sizeof(pointVertices);
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = sizeof(indices);
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	HRESULT hr;
	ComPtr<ID3D10Blob> errBlob;
	hr = CompileShaderFromFile(L"sandboxVS.hlsl", L"vs_6_0", m_vs, errBlob);
	if (FAILED(hr)) {
		OutputDebugStringA((const char*)errBlob->GetBufferPointer());
	}

	hr = CompileShaderFromFile(L"sandboxPS.hlsl", L"ps_6_0", m_ps, errBlob);
	if (FAILED(hr)) {
		OutputDebugStringA((const char*)errBlob->GetBufferPointer());
	}

	hr = CompileShaderFromFile(L"sandboxGS.hlsl", L"gs_6_0", m_gs, errBlob);
	if (FAILED(hr)) {
		OutputDebugStringA((const char*)errBlob->GetBufferPointer());
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> signature;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
	if (FAILED(hr)) {
		throw std::runtime_error("D3D12SerializeRootSignature faild");
	}

	hr = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	if (FAILED(hr)) {
		throw std::runtime_error("CreateRootSignature faild");
	}

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
  { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, offsetof(Vertex,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.Get());
	psoDesc.GS = CD3DX12_SHADER_BYTECODE(m_gs.Get());

	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // これを忘れると絵が出ない＆警告も出ないので注意.

	hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateGraphicsPipelineState failed");
	}
}

void MySandbox::Cleanup()
{
	auto index = m_swapchain->GetCurrentBackBufferIndex();
	auto fence = m_frameFences[index];
	auto value = ++m_frameFenceValues[index];
	m_commandQueue->Signal(fence.Get(), value);
	fence->SetEventOnCompletion(value, m_fenceWaitEvent);
	WaitForSingleObject(m_fenceWaitEvent, GpuWaitTimeout);
}

void MySandbox::MakeCommand(ComPtr<ID3D12GraphicsCommandList>& command)
{
	// パイプラインステートのセット
	command->SetPipelineState(m_pipeline.Get());
	// ルートシグネチャのセット
	command->SetGraphicsRootSignature(m_rootSignature.Get());
	// ビューポートとシザーのセット
	command->RSSetViewports(1, &m_viewport);
	command->RSSetScissorRects(1, &m_scissorRect);

	// プリミティブタイプ、頂点・インデックスバッファのセット
	command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	command->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	command->IASetIndexBuffer(&m_indexBufferView);

	// 描画命令の発行
	command->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}


MySandbox::ComPtr<ID3D12Resource1> MySandbox::CreateBuffer(UINT bufferSize, const void* initialData)
{
	HRESULT hr;
	ComPtr<ID3D12Resource1> buffer;
	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);

	// 初期データの指定があるときにはコピーする
	if (SUCCEEDED(hr) && initialData != nullptr)
	{
		void* mapped;
		CD3DX12_RANGE range(0, 0);
		hr = buffer->Map(0, &range, &mapped);
		if (SUCCEEDED(hr))
		{
			memcpy(mapped, initialData, bufferSize);
			buffer->Unmap(0, nullptr);
		}
	}

	return buffer;
}
