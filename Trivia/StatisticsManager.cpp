#include "StatisticsManager.h"


StatisticsManager::StatisticsManager(IDatabase* database) :m_database(database)
{
}

std::vector<Score> StatisticsManager::getHighScore() {
	std::vector<Score> vec = this->m_database->getHighScores();
	return vec;
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username) {
	std::vector<std::string> vec = this->m_database->getUserStatistics(username);
    return vec;
}

