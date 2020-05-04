////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;

#include "textureclass.h"
#include "textureshaderclass.h"
#include "utility.h"
#include "Object/object.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////

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
	SquareDrawer();
	SquareDrawer(const SquareDrawer&);
	~SquareDrawer();

	bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*, LPCSTR);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	void Shutdown();
	void SetSquare(Square*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

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

#endif