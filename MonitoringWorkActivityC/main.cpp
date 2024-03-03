#include "client.h"

struct PCDataRequest
{

};

int main()
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

    // Getting PC Data
    PCDataRequest pc_data;

    // sending    // CHAR
    // send(sock, pc.c_str(), pc.size() + 1, 0);
    std::string message = "Test Connect";
    send(sock, message.c_str(), message.size() + 1, 0);

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

        //
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