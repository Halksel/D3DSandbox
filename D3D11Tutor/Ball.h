#pragma once
#include "Object/object.h"
#include "Time.h"

class Ball : public Circle {
public:
	explicit Ball() = default;
	explicit Ball(XMFLOAT3 p, float v, float deg, float r);
	explicit Ball(Ball const&) = default;
	explicit Ball(Ball&&) = default;
	Ball& operator=(Ball&&) = default;
	Ball& operator=(Ball const&);

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnShutdown() override;
	void OnCollisionAfter() override;
private:

private:
	float m_Rad;
	float m_InitRad;
	float m_Velocity;
	XMFLOAT3 m_InitPoint;
	XMFLOAT3 m_BeforePoint;
	XMFLOAT3 m_DeltaVector;
	XMFLOAT3 m_Vector;

	float MaxSpeed = 2.0f;
};
