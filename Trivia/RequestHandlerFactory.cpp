#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) :
    m_database(database), m_loginManager(LoginManager(database)), m_statisticsManager(database), m_roomManager(), m_gameManager(GameManager(database)){
}

/*
Creates a new login request handler and manages it the login or the singup
input:
*	void
* output:
*  LoginRequestHandler - the login request handler
*/
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() {
    return new LoginRequestHandler(*this);
}

/*
Gets the login manager
input:
*	void
output:
* LoginManager - the login manager
*/
LoginManager& RequestHandlerFactory::getLoginManager() {
    return this->m_loginManager;
}

/*
Creates a new menu request handler and manages it the menu
input:
*	LoggedUser user - the user that is logged in
output:
*   new menu 
*/
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) {
    return new MenuRequestHandler(*this, user);
}

/*
Gets the m_statisticsManager
input:
*	void
output:
*   StatisticsManager - the statistics manager
*/
StatisticsManager& RequestHandlerFactory::getStatisticsManager() {
	return this->m_statisticsManager;
}

/*
Gets the m_roomManager
input:
*	void
output:
*   RoomManager - the room manager
*/
RoomManager& RequestHandlerFactory::getRoomManager() {
    return this->m_roomManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, int roomId)
{
    return new RoomAdminRequestHandler(roomId, user, *this, this->m_roomManager);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, int roomId)
{
    return new RoomMemberRequestHandler(roomId, user, *this, this->m_roomManager);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, int gameId,Room room)
{
    
    //this->m_roomManager.deleteRoom(room.getId());
    return new GameRequestHandler(this->m_database, gameId ,user, *this, this->m_gameManager);
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return this->m_gameManager;
}

