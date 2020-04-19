#pragma once


#include"inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"

class GameClass {
public:
	GameClass();
	bool Initialize(int, int);
	void Shutdown();
	bool Frame(const InputClass*);
	void SetCamera(CameraClass*);

private:
	CameraClass* m_Camera;
};
