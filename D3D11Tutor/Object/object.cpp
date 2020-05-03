#include "object.h"

Object::Object()
{
}

Object::Object(std::string name, ObjectDrawer* drawer)
{
	m_Name = name;
	m_Drawer = drawer;
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
	XMFLOAT2 point;
	m_p0 = XMLoadFloat2(&point);
	m_p1 = XMLoadFloat2(&point);
}

Square::Square(std::string name, XMFLOAT2 p0, XMFLOAT2 p1)
{
	m_Name = name;
	m_p0 = XMLoadFloat2(&p0);
	m_p1 = XMLoadFloat2(&p1);
}

float Square::GetWidth()
{
	return XMVectorGetX( m_p1 - m_p0);
}

float Square::GetHeight()
{
	return XMVectorGetY( m_p1 - m_p0);
}

XMVECTOR Square::GetCenter()
{
	return (m_p0 + m_p1) / 2;
}

XMFLOAT2 Square::GetPoint0()
{
	XMFLOAT2 point;
	XMStoreFloat2(&point, m_p0);
	return point;
}

XMFLOAT2 Square::GetPoint1()
{
	XMFLOAT2 point;
	XMStoreFloat2(&point, m_p1);
	return point;
}

void Square::SetPoint0(XMFLOAT2 *p0)
{
	m_p0 = XMLoadFloat2(p0);
}

void Square::SetPoint1(XMFLOAT2 *p1)
{
	m_p1 = XMLoadFloat2(p1);
}
