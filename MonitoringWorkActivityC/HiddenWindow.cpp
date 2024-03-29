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



Чат с Gemini
Фото профиля
Создай невидимый рабочий стол с помощь C++ и WinApi не используя сторонние библиотеки, а так же сделай так что бы программа сделала скриншот невидимого рабочего стола со всеми открытыми окнами
Показать черновики













Создание невидимого рабочего стола и скриншот с помощью C++ и WinApi

Создание невидимого рабочего стола:

C++
#include <windows.h>

int main() {
  // Создаем новый виртуальный рабочий стол
  HDESK hDesktop = CreateDesktop("Невидимый рабочий стол", NULL, NULL, 0, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS, NULL);
  if (hDesktop == NULL) {
    return 1;
  }

  // Переключаемся на новый рабочий стол
  BOOL bSwitch = SwitchDesktop(hDesktop);
  if (!bSwitch) {
    return 1;
  }

  // Выводим сообщение на невидимый рабочий стол
  HWND hWnd = GetConsoleWindow();
  HDC hdc = GetDC(hWnd);
  TextOut(hdc, 0, 0, "Привет с невидимого рабочего стола!", 32);
  ReleaseDC(hWnd, hdc);

  // Скриншот невидимого рабочего стола
  HDC hdcScreenshot = CreateDC(NULL, "DISPLAY", NULL, NULL);
  HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreenshot, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
  HGDIOBJ hOldBitmap = SelectObject(hdcScreenshot, hBitmap);
  BitBlt(hdcScreenshot, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), hdc, 0, 0, SRCCOPY);
  SelectObject(hdcScreenshot, hOldBitmap);

  // Сохранение скриншота
  BITMAP bmpInfo;
  GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
  BITMAPFILEHEADER bmfh;
  ZeroMemory(&bmfh, sizeof(BITMAPFILEHEADER));
  bmfh.bfType = 0x4D42; // "BM"
  bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmWidthBytes * bmpInfo.bmHeight;
  bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  BITMAPINFO bi;
  ZeroMemory(&bi, sizeof(BITMAPINFO));
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = bmpInfo.bmWidth;
  bi.bmiHeader.biHeight = bmpInfo.bmHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = bmpInfo.bmBitsPixel;
  bi.bmiHeader.biCompression = BI_RGB;

  HANDLE hFile = CreateFile("скриншот.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD dwWritten;
    WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, bmpInfo.bmBits, bmpInfo.bmWidthBytes * bmpInfo.bmHeight, &dwWritten, NULL);
    CloseHandle(hFile);
  }

  // Очистка
  DeleteObject(hBitmap);
  DeleteDC(hdcScreenshot);

  // Переключаемся обратно на основной рабочий стол
  SwitchDesktop(GetThreadDesktop(GetCurrentThreadId()));

  // Закрываем виртуальный рабочий стол
  CloseDesktop(hDesktop);

  return 0;
}