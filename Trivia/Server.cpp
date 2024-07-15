#include "Server.h"

/*
Server constructor
input:
*	database - the database of the server
output:
*	void
*/
Server::Server(IDatabase* database) : m_handlerFactory(RequestHandlerFactory(database)), m_communicator(m_handlerFactory) {
	this->m_database = database;
	this->m_database->open();
}


/*
Starts the server
input: 
*	void
output:
*	void
*/
void Server::run() {
	std::cout << "Server started" << std::endl;
	this->m_communicator.startHandleRequests();
}
