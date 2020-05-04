#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>

#include "objectdrawer.h"
#include "../inputclass.h"
#include "../modelclass.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class Object {
public:
	Object();
	enum ObjectType {
		OBJECT = 0,
		SQUARE,
		MAX
	};

	bool Initialize();
	virtual bool OnInitialize() = 0;
	void Update(const InputClass&);
	virtual void OnUpdate(const InputClass&) = 0;
	void Shutdown();
	virtual void OnShutdown() = 0;
	ObjectDrawer* GetDrawer();
	virtual ObjectDrawer* GetDrawerSub() = 0;

	std::string GetName();
	void SetName(std::string);
	ObjectType GetType();

protected:
	std::string m_Name;
	ObjectType m_Type;

private:

};

class SquareDrawer;

class Square : public Object {
public:
	Square();
	Square(std::string name, XMFLOAT3 p0 = XMFLOAT3(), XMFLOAT3 p1 = XMFLOAT3());
	Square(Square&&) = default;
	Square& operator=(const Square&);

	bool OnInitialize() override;
	void OnUpdate(const InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;

	float GetWidth();
	float GetHeight();
	XMVECTOR GetCenter();

	XMFLOAT3 GetPoint(int i);
	void SetSquare(XMFLOAT3*, XMFLOAT3*);

protected:


private:
	XMVECTOR m_points[4];
	SquareDrawer *m_Drawer;
};

class CircleDrawer;

class Circle : public Object {
public:
	Circle();
	Circle(std::string name, XMFLOAT3 p0, float r);
	Circle(Circle&&) = default;
	Circle& operator=(const Circle&);

	bool OnInitialize() override;
	void OnUpdate(const InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;

private:
	XMVECTOR m_center;
	CircleDrawer* m_drawer;

};
