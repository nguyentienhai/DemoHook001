#include <Windows.h>

// các biến toàn cục
HHOOK hHook = NULL; // handle của hook
HMODULE hDLL; // handle của DLL

// định nghĩa con trỏ hàm SetGlobalHookHandle() trong file DLL
typedef VOID (*LOADPROC)(HHOOK hHook);

BOOL InstallHook();

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	// cài đặt hook, exit nếu thất bại
	if(InstallHook() == FALSE)
	{
		MessageBox(0, L"Can not install hook!", L"Error", 0);
		return -1;
	}

	MSG msg;
	BOOL bRet;
	// vòng lặp đón và xử lý các mesage
	while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

// hàm cài đặt hook
BOOL InstallHook()
{
	// load file DLL
	if(LoadLibrary(L"DemoHook001.dll") == NULL)
	{
		MessageBox(0, L"Can not load DLL file!", L"Error", 0);
		return FALSE;
	}

	// lấy handle của file DLL
	HMODULE hDLL = GetModuleHandle(L"DemoHook001");

	// exit nếu lấy handle của file DLL không thành công
	if (hDLL == NULL)
	{
		MessageBox(0, L"Can not get handle of DLL file!", L"Error", 0);
		return FALSE;
	}

	// cài đặt hook, phạm vi toàn cục
	hHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)GetProcAddress(hDLL, "FillKeyboard"), hDLL, 0);

	// exit nếu cài đặt hook không thành công
	if (hHook == NULL)
	{
		MessageBox(0, L"Can not install global hook!", L"Error", 0);
		return FALSE;
	}

	// lấy địa chỉ hàm SetGlobalHookHandle() trong file DLL
	LOADPROC fPtrFcnt;
	fPtrFcnt = (LOADPROC)GetProcAddress(hDLL, "SetGlobalHookHandle");
	if (fPtrFcnt == NULL)
	{
		MessageBox(0, L"Can not get address of function SetGlobalHookHandle!", L"Error", 0);
		return FALSE;
	}

	// ấn định handle của hook vào vùng nhớ dùng chung (giữa DLL và ứng dụng này)
	fPtrFcnt(hHook);

	return TRUE;
}