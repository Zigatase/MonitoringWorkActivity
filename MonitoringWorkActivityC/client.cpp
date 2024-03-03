#include "client.h"

#define INFO_BUFFER_SIZE 32767


bool GetPcData()
{
    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    // Get and display the name of the computer.
    if (!GetComputerName(infoBuf, &bufCharCount))
        return false;

    std::wcout << "[Domain]: " << infoBuf << std::endl;

    // Get and display the user name.
    if (!GetUserName(infoBuf, &bufCharCount))
        return false;

    std::wcout << "[User]: " << infoBuf << std::endl;

    return true;
}


int Client()
{
    const char ip_address[] = "127.0.0.1";
    const int port = 77777;

    // Initialize WinSock
    WSAData data{};
    WORD ver = MAKEWORD(2, 2);
    int ws_result = WSAStartup(ver, &data);
    if (ws_result != 0)
    {
        std::cerr << "Can't start Winsock, Err #" << ws_result << std::endl;
        return -1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Fill in a hint structure
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &hint.sin_addr);

    // Connect to server
    int conn_result = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (conn_result == SOCKET_ERROR)
    {
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // While loop to send and receive data
    char buf[4096];

    // --- Getting PC Data ---
    // Сделать сбор данных пользователя domain/machine/ip/user

    // sending    // CHAR
    // send(sock, pc.c_str(), pc.size() + 1, 0);
    std::string message = "Test Connect";
    send(sock, message.c_str(), message.size() + 1, 0);


    // --- Waiting for commands from the server ---
    while (true)
    {
        ZeroMemory(buf, 4095);

        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }

        std::string command = std::string(buf, 0, bytesReceived);

        // Сommand processing
        if (command == "-Connect")
        {
            std::cout << "[Command from the server]: " << command << std::endl;;
        }
        else
        {
            std::cout << "[Unknown command from the server]: " << command << std::endl;
        }
    }

    // Close socket and cleanup winsock
    closesocket(sock);
    WSACleanup();

    system("pause");
    return 0;
}