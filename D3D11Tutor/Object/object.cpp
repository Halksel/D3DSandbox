#include "object.h"

Object::Object()
{
}

bool Object::Initialize()
{
	return OnInitialize();
}

void Object::Update(const InputClass& input)
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

Square::Square()
{
	m_Name = "";
}

Square::Square(std::string name, XMFLOAT3 p0, XMFLOAT3 size)
{
	SetSquare(&p0, &size);
	m_Drawer = new SquareDrawer;
	m_Drawer->SetSquare(this);
	m_Name = name;
}

Square& Square::operator=(const Square& r)
{
	for (size_t i = 0; i < 4; i++)
	{
		m_points[i] = r.m_points[i];
	}
	delete m_Drawer;
	m_Drawer = r.m_Drawer;
	m_Drawer->SetSquare(this);
	return *this;
}

bool Square::OnInitialize() 
{
	return true;
}

void Square::OnUpdate(const InputClass& input) {

}

void Square::OnShutdown()
{

}

ObjectDrawer* Square::GetDrawerSub() 
{
	return m_Drawer;
}

float Square::GetWidth()
{
	return XMVectorGetX( m_points[3] - m_points[0]);
}

float Square::GetHeight()
{
	return XMVectorGetY( m_points[3] - m_points[0]);
}

XMVECTOR Square::GetCenter()
{
	return (m_points[0] + m_points[3]) / 2;
}

XMFLOAT3 Square::GetPoint(int i)
{
	XMFLOAT3 point;
	XMStoreFloat3(&point, m_points[i]);
	return point;
}

void Square::SetSquare(XMFLOAT3 *p0, XMFLOAT3 *size)
{
	auto tl = XMFLOAT3(p0->x, p0->y + size->y, 0);
	auto br = XMFLOAT3(p0->x + size->x, p0->y, 0);
	auto tr = XMFLOAT3(p0->x + size->x, p0->y + size->y, 0);
	m_points[0] = XMLoadFloat3(p0);
	m_points[1] = XMLoadFloat3(&tl);
	m_points[2] = XMLoadFloat3(&br);
	m_points[3] = XMLoadFloat3(&tr);
}

Circle::Circle()
{
}

Circle::Circle(std::string name, XMFLOAT3 p0, float r)
{
}

Circle& Circle::operator=(const Circle& r)
{
	m_center = r.m_center;
	delete m_drawer;
	m_drawer = r.m_drawer;
	m_drawer->SetCircle(this);

	return *this;
}

bool Circle::OnInitialize()
{
	return false;
}

void Circle::OnUpdate(const InputClass& input)
{
}

void Circle::OnShutdown()
{
}

ObjectDrawer* Circle::GetDrawerSub()
{
	return nullptr;
}
