#include "Communicator.h"

/*
Start the communicator server and handle requests from clients
input:
*	Void
ootput:
*	Void
*/
void Communicator::startHandleRequests() {
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	
	std::cout << "Listening on port " << PORT << std::endl;

	while (true) {
		bindAndListen();
	}
}

/*
Bind and listen for new clients
input:
*	Void
output:
*	Void
*/
void Communicator::bindAndListen() {
	SOCKET client_socket = accept(this->m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);
	std::cout << "Client connected." << std::endl;
	std::thread clientThread(&Communicator::handleNewClient, this, client_socket);
	clientThread.detach();
	m_clients[client_socket] = this->m_handlerFactory.createLoginRequestHandler();

}

/*
Handle a new client
input:
*	clientSocket - the socket of the new client
output:
*	Void
*/
void Communicator::handleNewClient(SOCKET clientSocket) {
	while (true) {
		try {
			char m[2048];
			std::string message;

			int bytesReceived = recv(clientSocket, m, 2048, 0);

			if (bytesReceived <= 0) {
				std::cout << "Client has disconnected." << std::endl;
				try {
					this->logoutClient(clientSocket);
				}
				catch (const std::exception& e) {
					std::cout << e.what() << std::endl;
				}
				this->deleteClient(clientSocket);
				return;
			}

			if (bytesReceived < 2048)
				m[bytesReceived] = 0;
			message = m;

			std::cout << "Received: " << message << std::endl;

			// Create request info from the message
			RequestInfo info{
				(int)message[0],
				std::time(nullptr),
				std::vector<unsigned char>(message.begin(), message.end())
			};

			std::cout << "Received: " << message << std::endl << std::endl;
			RequestResult result = m_clients[clientSocket]->RequestHandler(info, clientSocket); // handle the request and get the result of the request and the new handler

			if (result.newHandler != nullptr) { // Switch to the new handler and delete the old one
				delete this->m_clients[clientSocket];
				m_clients[clientSocket] = result.newHandler;
			}
			this->sendToClient(clientSocket, std::string(result.responseBuffer.begin(), result.responseBuffer.end())); // send the response to the client
			
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			this->deleteClient(clientSocket);
			break;
		}
	}
}

/*
Delete client from the list of clients and close the socket
input:
*	clientSocket - the socket of the client to delete
output:
*	Void
*/
void Communicator::deleteClient(SOCKET clientSocket) {
	closesocket(clientSocket);
	this->m_clients.erase(clientSocket);
}

/*
Send a message to a client
input:
*	clientSocket - the socket of the client to send the message to
*   msg - the message to send
output:
*	Void
*/
void Communicator::sendToClient(SOCKET clientSocket, std::string msg) {
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

void Communicator::logoutClient(SOCKET clientSock) {
	IRequestHandler* handler = this->m_clients[clientSock];

	// Create temp request info to check if the request is relevent
	RequestInfo info{
		requestLogoutCode,
		std::time(nullptr),
		std::vector<unsigned char>(),
	};

	// The case if the user is in the main menu
	if (handler->isRequestRelevent(info)) {
		logutFromMainManu(clientSock);
		return;
	}
	// The case if the user is in a room
	info.id = requestLeaveRoomCode;
	if (handler->isRequestRelevent(info)) {
		logoutFromRoom(clientSock);
		return;
	}

	// The case if the user is a room admin
	info.id = requestCloseRoomCode;
	if (handler->isRequestRelevent(info)) {
		logoutFromRoomAdmin(clientSock);
		return;
	}

	info.id = requestLeaveGameCode;
	if (handler->isRequestRelevent(info)) { 
		logoutFromGame(clientSock);
		return;
	}
}

/*
Logout from the conected users if the user is in the main menu
input:
*	clientSock - the socket of the client
output:
*	Void
*/
void Communicator::logutFromMainManu(SOCKET clientSock) {
	RequestInfo info{
		requestLogoutCode,
		std::time(nullptr),
		std::vector<unsigned char>(),
	};

	RequestResult result = this->m_clients[clientSock]->RequestHandler(info, clientSock);
	this->sendToClient(clientSock, std::string(result.responseBuffer.begin(), result.responseBuffer.end()));

	this->m_clients[clientSock] = result.newHandler;
}

/*
Logout from the conected users if the user is in a room
input:
*	clientSock - the socket of the client
output:
*	Void
*/
void Communicator::logoutFromRoom(SOCKET clientSock) {
	RequestInfo info{
		requestLeaveRoomCode,
		std::time(nullptr),
		std::vector<unsigned char>(),
	};

	RequestResult result = this->m_clients[clientSock]->RequestHandler(info, clientSock);
	this->sendToClient(clientSock, std::string(result.responseBuffer.begin(), result.responseBuffer.end()));

	if (result.newHandler == nullptr) {
		this->deleteClient(clientSock); // delete the client if the new handler is null
		return;
	}

	this->m_clients[clientSock] = result.newHandler;

	info.id = requestLogoutCode;
	result = this->m_clients[clientSock]->RequestHandler(info, clientSock);

	this->sendToClient(clientSock, std::string(result.responseBuffer.begin(), result.responseBuffer.end()));
}

/*
Logout from the conected users if the user is an romm admin
input:
*	clientSock - the socket of the client
output:
*	Void
*/
void Communicator::logoutFromRoomAdmin(SOCKET clientSock) {
	RequestInfo info{
		requestCloseRoomCode,
		std::time(nullptr),
		std::vector<unsigned char>(),
	};

	RequestResult result = this->m_clients[clientSock]->RequestHandler(info, clientSock);

	this->sendToClient(clientSock, std::string(result.responseBuffer.begin(), result.responseBuffer.end()));
	this->m_clients[clientSock] = result.newHandler;

	logutFromMainManu(clientSock);
}

void Communicator::logoutFromGame(SOCKET clientSock) {
	RequestInfo info{
	requestLeaveGameCode, 
	std::time(nullptr),
	std::vector<unsigned char>(),
	};

	RequestResult result = this->m_clients[clientSock]->RequestHandler(info, clientSock); 
	this->sendToClient(clientSock, std::string(result.responseBuffer.begin(), result.responseBuffer.end())); 
	this->m_clients[clientSock] = result.newHandler; 
	logutFromMainManu(clientSock);
}



Communicator::Communicator(RequestHandlerFactory& factory) : m_handlerFactory(factory) {
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}
