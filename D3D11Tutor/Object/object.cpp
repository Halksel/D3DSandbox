#include "object.h"

Object& Object::operator=(const Object& r)
{
	return *this;
}

bool Object::Initialize()
{
	return OnInitialize();
}

void Object::Update(InputClass& input)
{
	OnUpdate(input);
}

void Object::CollisionAfter() {
	OnCollisionAfter();
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

void Object::AddCrossObject(shared_ptr<Object>&& obj)
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
	m_Type = RECT;
}

Rect& Rect::operator=(const Rect& r)
{
	m_points = r.m_points;
	m_Drawer = r.m_Drawer;
	m_Drawer->SetSquare(this);
	return *this;
}

bool Rect::OnInitialize() 
{
	return true;
}

void Rect::OnUpdate(InputClass& input) {

}

void Rect::OnCollisionAfter() {

}

void Rect::OnShutdown()
{
	m_Drawer->Shutdown();

}

ObjectDrawer* Rect::GetDrawerSub() 
{
	return m_Drawer;
}

bool Rect::IsCross(const std::shared_ptr<Object>& obj)
{
	switch (obj->GetType())
	{
		case Object::CIRCLE: 
		case Object::BALL: 
		{
			auto cir = dynamic_pointer_cast<Circle>(obj);
			auto center = cir->GetCenter();
			return CrossCircleRect(GetPoint(0).x, GetPoint(2).x, GetPoint(0).y, GetPoint(2).y, center.x, center.y, cir->GetRadius());
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
	m_points[2] = XMLoadFloat3(&tr);
	m_points[3] = XMLoadFloat3(&br);
}

void Rect::Move(XMFLOAT3 vector)
{
	auto v = XMLoadFloat3(&vector);
	for (size_t i = 0; i < 4; i++)
	{
		m_points[i] += v;
	}
}

std::shared_ptr<Object> Rect::GetSharedPtr()
{
	return shared_from_this();
}


XMFLOAT3 Rect::GetAfterIntersectPoint(XMFLOAT3 p, XMFLOAT3 delta, float r, float *rad)
{
	for (int i = 0; i < 4; ++i) {
		XMVECTOR ab;
		XMFLOAT3 a, b;
		if (i == 3) {
			ab = m_points[0] - m_points[i];
			a = GetPoint(i);
			b = GetPoint(0);
		}
		else {
			ab = m_points[i + 1] - m_points[i];
			a = GetPoint(i);
			b = GetPoint(i + 1);
		}
		XMFLOAT3 v;
		XMStoreFloat3(&v, ab);
		auto abx = v.x;
		auto aby = v.y;
		auto nx = -aby;
		auto ny = abx;
		auto sx = -nx * r;
		auto sy = -ny * r;
		auto l = sqrt(nx * nx + ny * ny);
		if (l > 0) l = 1 / l;
		nx *= l;
		ny *= l;
		auto d = -(a.x * nx + a.y * ny);
		auto t = -(nx * p.x + ny * p.y + d) / (nx * sx + ny * sy);
		if (t <= 0 || t > 1) continue;
		auto cx = p.x + sx * t;
		auto cy = p.y + sy * t;

		auto acx = cx - a.x;
		auto acy = cy - a.y;
		auto bcx = cx - b.x;
		auto bcy = cy - b.y;
		auto doc = (acx * bcx) + (acy * bcy);
		if (doc > 0) continue;

		auto mx = cx + r * nx;
		auto my = cy + r * ny;
		auto mt = -(nx * delta.x + ny * delta.y)/(nx * nx + ny * ny);
		auto m_dx = t * nx * 2;
		auto m_dy = t * ny * 2;
		
		if (i == 1 || i == 3) {
			*rad *= -1;
		}
		else {
			//*rad *= -1;
			*rad = (180 * Deg2Rad - *rad);
		}
		return XMFLOAT3(mx , my , 0);
	}
	return XMFLOAT3(-1, 0, -1);
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

Circle::Circle(std::string name, XMFLOAT3 p0, float r, int num) : m_Radius(r), m_Strides(num)
{
	m_Name = name;
	m_Center = XMLoadFloat3( &p0);
	m_Drawer = new CircleDrawer;
	m_Drawer->SetCircle(this);
	m_Type = CIRCLE;
}

Circle& Circle::operator=(const Circle& r)
{
	m_Center = r.m_Center;
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

bool Circle::IsCross(const std::shared_ptr<Object>& obj)
{
	switch (obj->GetType())
	{
	case Object::CIRCLE:
	case Object::BALL:
		return false;
	case Object::RECT:
	case Object::BAR:
		{
			return obj->IsCross(GetSharedPtr());
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

std::shared_ptr<Object> Circle::GetSharedPtr() 
{
	return shared_from_this();
}
