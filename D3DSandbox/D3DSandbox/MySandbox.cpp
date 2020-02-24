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
{ {  -1.0f, -1.0f, 0.0f }, blue},
{ { 0.0f,1.0f, 0.0f }, white },
{ {  1.0f, -1.0f, 0.0f }, cyan},
{ { 0.0f,1.0f, 0.0f }, cyan},
{ {  0.0f, 0.0f, 0.0f }, blue},
{ {  0.0f, 0.1f, 0.0f }, white},
{ { 0.5f,0.0f, 0.0f }, white },
{ { 0.5f,0.5f, 0.0f }, cyan},
	};
	uint32_t indices[] = { 0,1,2};


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

	CD3DX12_DESCRIPTOR_RANGE cbv;
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 ���W�X�^

	CD3DX12_ROOT_PARAMETER rootParams[1];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(_countof(rootParams), rootParams, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
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
	psoDesc.SampleMask = UINT_MAX; // �����Y���ƊG���o�Ȃ����x�����o�Ȃ��̂Œ���.

	hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateGraphicsPipelineState failed");
	}

	PrepareDescriptorHeapForSandboxApp();

	// �萔�o�b�t�@/�萔�o�b�t�@�r���[�̐���
	m_constantBuffers.resize(FrameBufferCount);
	m_cbViews.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
		m_constantBuffers[i] = CreateBuffer(bufferSize, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
		cbDesc.SizeInBytes = bufferSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
		m_device->CreateConstantBufferView(&cbDesc, handleCBV);

		m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
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
	using namespace DirectX;

	// �e�s��̃Z�b�g.
	ShaderParameters shaderParams;
	XMStoreFloat4x4(&shaderParams.mtxWorld, XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(45.0f)));
	auto mtxView = XMMatrixLookAtLH(
		XMVectorSet(4.0f, 3.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	auto mtxProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_viewport.Width / m_viewport.Height, 0.1f, 100.0f);
	XMStoreFloat4x4(&shaderParams.mtxView, XMMatrixTranspose(mtxView));
	XMStoreFloat4x4(&shaderParams.mtxProj, XMMatrixTranspose(mtxProj));

	// �萔�o�b�t�@�̍X�V.
	auto& constantBuffer = m_constantBuffers[m_frameIndex];
	{
		void* p;
		CD3DX12_RANGE range(0, 0);
		constantBuffer->Map(0, &range, &p);
		memcpy(p, &shaderParams, sizeof(shaderParams));
		constantBuffer->Unmap(0, nullptr);
	}
	
	// �p�C�v���C���X�e�[�g�̃Z�b�g
	command->SetPipelineState(m_pipeline.Get());
	// ���[�g�V�O�l�`���̃Z�b�g
	command->SetGraphicsRootSignature(m_rootSignature.Get());
	// �r���[�|�[�g�ƃV�U�[�̃Z�b�g
	command->RSSetViewports(1, &m_viewport);
	command->RSSetScissorRects(1, &m_scissorRect);
	// �f�B�X�N���v�^�q�[�v���Z�b�g.
	ID3D12DescriptorHeap* heaps[] = {
	  m_heapSrvCbv.Get()
	};
	command->SetDescriptorHeaps(_countof(heaps), heaps);

	// �v���~�e�B�u�^�C�v�A���_�E�C���f�b�N�X�o�b�t�@�̃Z�b�g
	command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	command->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	command->IASetIndexBuffer(&m_indexBufferView);

	command->SetGraphicsRootDescriptorTable(0, m_cbViews[m_frameIndex]);

	// �`�施�߂̔��s
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

	// �����f�[�^�̎w�肪����Ƃ��ɂ̓R�s�[����
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

void MySandbox::PrepareDescriptorHeapForSandboxApp()
{
	// CBV/SRV �̃f�B�X�N���v�^�q�[�v
	//  0:�V�F�[�_�[���\�[�X�r���[
	//  1,2 : �萔�o�b�t�@�r���[ (FrameBufferCount�����g�p)
	UINT count = FrameBufferCount + 1;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	  count,
	  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
	  0
	};
	m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heapSrvCbv));
	m_srvcbvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
