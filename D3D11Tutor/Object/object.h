#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>

#include "objectdrawer.h"
#include "../inputclass.h"
#include "../modelclass.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class ObjectDrawer;

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
	explicit Square();
	explicit Square(std::string name, XMFLOAT3 p0 = XMFLOAT3(), XMFLOAT3 p1 = XMFLOAT3());
	explicit Square(Square&&) = default;
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
	explicit Circle();
	explicit Circle(std::string name, XMFLOAT3 p0, float r, int strides);
	explicit Circle(Circle&&) = default;
	Circle& operator=(const Circle&);

	bool OnInitialize() override;
	void OnUpdate(const InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;

	XMVECTOR GetCenter();
	float GetRadius();
	int GetStrides();

private:
	XMVECTOR m_Center;
	float m_Radius;
	int m_Strides;
	CircleDrawer* m_Drawer;
};
