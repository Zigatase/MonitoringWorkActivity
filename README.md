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




Чат с Gemini
Фото профиля
Сделай функцию которая может сделать скриншот рабочего стала и сохранить его в виде массива байт используя C++, но не используя сторонние библиотеки 
Показать черновики




















Функция для создания скриншота рабочего стола в C++

Без использования сторонних библиотек:

C++
#include <windows.h>
#include <vector>

using namespace std;

// Функция для получения HDC рабочего стола
HDC GetDesktopHDC() {
  return GetDC(NULL);
}

// Функция для создания битмапа с экрана
HBITMAP CreateBitmapFromScreen(HDC hdc, int width, int height) {
  // Создаем совместимый DC
  HDC memdc = CreateCompatibleDC(hdc);

  // Создаем битмап
  HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height);

  // Выбираем битмап в DC
  SelectObject(memdc, hbitmap);

  // Копируем изображение экрана в битмап
  BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

  // Освобождаем DC
  DeleteDC(memdc);

  return hbitmap;
}

// Функция для получения размера экрана
void GetScreenSize(int& width, int& height) {
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);
}

// Функция для преобразования HBITMAP в массив байт
vector<BYTE> ConvertBitmapToBytes(HBITMAP hbitmap) {
  BITMAP bmp;
  GetObject(hbitmap, sizeof(bmp), &bmp);

  // Вычисляем размер изображения
  int size = bmp.bmWidthBytes * bmp.bmHeight;

  // Выделяем память для изображения
  BYTE* pBits = new BYTE[size];

  // Получаем данные изображения
  GetBitmapBits(hbitmap, size, pBits);

  // Освобождаем битмап
  DeleteObject(hbitmap);

  // Преобразуем данные в вектор байт
  vector<BYTE> bytes(pBits, pBits + size);

  // Удаляем выделенную память
  delete[] pBits;

  return bytes;
}

// Функция для создания скриншота и сохранения его в виде массива байт
vector<BYTE> GetScreenshotBytes() {
  int width, height;
  GetScreenSize(width, height);

  HDC hdc = GetDesktopHDC();
  HBITMAP hbitmap = CreateBitmapFromScreen(hdc, width, height);

  vector<BYTE> bytes = ConvertBitmapToBytes(hbitmap);

  return bytes;
}