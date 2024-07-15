#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include "IDatabase.h"


class StatisticsManager {
protected:
	IDatabase* m_database;

public:
	StatisticsManager(IDatabase* database);
	std::vector<Score> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
};

