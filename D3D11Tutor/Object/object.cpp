#include "object.h"

Object::Object()
{
}

bool Object::Initialize()
{
	return OnInitialize();
}

void Object::Update(InputClass& input)
{
	OnUpdate(input);
}

void Object::Shutdown()
{
	OnShutdown();
}

ObjectDrawer* Object::GetDrawer()
{
	return GetDrawerSub();
}

std::string Object::GetName() {
	return m_Name;
}

void Object::SetName(std::string name) {
	m_Name = name;
}

Object::ObjectType Object::GetType()
{
	return m_Type;
}

void Object::AddCrossObject(Object* obj)
{
	m_CrossList.push_back(obj);
}

void Object::ResetCrossObject()
{
	m_CrossList.clear();
}

Rect::Rect(std::string name, XMFLOAT3 p0, XMFLOAT3 size)
{
	SetSquare(&p0, &size);
	m_Drawer = new RectDrawer;
	m_Drawer->SetSquare(this);
	m_Name = name;
}

Rect& Rect::operator=(const Rect& r)
{
	m_points = std::move(r.m_points);
	delete m_Drawer;
	m_Drawer = r.m_Drawer;
	m_Drawer->SetSquare(this);
	//*this = Rect(r);
	return *this;
}

bool Rect::OnInitialize() 
{
	return true;
}

void Rect::OnUpdate(InputClass& input) {

}

void Rect::OnShutdown()
{
	m_Drawer->Shutdown();

}

ObjectDrawer* Rect::GetDrawerSub() 
{
	return m_Drawer;
}

bool Rect::IsCross(Object* obj)
{
	switch (obj->GetType())
	{
		case Object::CIRCLE: 
		{
			auto cir = static_cast<Circle*>(obj);
			auto center = cir->GetCenter();
			return CrossCircleRect(GetPoint(0).x, GetPoint(3).x, GetPoint(0).y, GetPoint(3).y, center.x, center.y, cir->GetRadius());
		}
		break;
		case Object::RECT: 
		{
			return false;
		}
		default:
			return false;
	}
}

float Rect::GetWidth()
{
	return XMVectorGetX( m_points[3] - m_points[0]);
}

float Rect::GetHeight()
{
	return XMVectorGetY( m_points[3] - m_points[0]);
}

XMFLOAT3 Rect::GetCenter()
{
	XMFLOAT3 point;
	XMStoreFloat3(&point, (m_points[0] + m_points[3]) / 2);
	return point;
}

XMFLOAT3 Rect::GetPoint(int i)
{
	XMFLOAT3 point;
	XMStoreFloat3(&point, m_points[i]);
	return point;
}

void Rect::SetSquare(XMFLOAT3 *p0, XMFLOAT3 *size)
{
	auto tl = XMFLOAT3(p0->x, p0->y + size->y, 0);
	auto br = XMFLOAT3(p0->x + size->x, p0->y, 0);
	auto tr = XMFLOAT3(p0->x + size->x, p0->y + size->y, 0);
	m_points.resize(4);
	m_points[0] = XMLoadFloat3(p0);
	m_points[1] = XMLoadFloat3(&tl);
	m_points[2] = XMLoadFloat3(&br);
	m_points[3] = XMLoadFloat3(&tr);
}

void Rect::Move(XMFLOAT3 vector)
{
	auto v = XMLoadFloat3(&vector);
	for (size_t i = 0; i < 4; i++)
	{
		m_points[i] += v;
	}
}

bool Rect::CrossCircleRect(float L, float R, float T, float B, float x, float y, float radius) {
	if (L - radius > x || R + radius < x || T - radius > y || B + radius < y) {//矩形の領域判定1
		return false;
	}
	if (L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < radius * radius)) {//左上の当たり判定
		return false;
	}
	if (R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < radius * radius)) {//右上の当たり判定
		return false;
	}
	if (L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < radius * radius)) {//左下の当たり判定
		return false;
	}
	if (R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < radius * radius)) {//右下の当たり判定
		return false;
	}
	return true;//すべての条件が外れたときに当たっている
}

Circle::Circle()
{
}

Circle::Circle(std::string name, XMFLOAT3 p0, float r, int num)
{
	m_Name = name;
	m_Center = XMLoadFloat3( &p0);
	m_Radius = r;
	m_Strides = num;
	m_Drawer = new CircleDrawer;
	m_Drawer->SetCircle(this);
}

Circle& Circle::operator=(const Circle& r)
{
	m_Center = r.m_Center;
	delete m_Drawer;
	m_Drawer = r.m_Drawer;
	m_Drawer->SetCircle(this);

	return *this;
}

bool Circle::OnInitialize()
{
	return true;
}

void Circle::OnUpdate(InputClass& input)
{
}

void Circle::OnShutdown()
{
	m_Drawer->Shutdown();
}

ObjectDrawer* Circle::GetDrawerSub()
{
	return m_Drawer;
}

bool Circle::IsCross(Object* obj)
{
	switch (obj->GetType())
	{
	case Object::CIRCLE:
		return false;
	case Object::RECT:
		{
			auto rect = static_cast<Rect*>(obj);
			return rect->IsCross(this);
		}
	}
}

XMFLOAT3 Circle::GetCenter()
{
	XMFLOAT3 point;
	XMStoreFloat3(&point, m_Center);
	return point;
}

float Circle::GetRadius()
{
	return m_Radius;
}

int Circle::GetStrides()
{
	return m_Strides;
}
