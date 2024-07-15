#pragma once
#include "SqliteDatabase.h"
#include "Game.h"
class GameManager {
protected:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	GameManager(IDatabase* database);
	Game createGame(Room room, LoggedUser host);
	void deleteGame(int gameId);
	Game getGame(int gameId);
	bool checkIfGameExists(int gameId);
	void setGame(Game game, int gameId);
};

