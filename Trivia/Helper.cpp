#include "Helper.h"

/*
Send message to the socket of the client
input:
	SOCKET socket - the socket of the client
	string message - the message to send
output:
	void
*/
void Helper::sendToSocket(const SOCKET socket, const std::string message) {
	if (send(socket, message.c_str(), message.size(), 0) == INVALID_SOCKET){
		throw std::exception("Error while sending message to client");
	}
}
