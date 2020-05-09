#pragma once
#include "Object/object.h"
#include "Time.h"

class Ball : public Circle {
public:
	explicit Ball();
	explicit Ball(XMFLOAT3 p, XMFLOAT3 v, XMFLOAT3 a, float r);
	explicit Ball(Ball const&) = default;
	explicit Ball(Ball&&) = default;
	Ball& operator=(Ball&&) = default;
	Ball& operator=(Ball const&);

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnShutdown() override;
private:

private:
	float m_Rad;
	XMFLOAT3 m_InitPoint;
	XMFLOAT3 m_InitVector;
	XMFLOAT3 m_Vector;
	XMFLOAT3 m_Accelete;

	float MaxSpeed = 2.0f;
};
