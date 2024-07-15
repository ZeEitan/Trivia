#pragma once
#include <map>
#include <iostream>
#include <vector>
#include "SqliteDatabase.h"
#include "Question.h"
#include "Room.h"


extern std::mutex gameMutex;


struct GameData {
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
};

class Game { 
private:
	IDatabase* m_database;
	std::vector<Question> questions;
	std::map<LoggedUser, GameData> m_players;

	std::map<LoggedUser, std::vector<Question>> questionsOfPlayers;
	int m_gameId;
	LoggedUser _host;
	
	

	std::vector<int> getQuestionsId(int max, int totalNumbers);
	std::vector<std::string> getAnswersFromDatabase(int questionId);
	std::string GetQuestionFromDatabase(int questionId);

public:
	void submitGameStatsToDB(std::string username, GameData gameData); 
	int getGameId();
	Game(Room room, IDatabase* database, LoggedUser host);
	Question getQuestionForUser(LoggedUser user);
	Question getPlayerCurrentQuestion(LoggedUser user);
	double updateAverage(double current_average, int count, double new_number); 
	void submitAnswer(LoggedUser user, int questionId, std::string answer, int answerTime);
	void removeUser(LoggedUser user);
	std::map<LoggedUser, GameData> getPlayers();
	LoggedUser getHost();
	bool checkIfUserExists(LoggedUser user);
};

