#include "DirectX11.h"
#include <exception>
#include <stdexcept>
#include <fstream>
#include <experimental/filesystem>

	DirectX11::DirectX11(void)
		: m_pDevice(nullptr)
		, m_pDeviceContext(nullptr)
		, m_pSwapChain(nullptr)
	{
	}

	DirectX11::~DirectX11(void)
	{
	}

	// 初期化
	bool DirectX11::Initialize(SimpleWindow& window)
	{
		D3D_FEATURE_LEVEL level;
		UINT flags = 0;
		HRESULT hr;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		UINT numModes,  i, numerator, denominator;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		CreateDXGIFactory(IID_PPV_ARGS(&factory));
		factory->EnumAdapters(0, &adapter);
		adapter->EnumOutputs(0, &adapterOutput);
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		displayModeList = new DXGI_MODE_DESC[numModes];
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (UINT)window.GetWidth())
			{
				if (displayModeList[i].Height == (UINT)window.GetHeight())
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		adapter->GetDesc(&adapterDesc);
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Release the display mode list.
		delete [] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		DXGI_SWAP_CHAIN_DESC desc{};
		// バッファ情報の設定
		desc.BufferCount = 1;
		desc.BufferDesc.Width = window.GetWidth();
		desc.BufferDesc.Height = window.GetHeight();
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = numerator;
		desc.BufferDesc.RefreshRate.Denominator = denominator;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// MSAAの設定
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		// ウィンドウ設定
		desc.Windowed = true;
		desc.OutputWindow = window.GetHandle();

		//	機能レベル
		const D3D_FEATURE_LEVEL FeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,		// DirectX 11.0
			//D3D_FEATURE_LEVEL_10_1,	// DirectX 10.1
			//D3D_FEATURE_LEVEL_10_0,	// DirectX 10.0
		};
		UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);


		//	デバイスを作成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			FeatureLevels,
			NumFeatureLevels,
			D3D11_SDK_VERSION,
			&desc,
			&m_pSwapChain,
			&m_pDevice,
			&level,
			&m_pDeviceContext
		);
		if (FAILED(hr))
		{
			OutputDebugString(L"CreateDeviceで失敗.\n");
			return false;
		}
		//ComPtr<ID3D11Texture2D> pBackBuffer;
		ID3D11Texture2D* pBackBuffer = 0;

		hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (FAILED(hr))
		{
			OutputDebugString(L"GetBufferで失敗.\n");
			return false;
		}

		hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);

		if (FAILED(hr))
		{
			OutputDebugString( L"CreateRenderTargetView [Backbuffer] failed.\n" );
			return false;
		}
		if (pBackBuffer) {

		pBackBuffer->Release();
		pBackBuffer = nullptr;
		}

		// 深度バッファの用意
		D3D11_TEXTURE2D_DESC txDesc;
		ZeroMemory(&txDesc, sizeof(txDesc));

		txDesc.Width = window.GetWidth();
		txDesc.Height = window.GetHeight();
		txDesc.MipLevels = 1;
		txDesc.ArraySize = 1;
		txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		txDesc.SampleDesc.Count = 1;
		txDesc.SampleDesc.Quality = 0;
		txDesc.Usage = D3D11_USAGE_DEFAULT;
		txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		txDesc.CPUAccessFlags = 0;
		txDesc.MiscFlags = 0;

		
		ID3D11Texture2D* pDepthStencil = 0;
		hr = m_pDevice->CreateTexture2D(&txDesc, NULL, &pDepthStencil);


		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;                  
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
		if(FAILED(hr))
		{
			return false;
		}

		// Set the depth stencil state.
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);


		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		dsDesc.Format = txDesc.Format;
		dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsDesc.Texture2D.MipSlice = 0;

		hr = m_pDevice->CreateDepthStencilView(pDepthStencil, &dsDesc, &m_pDepthStencilView);

		if (pDepthStencil) {
			pDepthStencil->Release();
			pDepthStencil = nullptr;
		}

		if (FAILED(hr)) {
			OutputDebugString(L"CreateDepthStencilView failed");
			return false;
		}

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView.Get());


		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = (float)window.GetWidth();
		m_Viewport.Height = (float)window.GetHeight();
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1, &m_Viewport);

		if (!CompileShaders())
			return false;

		if (!CreateVertice())
			return false;

		return true;
	}

	// 終了処理
	void DirectX11::Finalize(void)
	{
		if (m_pDeviceContext) {
			m_pDeviceContext->ClearState();
		}
		if (m_pDepthStencilView) {
		//	m_pDepthStencilView->Release();
		}
	}

	bool DirectX11::CompileShaders() {

		if (!m_vs.Initialize(m_pDevice, L"vs_test.hlsl", "main")) return false;


		if (!m_ps.Initialize(m_pDevice, L"ps_test.hlsl", "main")) return false;

		return true;
	}

	bool DirectX11::CreateVertice() {
		HRESULT hr;
		Vertex vertexList[]{
				{ { -0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
				{ {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_pStride = sizeof(Vertex);
		m_pOffset = 0;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = sizeof(Vertex) * 3;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA srData;
		srData.pSysMem = vertexList;
		srData.SysMemPitch = 0;
		srData.SysMemSlicePitch = 0;

		hr = m_pDevice->CreateBuffer(&bufferDesc, &srData, &m_pVertexBuffer);

		// 入力レイアウト
		if (!m_Inputlayout.Initialize(m_pDevice, m_vs, vertexDesc, ARRAYSIZE(vertexDesc))) return false;


		return true;
	}


	HRESULT DirectX11::CompileShaderFromFile(const std::wstring& fileName, const std::wstring& profile, ComPtr<ID3DBlob>& shaderBlob, ComPtr<ID3DBlob>& errorBlob)
	{
		using namespace std::experimental::filesystem;


		path filePath(fileName);
		std::ifstream infile(filePath);
		std::vector<char> srcData;
		if (!infile)
			throw std::runtime_error("shader not found");
		srcData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
		infile.seekg(0, infile.beg).read(srcData.data(), srcData.size());

		// DXC によるコンパイル処理
		ComPtr<IDxcLibrary> library;
		ComPtr<IDxcCompiler> compiler;
		ComPtr<IDxcBlobEncoding> source;
		ComPtr<IDxcOperationResult> dxcResult;

		DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
		library->CreateBlobWithEncodingFromPinned(srcData.data(), UINT(srcData.size()), CP_ACP, &source);
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

		LPCWSTR compilerFlags[] = {
	  #if _DEBUG
		  L"/Zi", L"/O0",
	  #else
		  L"/O2" // リリースビルドでは最適化
	  #endif
		};
		compiler->Compile(source.Get(), filePath.wstring().c_str(),
			L"main", profile.c_str(),
			compilerFlags, _countof(compilerFlags),
			nullptr, 0, // Defines
			nullptr,
			&dxcResult);

		HRESULT hr;
		dxcResult->GetStatus(&hr);
		if (SUCCEEDED(hr))
		{
			dxcResult->GetResult(
				reinterpret_cast<IDxcBlob**>(shaderBlob.GetAddressOf())
			);
		}
		else
		{
			dxcResult->GetErrorBuffer(
				reinterpret_cast<IDxcBlobEncoding**>(errorBlob.GetAddressOf())
			);
		}
		return hr;
	}

	void DirectX11::Render() {
		if (nullptr == m_pDevice) return;
		if (nullptr == m_pDeviceContext) return;
		if (nullptr == m_pSwapChain) return;
		if (nullptr == m_pRenderTargetView) return;
		if (nullptr == m_pDepthStencilView) return;
		float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha
		
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		 
		m_pDeviceContext->IASetInputLayout(m_Inputlayout.GetInputLayout());
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_pStride, &m_pOffset);
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pDeviceContext->VSSetShader(m_vs.GetShader(), NULL, 0);
		m_pDeviceContext->PSSetShader(m_ps.GetShader(), NULL, 0);

		m_pDeviceContext->Draw(3, 0);
		m_pSwapChain->Present(0, 0);

	}
