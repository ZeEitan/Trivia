#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username, SOCKET socket) {
	this->m_username = username;
	this->_socket = socket;
}

/*
Get the username of the user
input:
	None
output:
	string - the username of the user
*/
std::string LoggedUser::getUsername() const {
	return this->m_username;
}


/*
Get the socket of the user
input:
	None
output:
	SOCKET - the socket of the user
*/
SOCKET LoggedUser::getSocket() const {
	return this->_socket;
}
