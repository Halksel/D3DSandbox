#include "gameclass.h"

GameClass::GameClass() : m_Bar(Bar()), m_Ball(Ball()){
	m_ObjectHolder.reserve(256);
	for (size_t i = 0; i < HolderSize; i++)
	{
		m_ObjectHolder.push_back(nullptr);
	}
}

bool GameClass::Initialize(int width, int height) {

	m_Bar = Bar("bar", XMFLOAT3(-2, -8, 0), XMFLOAT3(4, 0.5f, 0));
	AddObject(&m_Bar);
	m_Ball = Ball(XMFLOAT3(-2, -8, 0), XMFLOAT3(4, 0.5f, 0), XMFLOAT3(0.5f, 0.5f, 0), 0.3f);
	AddObject(&m_Ball);
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 12; i++)
		{
			AddObject(new Rect("test", XMFLOAT3(-6 + i, 8 - j, 0), XMFLOAT3(1, 1, 0)));
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
//			delete obj;
			obj = nullptr;
		}
	}
}

bool GameClass::Frame(InputClass& input) {

	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->ResetCrossObject();
		}
	}
	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->Update(input);
		}
	}
	for (size_t i = 0; i < m_ObjectHolder.size(); i++)
	{
		if (m_ObjectHolder[i] != nullptr) {
			for (size_t j = i + 1; j < m_ObjectHolder.size(); j++)
			{
				if (m_ObjectHolder[j] != nullptr) {
					if (m_ObjectHolder[i]->IsCross(m_ObjectHolder[j])) {
						m_ObjectHolder[i]->AddCrossObject(m_ObjectHolder[j]);
						m_ObjectHolder[j]->AddCrossObject(m_ObjectHolder[i]);
					}
				}
			}
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
	if (m_ObjIndex >= HolderSize) {
		return false;
	}

	m_ObjectHolder[m_ObjIndex++] = obj;
	return true;
}

std::vector<Object*> GameClass::GetObjectHolder()
{
	return m_ObjectHolder;
}
