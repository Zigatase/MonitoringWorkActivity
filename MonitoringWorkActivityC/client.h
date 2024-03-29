#ifndef MONITORINGWORKACTIVITYC_CLIENT_H_
#define MONITORINGWORKACTIVITYC_CLIENT_H_
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <chrono>
#include <format>

#include <iphlpapi.h>

#include <fstream>

#include <stdio.h>
#include <gdiplus.h>
#include <time.h>
#include <cstdio>
#pragma comment (lib, "gdiplus.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

struct GET_PC_DATA_REQUSET
{
	std::string domain;
	std::string machine;
	std::string ip;
	std::string user;
};

class Client
{
private:
	bool GetDataPc(GET_PC_DATA_REQUSET& pc_data);

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void gdiscreen();

public:

	int ClientStart();
};

#endif // !MONITORINGWORKACTIVITYC_CLIENT_H_