#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>
#include "object.h"
#include "../utility.h"
#include "../textureclass.h"
#include "../textureshaderclass.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class ObjectDrawer {
public:
	ObjectDrawer();

	virtual bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR) = 0;
	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj) = 0;
	virtual void Shutdown() = 0;

protected:
	bool m_initialized;

private:

};

class Square;

class SquareDrawer : public ObjectDrawer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture ;
	};

public:
	explicit SquareDrawer();
	explicit SquareDrawer(const SquareDrawer&);
	~SquareDrawer();

	bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	void Shutdown();
	void SetSquare(Square*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR);
	void ReleaseTexture();


private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	TextureShaderClass* m_TextureShader;
	Square* m_obj;
};

class Circle;

class CircleDrawer : public ObjectDrawer {
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture ;
	};
	explicit CircleDrawer();
	explicit CircleDrawer(const CircleDrawer&);

	bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	void Shutdown();

	void SetCircle(Circle*);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	TextureShaderClass* m_TextureShader;
	Circle* m_obj;
};

