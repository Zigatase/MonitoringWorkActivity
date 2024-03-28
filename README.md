# MonitoringWorkActivity
Client-Server application for monitoring work activity

Explanation
Simple application to show current work activity of all employers in organisation

Example applications 
https://www.teramind.co/solutions/employee-monitoring-software
https://veriato.com/product/

Client (windows) - c/c++
Silent launches on user logon and work in background
Communicates with server at any protocol 

Server - desktop or web interface - any language 

List all connected clients - domain/machine/ip/user
Show client’s last active time
Ability to get screenshot from client’s desktop 

In response send link to github.com project page, which contains all Visual Studio solution files with full source code and dependencies if any.

# Введение

Мониторинг рабочей активности может быть полезен для различных целей, таких как повышение производительности, обеспечение соблюдения политик и обнаружение несанкционированной деятельности.

В начале планирования у меня было несколько идей

	#1 Сделать клиент-сервер на C/C++, для веб интерфейса использовать Wt

	#2 Сделать клиент-сервер C/C++, Python, клиент и сервер были бы на C/C++, а отдельно я бы сделал интерфейс который бы работал через БД

	#3 Сделать клиен-сервер на Python/C++, где сервер и веб интерфейс на py


# Архитектура приложения

Приложение будет состоять из двух компонентов:

    Клиент: Собирает данные о рабочей активности и отправляет их на сервер.
    Сервер: Получает данные от клиентов, хранит их и предоставляет интерфейс для просмотра данных.

# Реализация клиента

Клиентское приложение будет использовать API Windows для сбора данных о рабочей активности, таких как:

    Использование приложений
    Активность в Интернете (Можно было бы добавить)
    Ввод с клавиатуры и мыши (Можно было бы добавить)

Собранные данные будут затем отправляться на сервер с использованием сокетов или другого механизма сетевой связи.

# Запуск
	1) Запускаем Python Сервер
	2) Подключаем клиентов

C/C++ -> 17
Python -> 12

#include <windows.h>
#include <fstream>

void DisplayImageFromByteArray(const std::vector<BYTE>& imageData)
{
    // Создать окно
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ImageWindow";
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, "ImageWindow", "Image", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    // Создать контекст рисования
    HDC hdc = GetDC(hwnd);

    // Создать битмап из массива байтов
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 0;
    bmi.bmiHeader.biHeight = 0;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    void* pBits;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);

    // Скопировать данные изображения в битмап
    memcpy(pBits, imageData.data(), imageData.size());

    // Отобразить битмап в окне
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, SRCCOPY);

    // Освободить ресурсы
    DeleteObject(hBitmap);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}
