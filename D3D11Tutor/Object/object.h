#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>

#include "objectdrawer.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class Object {
public:
	Object();
	Object(std::string nam1e, ObjectDrawer*);
	enum ObjectType {
		OBJECT = 0,
		SQUARE,
		MAX
	};

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	std::string GetName();
	void SetName(std::string);
	ObjectType GetType();

protected:
	std::string m_Name;
	ObjectType m_Type;
	ObjectDrawer *m_Drawer;

private:

};

class Square : Object {
public:
	Square();
	Square(std::string name = "", XMFLOAT2 p0 = XMFLOAT2(), XMFLOAT2 p1 = XMFLOAT2());

	float GetWidth();
	float GetHeight();
	XMVECTOR GetCenter();

	XMFLOAT2 GetPoint0();
	XMFLOAT2 GetPoint1();
	void SetPoint0(XMFLOAT2*);
	void SetPoint1(XMFLOAT2*);

protected:


private:
	XMVECTOR m_p0;
	XMVECTOR m_p1;
};
