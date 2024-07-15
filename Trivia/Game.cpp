#include "Game.h"



std::mutex gameMutex;

void Game::submitGameStatsToDB(std::string username, GameData gameData){
	this->m_database->submitGameStatistics(username, gameData.correctAnswerCount, gameData.averageAnswerTime,this->m_gameId);
}

int Game::getGameId(){
	return this->m_gameId;
}

Game::Game(Room room, IDatabase* database, LoggedUser host) {
	std::cout << "-------------in game constructor" << std::endl;
	this->m_database = database;
	room.setRoomActiveStatus(1);
	this->m_gameId = room.getId();
	this->_host = host;

	std::vector<int> ids = getQuestionsId(10, room.getMetadata().numOfQuestionsInGame);// change the 10 according to the total questions in the database
	for (int i = 0; i < ids.size(); i++) {
		std::cout << ids[i] << std::endl;
	}
	for (int i = 0; i < ids.size(); i++) {
		std::vector<std::string> possibleAnswers = getAnswersFromDatabase(ids[i]);
		std::string theQuestion = GetQuestionFromDatabase(ids[i]);

		this->questions.push_back(Question(ids[i],theQuestion,possibleAnswers,possibleAnswers[0]));

		for (const auto& player : this->questionsOfPlayers){
			this->questionsOfPlayers.at(player.first).push_back(Question(ids[i], theQuestion, possibleAnswers, possibleAnswers[0])); 
		}
	}
	//filling the questions vector with the randomg ids
	
	
	std::vector<LoggedUser> v = room.getLoggedUsers();
	/*for (int i = 0; i < v.size(); i++) {
		std::cout << "inGame1: " << v[i].getUsername() << std::endl;
	}*/

	std::cout << std::endl;
	for (int i = 0; i < v.size(); i++) {
		LoggedUser user(v[i]);
		std::cout << "inGame2: " << user.getUsername() << std::endl;
		this->m_players[user] = { this->questions[0], 0, 0, 0}; //GameData Struct AS {}
		this->questionsOfPlayers[user] = std::vector<Question>(this->questions);
	}


	/*std::cout << std::endl;
	for (const auto& player : this->m_players) {
		std::cout << "inGame3: " << player.first.getUsername() << std::endl;
	}*/
}

Question Game::getQuestionForUser(LoggedUser user) {
	std::unique_lock<std::mutex> lock(gameMutex);

	GameData gData = this->m_players[user];
	//Question q = this->questions.back();
	Question q = this->questionsOfPlayers.at(user).back();  
	gData.currentQuestion = q; 
	//this->questions.pop_back();  
	this->questionsOfPlayers.at(user).pop_back();  
	this->m_players[user] = gData; 

	lock.unlock();
	return q;
}

Question Game::getPlayerCurrentQuestion(LoggedUser user)
{
	std::map<LoggedUser, GameData>::iterator it = this->m_players.find(user);
	if (it != this->m_players.end()) {
		return it->second.currentQuestion;
	}
	throw std::exception("user not found");
}


double Game::updateAverage(double current_average, int count, double new_number) {
	// Calculate the new average
	double new_average = double((current_average * count) + new_number) / double(count + 1);
	return new_average;
}


void Game::submitAnswer(LoggedUser user, int questionId, std::string answer, int answerTime) {
	std::string cAnswer = this->m_players.at(user).currentQuestion.getCorrectAnswer();
	int correct = 0;
	if (cAnswer == answer) {
		correct = 1;
	}
	if (checkIfUserExists(user)) {
		auto& player = this->m_players.at(user);
		if (correct == 1) 
			player.correctAnswerCount = player.correctAnswerCount + 1; 

		else if (correct == 0) 
			player.wrongAnswerCount = player.wrongAnswerCount + 1; 
		
		player.averageAnswerTime = updateAverage(player.averageAnswerTime, ((player.correctAnswerCount + player.wrongAnswerCount) - 1), answerTime);
		std::cout << "-=-=-=-=-=-=-=-=-=-=new average : " + std::to_string(player.averageAnswerTime) << std::endl;
	}
	
	this->m_database->submitAnswer(user.getUsername(), this->m_gameId, this->m_players.at(user).currentQuestion.getQuestionId(), correct, (double)answerTime, answer);
}



void Game::removeUser(LoggedUser user){

	for (const auto& player : this->m_players) {
		std::cout << "removeUser1 - " << player.first.getUsername() << std::endl;
	}

	auto it = this->m_players.find(user);
	if (it != this->m_players.end()) {
		this->m_players.erase(it);
	}
	
	
	for (const auto& player : this->m_players) { 
		std::cout <<"	removeUser2 - "<< player.first.getUsername() << std::endl; 
	}
}

std::map<LoggedUser, GameData> Game::getPlayers()
{
	return this->m_players;
}

LoggedUser Game::getHost()
{
	return this->_host;
}

bool Game::checkIfUserExists(LoggedUser user)
{
	const auto& player = this->m_players.find(user);
	if (player == this->m_players.end()) {
		return false;
	}
	return true;
}

std::vector<int> Game::getQuestionsId(int max, int totalNumbers) {
	std::vector<int> result;

	srand(time(0));
	while (result.size() < totalNumbers) {
		int randomNumber = rand() % (max + 1);
		bool isUnique = true;
		for (int num : result) {
			if (randomNumber == 0) {
				isUnique = false; 
				break;
			}
			if (num == randomNumber) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			result.push_back(randomNumber);
		}
	}

	return result;
}


std::vector<std::string> Game::getAnswersFromDatabase(int questionId)
{
	return this->m_database->getAnswers(questionId);
}

std::string Game::GetQuestionFromDatabase(int questionId)
{
	return this->m_database->getQuestion(questionId);
}


