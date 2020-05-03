////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
#include<string>
// �L�[�ő吔
#define KEY_MAX 256

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

////
// https://qiita.com/okmonn/items/e17e7998a669bf3fee08
////

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize(HWND);
    void Frame();
   bool IsKeyTrigger(UINT index);
   bool IsKeyDown(UINT index);
   XMFLOAT2 GetAxis(bool isTrigger);

   //InputProxy* CreateCurrentProxy(std::string label);
   //InputProxy* CreateTopPriorityProxy(std::string label);
   //void RemoveProxy(InputProxy*);

private:
   // �C���v�b�g�̐���
   HRESULT CreateInput(void);
   // �L�[�f�o�C�X�̐���
   HRESULT CreateKey(void);
   // �L�[�t�H�[�}�b�g�̃Z�b�g
   HRESULT SetKeyFormat(void);
   // �L�[�̋������x���̃Z�b�g
   HRESULT SetKeyCooperative(HWND);

   // �Q�ƌ���
   HRESULT result;
   // �C���v�b�g
   LPDIRECTINPUT8 input;
   // �C���v�b�g�f�o�C�X
   LPDIRECTINPUTDEVICE8 key;
   // �L�[���
   BYTE keys[KEY_MAX];
   // �O�̃L�[���
   BYTE olds[KEY_MAX];

   void UpdateProxy();
};

	class InputProxy {
	public:
		InputProxy();
	};

#endif