#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;


class GameRequestHandler : public IRequestHandler {
private:
	IDatabase* m_database;
	Game m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;
	unsigned int _howManyPlayersAtTheStart;
public:
	GameRequestHandler(IDatabase* database, int gameId, LoggedUser user, RequestHandlerFactory& handlerFactory, GameManager& gameManager);
	virtual bool isRequestRelevent(RequestInfo requestInfo) override;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) override;

	RequestResult getQuestion(RequestInfo rqInfo);
	RequestResult submitAnswer(RequestInfo rqInfo);

	RequestResult getGameResult(RequestInfo rqInfo);
	RequestResult leaveGame(RequestInfo rqInfo);
	RequestResult getPlayers(RequestInfo rqInfo);
	RequestResult getHowManyAnswered(RequestInfo rqInfo);
	RequestResult endGame(RequestInfo rqInfo);


	RequestResult leaveGameToMainMenu(RequestInfo requestInfo);
};

