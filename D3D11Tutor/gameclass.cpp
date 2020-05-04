#include "gameclass.h"

GameClass::GameClass() : square(Square()){
	m_ObjectHolder.reserve(256);
	for (size_t i = 0; i < HolderSize; i++)
	{
		m_ObjectHolder.push_back(nullptr);
	}
}

bool GameClass::Initialize(int width, int height) {

	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 12; i++)
		{
			m_ObjectHolder[m_objIndex++] = new Square("test", XMFLOAT3(-6 + i, 8 - j, 0), XMFLOAT3(1, 1, 0));
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

std::vector<Object*> GameClass::GetObjectHolder()
{
	return m_ObjectHolder;
}
