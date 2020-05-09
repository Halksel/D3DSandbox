#include "Ball.h"
#include "singleton.h"

using namespace DirectX;

Ball::Ball() 
{
}

Ball::Ball(XMFLOAT3 p, XMFLOAT3 v, XMFLOAT3 a, float r) : Circle("ball", p, r, 50),
	m_Vector(v),
	m_InitPoint(p),
	m_InitVector(v),
	m_Accelete(a)
{
}

Ball& Ball::operator=(Ball const& r)
{
	*this = Ball(r);
	return *this;
}

bool Ball::OnInitialize()
{
	return true;
}

void Ball::OnUpdate(InputClass& input)
{
	auto p = GetCenter();
	auto x = p.x;
	auto y = p.y;
	auto v = XMLoadFloat3(&m_Vector);
	auto v0 = XMLoadFloat3(&m_InitVector);
	auto a = XMLoadFloat3(&m_Accelete);
	auto time = Singleton<TimeClass>::get_instance();
	auto t = time.GetMyCurrentTime();
	auto dt = time.GetDeltaTime();
	v = v0 + a * t;
	auto l = XMVectorGetX(XMVector3LengthSq(v));
	float scale = 1.0f;
	if (l > MaxSpeed * MaxSpeed) {
		scale = MaxSpeed * MaxSpeed / l;
	}
	v = v * scale;

	XMStoreFloat3(&m_Vector, v);
	auto np = XMFLOAT3(x + m_Vector.x * dt * cos(m_Rad), y + m_Vector.y * dt * sin(m_Rad), 0.0f);
	m_Center = XMLoadFloat3(&np);
}

void Ball::OnShutdown()
{
}
