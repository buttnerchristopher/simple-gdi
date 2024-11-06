//
// Reference material form Tricks of the Windows Game Programming Gurus.
// ISBN: 0-672-31361-8
// 
// The primary purpose of defining WIN32_LEAN_AND_MEAN is to make the compilation process faster by excluding unnecessary APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
#define WIN32_LEAN_AND_MEAN

#include <windows.h>	// main windows include
#include <windowsx.h>	// has same macros in it.
#include <stdio.h>
#include <math.h>

#define WINDOW_CLASS_NAME	L"WINCLASS1"

void PaintSimpleStuff(_In_ HWND hwnd) {
	HDC gdc;
	
	if (!(gdc = GetDC(hwnd)))
	{
		return;
	}

	// Create a Black pen.
	
	HPEN black_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN old_pen = (HPEN)SelectObject(gdc, black_pen);

	// Draw a simple line.
	MoveToEx(gdc, 50,100, NULL);
	LineTo(gdc, 350, 100);

	// Draw a circle (actually an ellipse).
	Ellipse(gdc, 50, 200, 350, 300);

	// Draw a rectangle.
	Rectangle(gdc, 100, 50, 300, 350);

	SelectObject(gdc, old_pen);

	// Delete the pen
	DeleteObject(black_pen);

	ReleaseDC(hwnd, gdc);
}

LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wparam, _In_ LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// do initialisation stuff here

		return 0;
	} break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(hwnd, &ps);
		
		PaintSimpleStuff(hwnd);

		EndPaint(hwnd, &ps);

		// Not sure about this, but take from my reference book.
		GetClientRect(hwnd, &rect);
		ValidateRect(hwnd, &rect);

		return 0;
	} break;
	case WM_DESTROY:
	{
		// Clean up.
		PostQuitMessage(0);	// Kill the app. Sends a WM_QUIT message.
		return 0;
	} break;

	default: break;
	} // end of the switch

	// Anything we didn't take care of, send through to the default handler.
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Do stuff in here.
	WNDCLASSEX	winclass;
	HWND		hwnd;
	MSG			msg;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;	// Redraw entire window on horizontal/vertical movement/adjustment
																		// Send double clicks to the window and create unique device context per window.
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	winclass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClassEx(&winclass)) {
		return 0;
	}

	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 400, 400, NULL, NULL, hInstance, NULL)))
	{
		return 0;
	}

	// Event loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate accelerator keys
		TranslateMessage(&msg);

		// send the message to WindowProc
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

