#include <Windows.h>

// vùng nhớ dùng chung, chứa biến handle của Hook
#pragma data_seg("SHARED_DATA")
HHOOK hGlobalHook = NULL;
#pragma dataseg()

// hàm lọc sự kiện nhấn phím
__declspec(dllexport) LRESULT CALLBACK FillKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
	// nếu sự kiện là nhấn phím và mã phím là Enter
	if((nCode == HC_ACTION) && (wParam == 13))
	{
		MessageBox(0, L"haint sao lai co 2 cai nhi", L"Hello", 0);
		return 1;
	}

	// gọi Filter Function kế tiếp trong chuỗi các Filter Function
	return CallNextHookEx(hGlobalHook, nCode, wParam, lParam);
}

// hàm ấn định biến hGlobalHook tại vùng nhớ dùng chung
__declspec(dllexport) void SetGlobalHookHandle(HHOOK hHook)
{
	hGlobalHook = hHook;
}