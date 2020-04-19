////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass() : result(S_OK), input(nullptr), key(nullptr)
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize(HWND hwnd)
{
	memset(&keys, 0, sizeof(keys));
	memset(&olds, 0, sizeof(olds));

	CreateInput();
	CreateKey();
	SetKeyFormat();
	SetKeyCooperative(hwnd);

	return;
}

void InputClass::Frame() {
   //�L�[�����擾
   key->GetDeviceState(sizeof(keys), &keys);
}


HRESULT InputClass::CreateInput(void)
{
   result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&input), NULL);

   return result;
}
// �L�[�f�o�C�X�̐���
HRESULT InputClass::CreateKey(void)
{
   result = input->CreateDevice(GUID_SysKeyboard, &key, NULL);

   return result;
}
// �L�[�t�H�[�}�b�g�̃Z�b�g
HRESULT InputClass::SetKeyFormat(void)
{
   result = key->SetDataFormat(&c_dfDIKeyboard);

   return result;
}
// �L�[�̋������x���̃Z�b�g
HRESULT InputClass::SetKeyCooperative(HWND hwnd)
{
   result = key->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

   //���̓f�o�C�X�ւ̃A�N�Z�X�������擾
   key->Acquire();

   return result;
}
// �g���K�[�̓���
bool InputClass::IsKeyTrigger(UINT index)
{
   //�`�F�b�N�t���O
   bool flag = false;

   if ((keys[index] & 0x80) && !(olds[index] & 0x80))
   {
      flag = true;
   }
   olds[index] = keys[index];

   return flag;
}

// �L�[����
bool InputClass::IsKeyDown(UINT index)
{
   //�`�F�b�N�t���O
   bool flag = false;

   if (keys[index] & 0x80)
   {
      flag = true;
   }
   olds[index] = keys[index];

   return flag;
}


XMFLOAT2 InputClass::GetAxis(bool isTrigger)
{
    XMFLOAT2 dir;
    dir.x = 0.0f;
    dir.y = 0.0f;
    if (isTrigger) {
        if (IsKeyTrigger(DIK_A)) {
            dir.x = 1.0f;
        }
        else if (IsKeyTrigger(DIK_D)) {
            dir.x = -1.0f;
        }
        if (IsKeyTrigger(DIK_S)) {
            dir.y = 1.0f;
        }
        else if (IsKeyTrigger(DIK_W)) {
            dir.y = -1.0f;
        }
    }
    else{
        if (IsKeyDown(DIK_A)) {
            dir.x = 1.0f;
        }
        else if (IsKeyDown(DIK_D)) {
            dir.x = -1.0f;
        }
        if (IsKeyDown(DIK_S)) {
            dir.y = 1.0f;
        }
        else if (IsKeyDown(DIK_W)) {
            dir.y = -1.0f;
        }
    }
    return dir;
}


