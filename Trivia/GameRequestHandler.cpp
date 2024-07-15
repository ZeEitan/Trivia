#include "GameRequestHandler.h"



GameRequestHandler::GameRequestHandler(IDatabase* database, int gameId, LoggedUser user, RequestHandlerFactory& handlerFactory, GameManager& gameManager)
	: m_handlerFactory(handlerFactory), m_gameManager(gameManager), m_game(gameManager.getGame(gameId)), m_user(user), m_database(database) { 
	this->_howManyPlayersAtTheStart = this->m_game.getPlayers().size();
}

bool GameRequestHandler::isRequestRelevent(RequestInfo requestInfo){
	if (requestInfo.id == requestEndGameCode || requestInfo.id == requestGetHowManyAnsweredCode ||  requestInfo.id == requestGetQuestionsCode || requestInfo.id == requestMoveToMainMenuHandlerCode || requestInfo.id == requestSubmitAnswerCode || requestInfo.id == requestGetPlayersInGameCode || requestInfo.id == requestGetGameResultsCode || requestInfo.id == requestLeaveGameCode) {
		return true;
	}
	
	return false;
}

RequestResult GameRequestHandler::RequestHandler(RequestInfo requestInfo, const SOCKET socket) {
	if (!isRequestRelevent(requestInfo))
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(ErrorResponse{"Request is not relevant" }), nullptr };
	try {
		switch (requestInfo.id) {
		case requestGetQuestionsCode:
			return getQuestion(requestInfo);
			break;
		case requestGetGameResultsCode:
			return getGameResult(requestInfo);
			break;
		case requestSubmitAnswerCode:
			return submitAnswer(requestInfo);
			break;
		case requestLeaveGameCode:
			return leaveGame(requestInfo);
			break;
		case requestGetPlayersInGameCode:
			return getPlayers(requestInfo);
			break;
		case requestMoveToMainMenuHandlerCode:
			return leaveGameToMainMenu(requestInfo);
			break;
		case requestGetHowManyAnsweredCode:
			return getHowManyAnswered(requestInfo);
			break;
		case requestEndGameCode:
			return endGame(requestInfo);
			break;
		default:
			throw std::exception("ERROR: Request is not relevant for GameRequestHandler");
			break;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		ErrorResponse error{ e.what() };
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(error), nullptr };
	}

}
//std::string question;
//std::map<unsigned int, std::string> answers;
RequestResult GameRequestHandler::getQuestion(RequestInfo rqInfo){
	std::vector<unsigned char> buffer;

	Game game = this->m_gameManager.getGame(this->m_game.getGameId()); 

	Question question = game.getQuestionForUser(this->m_user);

	this->m_gameManager.setGame(game, this->m_game.getGameId());

	std::map<std::string, std::string> answers;
	std::vector<std::string> possibleAnswers = question.getPossibleAnswers();
	std::string answersFinalString = "";
	for (int i = 0; i < possibleAnswers.size(); i++) {
		answers[std::to_string(i)] = possibleAnswers[i];
		
	}
	

	int status = 1;
	GetQuestionsResponse response{
		status,
		question.getQuestion(),
		answers,
	};

	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}

//unsigned int status;
//unsigned int correctAnswerId;
RequestResult GameRequestHandler::submitAnswer(RequestInfo rqInfo) {
	std::vector<unsigned char> buffer;

	
	SubmitAnswerRequest submitAnswer = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(rqInfo.buffer); 

	Game game = this->m_gameManager.getGame(this->m_game.getGameId()); 
	Question question = game.getPlayerCurrentQuestion(this->m_user); 

	
	game.submitAnswer(this->m_user, question.getQuestionId(), submitAnswer.answer, submitAnswer.answerTime);
	this->m_gameManager.setGame(game, this->m_game.getGameId());
	int status = 1;
	SubmitAnswerResponse response{
		status
	};

	buffer = JsonResponsePacketSerializer::serializeResponse(response);  
	return RequestResult{ buffer, nullptr };
}




// unsigned int status;
// std::vector<PlayerResults> results;
RequestResult GameRequestHandler::getGameResult(RequestInfo rqInfo) {
	std::vector<unsigned char> buffer;
	
	std::vector<PlayerResults> results; 

	Game game = this->m_gameManager.getGame(this->m_game.getGameId());
	std::map<LoggedUser, GameData> players = game.getPlayers();
	for (const auto& player : players) {

		PlayerResults playerResult;
		playerResult.username = player.first.getUsername();
		playerResult.averageAnswerTime = player.second.averageAnswerTime;
		playerResult.correctAnswerCount = player.second.correctAnswerCount;
		playerResult.wrongAnswerCount = player.second.wrongAnswerCount;

		results.push_back(playerResult);

		if (this->m_user == player.first) {
			game.submitGameStatsToDB(player.first.getUsername(), player.second); 
		}
		
	}


	GetGameResultsResponse response{ 1, results};

	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}





RequestResult GameRequestHandler::leaveGame(RequestInfo rqInfo)
{
	std::vector<unsigned char> buffer;

	Game game = this->m_gameManager.getGame(this->m_game.getGameId());


	if (this->m_user.getUsername() == game.getHost().getUsername()) {
		game.removeUser(this->m_user); 
		this->m_gameManager.setGame(game, this->m_game.getGameId()); 

		this->m_gameManager.deleteGame(game.getGameId());
		this->m_handlerFactory.getRoomManager().deleteRoom(game.getGameId());
	}
	else {
		game.removeUser(this->m_user); 
		this->m_gameManager.setGame(game, this->m_game.getGameId()); 
	}



	buffer = JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse{ 1 }); 
	return RequestResult{ buffer, m_handlerFactory.createMenuRequestHandler(m_user) };

}

RequestResult GameRequestHandler::getPlayers(RequestInfo rqInfo)
{
	std::vector<unsigned char> buffer;
	std::vector<std::string> players; 

	if (this->m_gameManager.checkIfGameExists(this->m_game.getGameId()) == true) { 
		std::map<LoggedUser, GameData> playersBefore = this->m_gameManager.getGame(this->m_game.getGameId()).getPlayers();

		for (const auto& player : playersBefore) {
			players.push_back(player.first.getUsername());
			std::cout << "123456789--- " << player.first.getUsername() << std::endl;
		}

		buffer = JsonResponsePacketSerializer::serializeResponse(GetPlayersInGameResponse{ 1, players });
		return RequestResult{ buffer, nullptr };
	}
	else { 

		std::vector<std::string> players;  
		buffer = JsonResponsePacketSerializer::serializeResponse(GetPlayersInGameResponse{ 0, players }); 
		return RequestResult{ buffer, nullptr }; 
	}

	
}

RequestResult GameRequestHandler::getHowManyAnswered(RequestInfo rqInfo) {
	std::vector<unsigned char> buffer;

	GetHowManyAnsweredRequest request = JsonRequestPacketDeserializer::deserializeGetHowManyAnsweredRequest(rqInfo.buffer);
	int questionNum = request.questionNnm;

	std::map<LoggedUser, GameData> players = this->m_gameManager.getGame(this->m_game.getGameId()).getPlayers();
	unsigned int outOf = players.size();
	unsigned int answered = 0;

	for (const auto& player : players) {
		if (questionNum == (player.second.correctAnswerCount + player.second.wrongAnswerCount)) { 
			answered++; 
		}
	}

	buffer = JsonResponsePacketSerializer::serializeResponse(GetHowManyAnsweredResponse{ answered, outOf });
	return RequestResult{ buffer, nullptr };
}




RequestResult GameRequestHandler::endGame(RequestInfo rqInfo)
{
	std::vector<unsigned char> buffer; 
	EndGameRequest request = JsonRequestPacketDeserializer::deserializeEndGameRequest(rqInfo.buffer); 

	this->m_handlerFactory.getRoomManager().deleteRoom(request.gameId);
	this->m_gameManager.deleteGame(request.gameId);

	buffer = JsonResponsePacketSerializer::serializeResponse(EndGameResponse{ 1 });
	return RequestResult{ buffer , nullptr };
}



RequestResult GameRequestHandler::leaveGameToMainMenu(RequestInfo requestInfo)
{
	std::vector<unsigned char> buffer;

	buffer = JsonResponsePacketSerializer::serializeResponse(GoBackToMainMenuResponse{ 1 });

	return RequestResult{ buffer, m_handlerFactory.createMenuRequestHandler(this->m_user) };  

}






//std::vector<unsigned char> buffer;
//unsigned int status = 1;
//this->m_game.removeUser(this->m_user);
////this->handleRequest(rqInfo);
////this->m_game.removeUser();
//LeaveGameResponse response{ status };
//buffer = JsonResponsePacketSerializer::serializeResponse(response);
//return RequestResult{ buffer, nullptr };  




