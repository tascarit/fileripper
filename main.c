#include <Windows.h>
#include <stdio.h>
#include <commdlg.h>

#include "utility.h"

#define CLASS_NAME "File Ripper Class"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow){
	MSG msg = {0};
	WNDCLASS wnd = {0};
	hFont = CreateFontA(18, 11, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Consolas");
	
	if (!hFont){
		MessageBoxA(NULL, "Failed to load font", "Error", 0);
	}

	wnd.hInstance = hInst;
	wnd.lpszClassName = CLASS_NAME;
	wnd.lpfnWndProc = WndProc;

	if (!RegisterClass(&wnd)){
		MessageBoxA(NULL, "Failed to register class.", "Critical error", 0);
		return 1;
	}

	HWND hWnd = CreateWindow(CLASS_NAME, "File Ripper", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 600, NULL, NULL, hInst, NULL);
	
	if (!hWnd){
		MessageBoxA(NULL, "Failed to create window.", "Critical error", 0);
		return 1;
	}
	
	AddMenu(hWnd);
	AddWidgets(hWnd, hInst);
	SendMessageA(hWnd, WM_SETFONT, (WPARAM) hFont, TRUE);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&msg, hWnd, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
	switch (msg){
		case WM_COMMAND:
			switch(wp){
				case 0:
					OPENFILENAME ofn = {0};
					char filePath[256];

					ofn.hwndOwner = NULL;
					ofn.lpstrFile = filePath;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(filePath);
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					ofn.nMaxFileTitle = 0;
					ofn.nFilterIndex = 1;
					ofn.lStructSize = sizeof(ofn);

					if(!GetOpenFileName(&ofn)){
						MessageBoxA(NULL, "Error getting file path.", "Error", 0);
					} else {
						printf("%s\n", ofn.lpstrFile);
						FILE* fp = fopen(ofn.lpstrFile, "rb");
						BYTE byte[16];

						while (fread(byte, sizeof(BYTE), 16, fp) != 0){
							WriteAddress(ftell(fp));
							WriteHEX(byte);
							WriteASCII(byte);

						}

						fclose(fp);
					}
			return 0;
			}
		case WM_CLOSE:
			DestroyWindow(hWnd);
			ExitProcess(0);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, msg, wp, lp);
}
