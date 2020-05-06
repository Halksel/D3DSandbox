#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "objectdrawer.h"
#include "../inputclass.h"
#include "../modelclass.h"
#include "../utility.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class ObjectDrawer;

class Object {
public:
	Object();
	enum ObjectType {
		OBJECT = 0,
		RECT,
		CIRCLE,
		MAX
	};

	bool Initialize();
	virtual bool OnInitialize() = 0;
	void Update(InputClass&);
	virtual void OnUpdate(InputClass&) = 0;
	void Shutdown();
	virtual void OnShutdown() = 0;
	ObjectDrawer* GetDrawer();
	virtual ObjectDrawer* GetDrawerSub() = 0;

	std::string GetName();
	void SetName(std::string);
	ObjectType GetType();
	virtual bool IsCross(Object*) = 0;

protected:
	std::string m_Name;
	ObjectType m_Type;

private:

};

class RectDrawer;

class Rect : public Object {
public:
	explicit Rect() = default;
	explicit Rect(Rect const&) = default;
	explicit Rect(Rect&&) = default;
	//Rect& operator=(Rect &&) = default;
	Rect& operator=(const Rect&);

	explicit Rect(std::string name, XMFLOAT3 p0 = XMFLOAT3(), XMFLOAT3 p1 = XMFLOAT3());

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;
	bool IsCross(Object*) override;

	float GetWidth();
	float GetHeight();
	XMFLOAT3 GetCenter();

	XMFLOAT3 GetPoint(int i);
	void SetSquare(XMFLOAT3*, XMFLOAT3*);
	void Move(XMFLOAT3 vector);

protected:
	std::vector<XMVECTOR> m_points;
	bool CrossCircleRect(float L, float R, float T, float B, float x, float y, float radius);

private:
	RectDrawer *m_Drawer;
};

class CircleDrawer;

class Circle : public Object {
public:
	explicit Circle();
	explicit Circle(std::string name, XMFLOAT3 p0, float r, int strides);
	explicit Circle(Circle&&) = default;
	Circle& operator=(const Circle&);

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;
	bool IsCross(Object*) override;

	XMFLOAT3 GetCenter();
	float GetRadius();
	int GetStrides();

private:
	XMVECTOR m_Center;
	float m_Radius;
	int m_Strides;
	CircleDrawer* m_Drawer;
};
