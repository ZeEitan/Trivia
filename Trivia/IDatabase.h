#pragma once

#include <iostream>
#include <string>

#include "sqlite3.h"
#include <vector>
class LoggedUser;


struct Score {
	std::string username;
	int correctAnswerCount;
	double averageAnswerTime;

};


class IDatabase {

public:
	virtual sqlite3* getDB() = 0;
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int doesUserExists(std::string usrname) = 0;
	virtual int doesPasswordMatch(std::string username, std::string password) = 0;
	virtual int addNewUser(std::string username, std::string password, std::string mail) = 0;
	virtual int doesMailExists(std::string mail) = 0;
	virtual int addNewUserStatistics(std::string username) = 0;
	/*
	virtual List<Question> getQuestions(int gameId) = 0;
	*/

	virtual double getPlayerAverageAnswerTime(std::string username) = 0;
	virtual int getNumOfCorrectAnswers(std::string username) = 0;
	virtual int getNumOfTotalAnswers(std::string username) = 0;
	virtual int getNumOfPlayerGames(std::string username) = 0;

	
	virtual double editPlayerAverageAnswerTime(std::string username, double answerTime) = 0;
	virtual int incNumOfCorrectAnswers(std::string username) = 0;
	virtual int incNumOfTotalAnswers(std::string username) = 0;
	virtual int addAnswerToStatisticsTable(std::string username, bool isCorrect, double answerTime, int gameID) = 0;


	virtual int addAnswer(std::string username, bool isCorrect, double answerTime, int gameID) = 0;
	virtual int incNumOfPlayerGames(std::string username) = 0;




	virtual int getHowManyGamesAreRegistered(std::string username) = 0;
	virtual int shiftGamesInHistory(std::string username,int currentLen) = 0;
	virtual int submitGameToGameHistory(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId, int queue) = 0;
	virtual int submitGameStatistics(std::string username, int correctAnswerCount, double averageAnswerTime, int gameId) = 0;







	virtual int submitAnswer(std::string username, int gameId, int questionId, int isCorrect, double answerTime, std::string answer) = 0;

	virtual int getPlayerScore(std::string username) = 0;
	virtual std::vector<Score> getHighScores() = 0;
	virtual std::vector<std::string> getUserStatistics(std::string username) = 0;
	

	virtual std::string getQuestion(int questionId) = 0;
	virtual std::vector<std::string> getAnswers(int questionId) = 0;

}; 
