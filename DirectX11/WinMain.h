#ifndef __WinMain_h
#define __WinMain_h
#include <windows.h>
#include <timeapi.h>

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//��Ϣѭ��
int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

//��ʼ��Windows
bool InitWindow(HINSTANCE hInstance, int width, int height);

#endif // __WinMain_h
