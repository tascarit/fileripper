#include <windows.h>
#include <commctrl.h>
#include <stdint.h>
#include <stdio.h>

#define QWORD uint64_t
#define BYTE unsigned char

#define DUMP(var) #var

HWND listView;
HFONT hFont;
int index = 0;

void ToUpper(char* buffer, int size){
	for (int i = 0; i < size; i++){
		if (buffer[i] > 96 && buffer[i] < 123)
			buffer[i] -= 32;
	}
}

void ProcessToAscii(char* buffer, int size){
	for (int i = 0; i < size; i++){
		if (buffer[i] < 33 || buffer[i] > 126)
			buffer[i] = '.';
	}
}

void AddMenu(HWND hWnd){
	HMENU hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, 0, "Open file");
	SetMenu(hWnd, hMenu);
}

BOOL AddWidgets(HWND hWnd, HINSTANCE hParentInst){
	//INITCOMMONCONTROLSEX ic;
	
	//ic.dwICC = ICC_LISTVIEW_CLASSES;
	//InitCommonControlsEx(&ic);

	listView = CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | LVS_REPORT, 0, 0, 1000, 500, hWnd, NULL, hParentInst, NULL);
	
	SendMessageA(listView, WM_SETFONT, (WPARAM) hFont, TRUE);

	LVCOLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVIF_IMAGE;

	for (int i = 0; i < 3; i++){
		lvc.iSubItem = i;
		lvc.fmt = LVCFMT_CENTER;

		switch(i){
			case 0: 
				lvc.cx = 100;
				lvc.pszText = "Address";
				break;
			case 1: 
				lvc.cx = 600;
				lvc.pszText = "HEX";
				break;
			case 2:
				lvc.cx = 250;
				lvc.pszText = "ASCII";
				break;
		}

		if (ListView_InsertColumn(listView, i, &lvc) == -1){
			MessageBoxA(NULL, "Failed to insert column into ListView", "Error", 0);
			return FALSE;
		}
	}

	return TRUE;
}

void WriteAddress(QWORD address){
	LVITEM lvi;
	char buffer[19];
	
	sprintf(buffer, "0x%llx", address);

	lvi.iSubItem = 0;
	lvi.iItem = index;
	lvi.iImage = index;
	lvi.pszText = buffer;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	ListView_InsertItem(listView, &lvi);
}

void WriteHEX(BYTE bytes[16]){
	LVITEM lvi;
	char buffer[48];

	sprintf(buffer, "%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx", 
			bytes[0], bytes[1], bytes[2],
			bytes[3], bytes[4], bytes[5], 
			bytes[6], bytes[7], bytes[8],
			bytes[9], bytes[10], bytes[11],
			bytes[12], bytes[13], bytes[14],
			bytes[15]);
	ToUpper(buffer, 48);

	lvi.iSubItem = 1;
	lvi.iItem = index;
	lvi.iImage = index;
	lvi.pszText = buffer;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	ListView_SetItem(listView, &lvi);
}

void WriteASCII(BYTE bytes[16]){
	LVITEM lvi;
	char buffer[17];
	buffer[16] = '\0';

	memcpy(buffer, bytes, 16);
	ProcessToAscii(buffer, 16);

	lvi.iSubItem = 2;
	lvi.iItem = index;
	lvi.iImage = index;
	lvi.pszText = buffer;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	ListView_SetItem(listView, &lvi);
	index++;
}
