#include "gameclass.h"

//GameClass::GameClass() : m_Bar(Bar()), m_Ball(Ball()){
GameClass::GameClass() : m_ObjIndex(0){
	m_ObjectHolder.reserve(256);
	for (size_t i = 0; i < HolderSize; i++)
	{
		m_ObjectHolder.push_back(nullptr);
	}
}

bool GameClass::Initialize(int width, int height) {

	m_Bar = make_shared<Bar>("bar", XMFLOAT3(-2, -8, 0), XMFLOAT3(4, 0.5f, 0));
	AddObject(m_Bar);
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 12; i++)
		{
			AddObject<Rect>(make_unique<Rect>("test", XMFLOAT3(-6 + i, 8 - j, 0), XMFLOAT3(1, 1, 0)));
		}
	}
	AddObject<Rect>(make_unique<Rect>("wall", XMFLOAT3(-8 , 10 , 0), XMFLOAT3(16, 1, 0)));
	AddObject<Rect>(make_unique<Rect>("wall", XMFLOAT3(-8.5 , -7 , 0), XMFLOAT3(1, 16, 0)));
	AddObject<Rect>(make_unique<Rect>("wall", XMFLOAT3(7.5 , -7 , 0), XMFLOAT3(1, 16, 0)));
	//AddObject<Rect>(make_unique<Rect>("wall", XMFLOAT3(8 , 9 , 0), XMFLOAT3(16, 1, 0)));
	m_Ball = make_shared<Ball>(XMFLOAT3(-2, -8, 0), 3.0f, 30.0f, 0.3f);
	AddObject<Ball>(m_Ball);
	return true;
}

void GameClass::Shutdown()
{
	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->Shutdown();
			obj->GetDrawer()->Shutdown();
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
						m_ObjectHolder[i]->AddCrossObject(m_ObjectHolder[j]->GetSharedPtr());
						m_ObjectHolder[j]->AddCrossObject(m_ObjectHolder[i]->GetSharedPtr());
					}
				}
			}
		}
	}
	for (auto& obj : m_ObjectHolder) {
		if (obj != nullptr) {
			obj->CollisionAfter();
		}
	}

	return true;
}

template<class T>
std::shared_ptr<Object> GameClass::AddObject(std::unique_ptr<T>&& uptr)
{
	if (m_ObjIndex >= HolderSize) {
		return nullptr;
	}

	return m_ObjectHolder[m_ObjIndex++] = std::move(uptr);
}

template<class T>
std::shared_ptr<Object> GameClass::AddObject(std::shared_ptr<T>& sptr)
{
	if (m_ObjIndex >= HolderSize) {
		return nullptr;
	}

	return m_ObjectHolder[m_ObjIndex++] = sptr;
}

std::vector<shared_ptr<Object>>& GameClass::GetObjectHolder()
{
	return m_ObjectHolder;
}
