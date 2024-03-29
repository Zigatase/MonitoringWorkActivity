#include <Windows.h>
#include <stdio.h>

//Create a new desktop or open an existing one
HDESK CreateHiddenDesktop(CHAR *desktop_name)
{
    CHAR explorer_path[MAX_PATH];
    HDESK hidden_desktop = NULL, original_desktop;
    STARTUPINFOA startup_info = {0};
    PROCESS_INFORMATION process_info = {0};

    ExpandEnvironmentStringsA("%windir%\\explorer.exe", explorer_path, MAX_PATH-1);

    hidden_desktop = OpenDesktopA(desktop_name, NULL, FALSE, GENERIC_ALL);
    if(!hidden_desktop)
    {
        hidden_desktop = CreateDesktopA(desktop_name, NULL, NULL, 0, GENERIC_ALL, NULL);
        if(hidden_desktop)
        {
            original_desktop = GetThreadDesktop(GetCurrentThreadId());

            if(SetThreadDesktop(hidden_desktop))
            {
                startup_info.cb = sizeof(startup_info);
                startup_info.lpDesktop = desktop_name;

                //We need to create an explorer.exe in the context of the new desktop for start menu, etc
                CreateProcessA(explorer_path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info);

                SetThreadDesktop(original_desktop);
            }
        }
    }
    return hidden_desktop;
}

void main()
{
    HDESK original_desktop, hidden_desktop;
    
    hidden_desktop = CreateHiddenDesktop("hVNC");
    original_desktop = GetThreadDesktop(GetCurrentThreadId());

    printf("Entering hidden desktop\n");

    //Need to switch thread into context of new desktop to register hotkeys
    SetThreadDesktop(hidden_desktop);
    SwitchDesktop(hidden_desktop);

    if(RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x45))
    {
        MSG msg = {0};
        while(GetMessage(&msg, NULL, 0, 0) != 0)
        {
            if(msg.message == WM_HOTKEY)
            {
                printf("Exiting hidden desktop\n");
                SwitchDesktop(original_desktop);
                break;
            }
        }
    }

    CloseHandle(hidden_desktop);
    getchar();
}


bool TakeScreenshot(HWND hDesktop, const wstring& filename) {
    // Получение размеров рабочего стола
    RECT rect;
    GetWindowRect(hDesktop, &rect);

    // Создание контекста устройства для рабочего стола
    HDC hdcDesktop = GetDCEx(hDesktop, NULL, DCX_WINDOW);
    if (hdcDesktop == NULL) {
        cout << "Ошибка получения контекста устройства!" << endl;
        return false;
    }

    // Создание совместимого контекста устройства
    HDC hdcMem = CreateCompatibleDC(hdcDesktop);
    if (hdcMem == NULL) {
        cout << "Ошибка создания совместимого контекста устройства!" << endl;
        return false;
    }

    // Создание битмапа
    HBITMAP hBitmap = CreateDIBSection(hdcDesktop, (BITMAPINFO*)&BITMAPINFO{ sizeof(BITMAPINFOHEADER), rect.right - rect.left, rect.bottom - rect.top, 1, 32, BI_RGB, 0 }, DIB_RGB_COLORS, NULL, NULL, 0);
    if (hBitmap == NULL) {
        cout << "Ошибка создания битмапа!" << endl;
        return false;
    }

    // Выбор битмапа в контекст устройства
    SelectObject(hdcMem, hBitmap);

    // Копирование изображения рабочего стола в битмап
    BitBlt(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcDesktop, 0, 0, SRCCOPY);

    // Сохранение битмапа в JPEG-файл
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID encoderClsid;
    GetEncoderClsid(L"image/jpeg", &encoderClsid);

    Gdiplus::Bitmap bitmap(hBitmap);
    bitmap.Save(filename.c_str(), &encoderClsid, NULL);

    GdiplusShutdown(gdiplusToken);

    // Освобождение ресурсов
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    DeleteDC(hdcDesktop);

    return true;
}
