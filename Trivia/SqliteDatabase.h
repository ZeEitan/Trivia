#pragma once
#include <iostream>
#include "IDatabase.h"
#include "sqlite3.h"
#include <vector>
#include <mutex>
#include <list>


extern std::mutex dataBaseMutex; 

struct GameHistory {
	std::string username;
	int gameId;
	double averageAnswerTime;
	int correctAnswerCount;
	int queue;
};



const int MAX_LENGTH_OF_GAME_HISTORY = 5;


class SqliteDatabase : public IDatabase {


private:
	sqlite3* _db;

public:


	sqlite3* getDB();
	bool open();
	bool close();
	int doesUserExists(std::string username);
	int doesPasswordMatch(std::string username, std::string password);
	int addNewUser(std::string username, std::string password, std::string mail);
	int doesMailExists(std::string mail);
	void clearTable(std::string tableName);
	int addNewUserStatistics(std::string username);

	double getPlayerAverageAnswerTime(std::string username);
	int getNumOfCorrectAnswers(std::string username);
	int getNumOfTotalAnswers(std::string username);
	int getNumOfPlayerGames(std::string username);


	//do not use directly -------------------
	double editPlayerAverageAnswerTime(std::string username, double answerTime);
	int incNumOfCorrectAnswers(std::string username);
	int incNumOfTotalAnswers(std::string username);
	int addAnswerToStatisticsTable(std::string username, bool isCorrect, double answerTime, int gameID);
	//--------addAnswer auto use them--------
	int addAnswer(std::string username, bool isCorrect, double answerTime, int gameID);
	int incNumOfPlayerGames(std::string username);
	

	int getPlayerScore(std::string username);

	int getHowManyGamesAreRegistered(std::string username);
	int shiftGamesInHistory(std::string username, int currentLen);
	int submitGameToGameHistory(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId, int queue);
	int submitGameStatistics(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId);

	int submitAnswer(std::string username, int gameId, int questionId, int isCorrect, double answerTime, std::string answer);

	std::vector<Score> getHighScores();
	std::vector<std::string> getUserStatistics(std::string username);


	std::string getQuestion(int questionId);
	std::vector<std::string> getAnswers(int questionId);

	SqliteDatabase() = default;
	SqliteDatabase(sqlite3* _db);

};

