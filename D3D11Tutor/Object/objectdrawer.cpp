#include "objectdrawer.h"

ObjectDrawer::ObjectDrawer()
{
}

RectDrawer& RectDrawer::operator=(const RectDrawer& r)
{
	// TODO: return ステートメントをここに挿入します
	*this = RectDrawer(r);
	return *this;
}

RectDrawer::~RectDrawer()
{
}


bool RectDrawer::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR textureFilename)
{
	if (m_initialized) return true;
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		OutputDebugString(L"model initialize buffer failed");
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(hwnd, device, deviceContext, textureFilename);
	if (!result)
	{
		MyOutputDebugString(L"Failed TextureLoad %s", textureFilename);
		return false;
	}
	m_initialized = true;

	return true;
}

void RectDrawer::Update(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE Mappedresource;
	deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mappedresource);

	VertexType* verticesptr;
	verticesptr = (VertexType*)Mappedresource.pData;

	auto vertices = new VertexType[m_vertexCount];
	vertices[0].position = m_obj->GetPoint(0);  // 左下
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[1].position = m_obj->GetPoint(1);  // 左上
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[2].position = m_obj->GetPoint(2);  // 右上
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].position = m_obj->GetPoint(3);  // 右下
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

	memcpy (verticesptr, (void*) vertices, (sizeof (VertexType) * m_vertexCount));

	deviceContext->Unmap (m_vertexBuffer, 0);
	delete [] vertices;
	vertices = 0;
}


void RectDrawer::Shutdown()
{
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	

	return;
}

void RectDrawer::SetSquare(Rect* obj)
{
	m_obj = obj;
}


void RectDrawer::Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	m_TextureShader->Render(deviceContext, m_indexCount, world, view, proj, m_Texture->GetTexture());

	return;
}


int RectDrawer::GetIndexCount()
{
	return m_indexCount;
}

bool RectDrawer::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = m_obj->GetPoint(0);  // 左下
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[1].position = m_obj->GetPoint(1);  // 左上
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[2].position = m_obj->GetPoint(2);  // 右上
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].position = m_obj->GetPoint(3);  // 右下
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 3;  
	indices[3] = 1;  
	indices[4] = 2;  
	indices[5] = 3;  

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void RectDrawer::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void RectDrawer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool RectDrawer::LoadTexture(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		MyOutputDebugString(L"failed loadTexture initialize %s", filename);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MyOutputDebugString(L"failed loadTexture shader initialize %s", filename);
		return false;
	}

	return true;
}

void RectDrawer::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}

CircleDrawer& CircleDrawer::operator=(const CircleDrawer& r)
{
	return *this = CircleDrawer(r);
}

bool CircleDrawer::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR textureFilename)
{
	if (m_initialized) return true;
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		OutputDebugString(L"model initialize buffer failed");
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(hwnd, device, deviceContext, textureFilename);
	if (!result)
	{
		MyOutputDebugString(L"Failed TextureLoad %s", textureFilename);
		return false;
	}
	m_initialized = true;

	return true;
}

void CircleDrawer::Update(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE Mappedresource;
	deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mappedresource);

	VertexType* verticesptr;
	verticesptr = (VertexType*)Mappedresource.pData;

	int count = 0;
	XMFLOAT3 c = m_obj->GetCenter();
	auto r = m_obj->GetRadius();
	auto num = m_obj->GetStrides();
	float Step = PI * 2.0 / num;

	auto vertices = new VertexType[m_vertexCount];

	count = 0;
	vertices[count].position = XMFLOAT3(c.x, c.y, 0);
	vertices[count].texture = XMFLOAT2(0.5f, 0.5f);
	count = 1;
	for (float a=0; a < PI*2.0; a += Step)
	{
		float cosa = cos(a);
		float sina = sin(a);
		float X1 = r * cosa + c.x;
		float Y1 = r * sina + c.y;
		vertices[count].position = XMFLOAT3(X1, Y1, 0);
		//vertices[count].texture = XMFLOAT2(cosa * 0.5f + 0.5f, sina* 0.5f + 0.5f);
		vertices[count].texture = XMFLOAT2(0.3f, 0.5f);

		count += 1;
	}

	memcpy (verticesptr, (void*) vertices, (sizeof (VertexType) * m_vertexCount));

	deviceContext->Unmap (m_vertexBuffer, 0);
	delete [] vertices;
	vertices = 0;
}

void CircleDrawer::Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	m_TextureShader->Render(deviceContext, m_indexCount, world, view, proj, m_Texture->GetTexture());
}

void CircleDrawer::Shutdown()
{
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

void CircleDrawer::SetCircle(Circle* obj)
{
	m_obj = obj;
}

bool CircleDrawer::InitializeBuffers(ID3D11Device* device)
{
	VertexType *vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;



	int count = 0;
	XMFLOAT3 c = m_obj->GetCenter();
	auto r = m_obj->GetRadius();
	auto num = m_obj->GetStrides();
	float Step = PI * 2.0 / num;
	// Set the number of vertices in the vertex array.
	m_vertexCount = num + 1;

	// Set the number of indices in the index array.
	m_indexCount = num * 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	count = 0;
	vertices[count].position = XMFLOAT3(c.x, c.y, 0);
	vertices[count].texture = XMFLOAT2(0.5f, 0.5f);
	count = 1;
	for (float a=0; a < PI*2.0; a += Step)
	{
		float cosa = cos(a);
		float sina = sin(a);
		float X1 = r * cosa + c.x;
		float Y1 = r * sina + c.y;
		vertices[count].position = XMFLOAT3(X1, Y1, 0);
		//vertices[count].texture = XMFLOAT2(cosa * 0.5f + 0.5f, sina* 0.5f + 0.5f);
		vertices[count].texture = XMFLOAT2(0.3f, 0.5f);

		count += 1;
	}


	count = 1;
	for (size_t i = 0; i < m_indexCount; i += 3)
	{
		if (i + 3 < m_indexCount) {
			indices[i] = 0;
			indices[i + 1] = count + 1;
			indices[i + 2] = count;
		}
		else {
			indices[i] = 0;
			indices[i + 1] = 1;
			indices[i + 2] = count;
		}
		count++;
	}

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void CircleDrawer::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void CircleDrawer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool CircleDrawer::LoadTexture(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		MyOutputDebugString(L"failed loadTexture %s", filename);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MyOutputDebugString(L"failed loadTexture %s", filename);
		return false;
	}

	return true;
}

void CircleDrawer::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}
