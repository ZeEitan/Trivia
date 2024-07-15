#pragma once
#include <iostream>
#include <string>
#include "SqliteDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameManager.h"
#include "GameRequestHandler.h"

class GameRequestHandler;
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory {
private:

	GameManager m_gameManager;
	LoginManager m_loginManager;
	IDatabase* m_database;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;

public:
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* createLoginRequestHandler();

	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	StatisticsManager& getStatisticsManager();

	LoginManager& getLoginManager();
	RoomManager& getRoomManager();

	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, int roomId);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, int roomId);
	GameRequestHandler* createGameRequestHandler(LoggedUser user, int gameId, Room room);

	GameManager& getGameManager();
};

