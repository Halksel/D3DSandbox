#pragma once


#include"inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "Object/object.h"
#include "Bar.h"
#include "Ball.h"

#include <vector>
#include <memory>

class GameClass {
public:
	GameClass();
	bool Initialize(int, int);
	void Shutdown();
	bool Frame(InputClass&);

	template<class T>
	std::shared_ptr<Object> AddObject(std::unique_ptr<T>&&);
	template<class T>
	std::shared_ptr<Object> AddObject(std::shared_ptr<T>&);

	std::vector <std::shared_ptr<Object>>& GetObjectHolder();

private:
	shared_ptr<Bar> m_Bar;
	shared_ptr<Ball> m_Ball;
	std::vector<std::shared_ptr<Object>> m_ObjectHolder;
	int m_ObjIndex;
	const int HolderSize = 256;
};
