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
// キー最大数
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
   // インプットの生成
   HRESULT CreateInput(void);
   // キーデバイスの生成
   HRESULT CreateKey(void);
   // キーフォーマットのセット
   HRESULT SetKeyFormat(void);
   // キーの協調レベルのセット
   HRESULT SetKeyCooperative(HWND);

   // 参照結果
   HRESULT result;
   // インプット
   LPDIRECTINPUT8 input;
   // インプットデバイス
   LPDIRECTINPUTDEVICE8 key;
   // キー情報
   BYTE keys[KEY_MAX];
   // 前のキー情報
   BYTE olds[KEY_MAX];

   void UpdateProxy();
};

	class InputProxy {
	public:
		InputProxy();
	};

#endif