#pragma once


#include"inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include <vector>

class GameClass {
public:
	GameClass();
	bool Initialize(int, int);
	void Shutdown();
	bool Frame(const InputClass&);
	void SetCamera(CameraClass*);

	std::vector < Object*> GetObjectHolder();

private:
	CameraClass* m_Camera;
	Square square;
	std::vector<Object*> m_ObjectHolder;
	int m_objIndex;
	const int HolderSize = 256;
};
