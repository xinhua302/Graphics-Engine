#ifndef __WinMain_h
#define __WinMain_h
#include <windows.h>
#include <timeapi.h>

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//消息循环
int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

//初始化Windows
bool InitWindow(HINSTANCE hInstance, int width, int height);

#endif // __WinMain_h
