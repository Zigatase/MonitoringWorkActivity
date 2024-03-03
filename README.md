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

Мониторинг рабочей активности может быть полезен для различных целей, таких как повышение производительности, обеспечение соблюдения политик и обнаружение несанкционированной деятельности. В этой статье мы рассмотрим, как создать простое клиент-серверное приложение для мониторинга рабочей активности с использованием C/C++.

# Архитектура приложения

Приложение будет состоять из двух компонентов:

    Клиент: Собирает данные о рабочей активности и отправляет их на сервер.
    Сервер: Получает данные от клиентов, хранит их и предоставляет интерфейс для просмотра и анализа данных.

# Реализация клиента

Клиентское приложение будет использовать API Windows для сбора данных о рабочей активности, таких как:

    Использование приложений
    Активность в Интернете
    Ввод с клавиатуры и мыши

Собранные данные будут затем отправляться на сервер с использованием сокетов или другого механизма сетевой связи.