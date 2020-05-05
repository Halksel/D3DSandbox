#include "gameclass.h"

GameClass::GameClass() : square(Square()){
	m_ObjectHolder.reserve(256);
	for (size_t i = 0; i < HolderSize; i++)
	{
		m_ObjectHolder.push_back(nullptr);
	}
}

bool GameClass::Initialize(int width, int height) {

	AddObject(new Circle("ball", XMFLOAT3(0, -5, 0), 3, 50));
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 12; i++)
		{
			AddObject(new Square("test", XMFLOAT3(-6 + i, 8 - j, 0), XMFLOAT3(1, 1, 0)));
		}
	}
	return true;
}

void GameClass::Shutdown()
{
	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->Shutdown();
			obj->GetDrawer()->Shutdown();
			obj = nullptr;
		}
	}
}

bool GameClass::Frame(const InputClass& input) {

	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->Update(input);
		}
	}
	return true;
}

void GameClass::SetCamera(CameraClass* camera)
{
	m_Camera = camera;
}

bool GameClass::AddObject(Object* obj)
{
	if (m_objIndex >= HolderSize) {
		return false;
	}

	m_ObjectHolder[m_objIndex++] = obj;
	return true;
}

std::vector<Object*> GameClass::GetObjectHolder()
{
	return m_ObjectHolder;
}
