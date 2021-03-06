#include "Bar.h"
#include "Ball.h"
#include "utility.h"
using namespace DirectX;

Bar::Bar()
{
}

Bar::Bar(std::string name, XMFLOAT3 p0, XMFLOAT3 p1) : Rect(name, p0, p1) 
{
	m_Type = BAR;
}

Bar& Bar::operator=(const Bar& r)
{
	m_Speed = r.m_Speed;
	m_points = r.m_points;
	m_Drawer = r.m_Drawer;
	m_Drawer->SetSquare(this);
//	*this = Bar(r);
	return *this;
}

bool Bar::OnInitialize()
{
	return true;
}

void Bar::OnUpdate(InputClass& input)
{
	Rect::OnUpdate(input);
	float x = 0.0f, y = 0.0f;
	if (input.IsKeyDown(DIK_A)) {
		x = -m_Speed;
	}
	else if (input.IsKeyDown(DIK_D)) {
		x = m_Speed;
	}
	Move(XMFLOAT3(x, y, 0));
}

void Bar::OnCollisionAfter() {
	for (auto& obj : m_CrossList) {
		if (!obj.expired()) {
			auto pt = obj.lock();
			if (pt != nullptr) {
				if (pt->GetType() == ObjectType::BALL) {
					auto ball = std::dynamic_pointer_cast<Ball>(pt);
					auto width = GetWidth() * 0.5f;
					auto diff = GetCenter().x - ball->GetCenter().x;
					auto value = min(1.0f, abs(diff / width)) * (signbit(diff) ? 1 : -1 );
					ball->SetRadian( acos(value));
				}
			}
		}
	}

}

void Bar::OnShutdown()
{
}
