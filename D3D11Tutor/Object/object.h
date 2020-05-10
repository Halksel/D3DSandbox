#pragma once

#include<string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "objectdrawer.h"
#include "../inputclass.h"
#include "../modelclass.h"
#include "../utility.h"

#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class ObjectDrawer;

class Object : public std::enable_shared_from_this<Object> {
public:
	explicit Object() = default;
	explicit Object(Object const&) = default;
	explicit Object(Object &&) = default;
	Object& operator=(Object &&) = default;
	Object& operator=(const Object&);

	enum ObjectType {
		OBJECT = 0,
		RECT,
		CIRCLE,
		BAR,
		BALL,
		MAX
	};

	bool Initialize();
	virtual bool OnInitialize() = 0;
	void Update(InputClass&);
	virtual void OnUpdate(InputClass&) = 0;
	void CollisionAfter();
	virtual void OnCollisionAfter() = 0;
	void Shutdown();
	virtual void OnShutdown() = 0;

	ObjectDrawer* GetDrawer();
	virtual ObjectDrawer* GetDrawerSub() = 0;
	virtual std::shared_ptr<Object> GetSharedPtr() = 0;

	std::string GetName();
	void SetName(std::string);
	ObjectType GetType();
	virtual bool IsCross(const std::shared_ptr<Object>&) = 0;
	void AddCrossObject(std::shared_ptr<Object>&&);
	void ResetCrossObject();

protected:
	std::string m_Name;
	ObjectType m_Type;
	std::vector<std::weak_ptr<Object>> m_CrossList;

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
	void OnCollisionAfter();
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;
	bool IsCross(const std::shared_ptr<Object>&) override;

	float GetWidth();
	float GetHeight();
	XMFLOAT3 GetCenter();

	XMFLOAT3 GetPoint(int i);
	void SetSquare(XMFLOAT3*, XMFLOAT3*);
	void Move(XMFLOAT3 vector);
	std::shared_ptr<Object> GetSharedPtr() override;
	XMFLOAT3 GetAfterIntersectPoint(XMFLOAT3, XMFLOAT3, float r, float *rad);

protected:
	std::vector<XMVECTOR> m_points;
	RectDrawer *m_Drawer;
	bool CrossCircleRect(float L, float R, float T, float B, float x, float y, float radius);

private:
};

class CircleDrawer;

class Circle : public Object {
public:
	explicit Circle() = default;
	explicit Circle(std::string name, XMFLOAT3 p0, float r, int strides);
	explicit Circle(Circle const&) = default;
	explicit Circle(Circle&&) = default;
	Circle& operator=(const Circle&);

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnShutdown() override;
	ObjectDrawer* GetDrawerSub() override;
	bool IsCross(const std::shared_ptr<Object>&) override;

	XMFLOAT3 GetCenter();
	float GetRadius();
	int GetStrides();
	std::shared_ptr<Object> GetSharedPtr() override;

protected:
	XMVECTOR m_Center;
	float m_Radius;

private:
	int m_Strides;
	CircleDrawer* m_Drawer;
};
