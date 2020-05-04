#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>

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

class Circle;

class CircleDrawer : public ObjectDrawer {
public:
	CircleDrawer();

	void SetCircle(Circle*);


private:
	Circle* m_obj;

};
