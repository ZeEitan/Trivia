#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <exception>
#include <mutex>
#include <ctime>

#include "IRequestHandler.h"
#include "LoginRequestHandler.h"

#define PORT 8200

class Communicator {
private:
	SOCKET m_serverSocket;
	RequestHandlerFactory& m_handlerFactory;
	std::map <SOCKET, IRequestHandler*> m_clients;
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
	
	void deleteClient(SOCKET clientSocket);
	void sendToClient(SOCKET clientSocket, std::string msg);

	void logoutClient(SOCKET clientSock);
	void logutFromMainManu(SOCKET clientSock);
	void logoutFromRoom(SOCKET clientSock);
	void logoutFromRoomAdmin(SOCKET clientSock);
	void logoutFromGame(SOCKET clientSock);

public:
	Communicator(RequestHandlerFactory& factory);

	void startHandleRequests();
};

