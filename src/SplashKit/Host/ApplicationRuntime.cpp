#include "ApplicationRuntime.h"

std::vector<IWindow*> _windows;

ApplicationRuntime::ApplicationRuntime(HINSTANCE instance)
{
    Instance = instance;
    _windows = std::vector<IWindow*>();
}

ApplicationRuntime::~ApplicationRuntime()
{
}

int ApplicationRuntime::Run() 
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
        TranslateMessage(&msg);		
		DispatchMessage(&msg);
	}
    return msg.wParam;
}

void ApplicationRuntime::Enlist(IWindow *window)
{
    _windows.push_back(window);
}

LRESULT CALLBACK ApplicationRuntime::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) 
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            for (int i = 0; i < _windows.size(); i++)
            {
                _windows[i]->HandleMessage(hWnd, message, wParam, lParam);
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}