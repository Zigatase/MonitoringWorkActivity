#ifndef MONITORINGWORKACTIVITYC_CLIENT_H_
#define MONITORINGWORKACTIVITYC_CLIENT_H_

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// domain/machine/ip/user
struct GET_PC_DATA_REQUSET
{
	std::string domain;
	std::string machine;
	std::string ip;
	std::string user;
};


bool GetPcData();

int Client();

#endif // !MONITORINGWORKACTIVITYC_CLIENT_H_
