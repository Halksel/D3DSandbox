#include "Bar.h"
using namespace DirectX;

Bar::Bar()
{
}

Bar::Bar(std::string name, XMFLOAT3 p0, XMFLOAT3 p1) : Rect(name, p0, p1) 
{
}

Bar& Bar::operator=(const Bar& r)
{
	*this = Bar(r);
	return *this;
}

bool Bar::OnInitialize()
{
	return false;
}

void Bar::OnUpdate(InputClass& input)
{
	Rect::OnUpdate(input);
	float x = 0.0f, y = 0.0f;
	if (input.IsKeyDown(DIK_A)) {
		x = -1;
	}
	else if (input.IsKeyDown(DIK_D)) {
		x = 1;
	}
	Move(XMFLOAT3(x, y, 0));
}

void Bar::OnShutdown()
{
}
