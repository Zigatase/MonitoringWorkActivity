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

// Функция для получения снимка экрана и сохранения его в виде массива байтов
std::vector<unsigned char> captureScreenToBytes() {
    HDC hScreen = GetDC(NULL);
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Создание массива для хранения снимка экрана (RGB формат)
    std::vector<unsigned char> pixels;
    pixels.resize(nScreenWidth * nScreenHeight * 3);

    // Получение снимка экрана
    if (!BitBlt(hScreen, 0, 0, nScreenWidth, nScreenHeight, hScreen, 0, 0, SRCCOPY)) {
        ReleaseDC(NULL, hScreen);
        return pixels;
    }

    // Копирование пикселей изображения в массив
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = nScreenWidth;
    bi.biHeight = nScreenHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    GetDIBits(hScreen, hScreen, 0, nScreenHeight, &pixels[0], (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    ReleaseDC(NULL, hScreen);
    
    return pixels;
}

// Функция для сохранения массива байтов изображения в файл PNG
void saveBytesAsPNG(const std::vector<unsigned char>& imageBytes, const std::string& filename, int width, int height) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) {
        return;
    }

    // Заголовок PNG файла
    const unsigned char header[] = {
        0x89, 'P', 'N', 'G', 'r', 'n', 0x1A, 'n',
        0, 0, 0, 13, 'I', 'H', 'D', 'R',
        static_cast<unsigned char>(width >> 24),
        static_cast<unsigned char>(width >> 16),
        static_cast<unsigned char>(width >> 8),
        static_cast<unsigned char>(width),
        static_cast<unsigned char>(height >> 24),
        static_cast<unsigned char>(height >> 16),
        static_cast<unsigned char>(height >> 8),
        static_cast<unsigned char>(height),
        8, 6, 0, 0, 0
    };

    file.write((char*)header, sizeof(header));
    file.write(reinterpret_cast<const char*>(&imageBytes[0]), imageBytes.size());

    // Завершение файла PNG
    const unsigned char footer[] = {
        0, 0, 0, 0, 'I', 'E', 'N', 'D', 0xAE, 0x42, 0x60, 0x82
    };
    file.write((char*)footer, sizeof(footer));

    file.close();
}

int main() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Получаем снимок экрана и сохраняем его в виде массива байтов
    std::vector<unsigned char> screenshotBytes = captureScreenToBytes();

    // Сохраняем полученный снимок в файле screenshot.png
    saveBytesAsPNG(screenshotBytes, "screenshot.png", screenWidth, screenHeight);

    return 0;
}
