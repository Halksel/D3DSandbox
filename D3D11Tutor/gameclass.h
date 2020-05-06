#pragma once


#include"inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "Object/object.h"
#include "Bar.h"
#include <vector>

class GameClass {
public:
	GameClass();
	bool Initialize(int, int);
	void Shutdown();
	bool Frame(InputClass&);
	void SetCamera(CameraClass*);

	bool AddObject(Object*);

	std::vector < Object*> GetObjectHolder();

private:
	CameraClass* m_Camera;
	Bar m_bar;
	std::vector<Object*> m_ObjectHolder;
	int m_objIndex;
	const int HolderSize = 256;
};
