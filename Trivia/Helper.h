#pragma once
#include <iostream>
#include <string>

#include <WinSock2.h>
#include <Windows.h>
#include <exception>

class Helper
{
public:
	static void sendToSocket(const SOCKET socket, const std::string message);
};

