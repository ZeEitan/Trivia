#include "GameManager.h"

GameManager::GameManager(IDatabase* database)
{
	this->m_database = database;
}

Game GameManager::createGame(Room room, LoggedUser host) {
	std::cout << "in createGame" << std::endl;
	Game game(room, this->m_database, host);
	this->m_games.push_back(game);
	return game;
}

void GameManager::deleteGame(int gameId) {
	int dex = -1;
	for (int i = 0; i < this->m_games.size(); i++) {
		if (this->m_games[i].getGameId() == gameId) {
			dex = i;
		}
	}
	for (int i = dex; i < this->m_games.size() - 1; ++i) {
		this->m_games[i] = this->m_games[i + 1];
	}
	this->m_games.pop_back();

}

Game GameManager::getGame(int gameId){
	for (int i = 0; i < this->m_games.size(); i++) {
		if (this->m_games[i].getGameId() == gameId) {
			return this->m_games[i];
		}
	}
	
}

bool GameManager::checkIfGameExists(int gameId)
{
	for (int i = 0; i < this->m_games.size(); i++) {
		if (this->m_games[i].getGameId() == gameId) {
			return true;
		}
	}
	return false;
}

void GameManager::setGame(Game game, int gameId) {
	for (int i = 0; i < this->m_games.size(); i++) {
		if (this->m_games[i].getGameId() == gameId) {
			this->m_games[i] = game;
		}
	}
}
