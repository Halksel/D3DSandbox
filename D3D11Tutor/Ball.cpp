#include "Ball.h"
#include "singleton.h"

using namespace DirectX;

Ball::Ball(XMFLOAT3 p, float v, float deg, float r) : Circle("ball", p, r, 50),
m_InitPoint(p),
m_Rad(deg* Deg2Rad),
m_InitRad(deg* Deg2Rad),
m_Velocity(v)
{
	m_Type = BALL;
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
	if (m_Vector.x == 0) return;
	m_Vector = XMFLOAT3(m_Velocity* cos(m_Rad), m_Velocity* sin(m_Rad), 0.0f);
	auto p = GetCenter();
	m_BeforePoint = GetCenter();
	auto x = p.x;
	auto y = p.y;
	auto v = XMLoadFloat3(&m_Vector);
	auto time = Singleton<TimeClass>::get_instance();
	auto t = time.GetMyCurrentTime();
	auto dt = time.GetDeltaTime();
	auto l = XMVectorGetX(XMVector3Length(v));
	float scale = 1.0f;
	if (l > MaxSpeed * MaxSpeed) {
		scale = MaxSpeed * MaxSpeed / l;
	}
	v = v * scale;

	XMStoreFloat3(&m_Vector, v);
	XMStoreFloat3(&m_DeltaVector, v * dt);
	auto np = XMFLOAT3(x + m_Vector.x * dt, y + m_Vector.y * dt, 0.0f);
	m_Center = XMLoadFloat3(&np);
}

void Ball::OnShutdown()
{
}

void Ball::OnCollisionAfter()
{
	for (auto obj : m_CrossList)
	{
		if (obj.expired()) continue;
		auto ptr = obj.lock();
		if (ptr->GetType() == Object::RECT) {
			auto p = m_BeforePoint;
			auto rect = dynamic_pointer_cast<Rect>(ptr);
			auto cp = rect->GetAfterIntersectPoint(p, m_DeltaVector, GetRadius(), &m_Rad);
			if (cp.x == -1 && cp.z == -1) continue;

			m_BeforePoint = cp;
			m_Center = XMLoadFloat3(&cp);
			break;
		}
	}
}
