#ifndef MENU_REQUEST_HANDLER_H
#define MENU_REQUEST_HANDLER_H

#include "IRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RoomManager.h"
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "StatisticsManager.h"
#include <WinSock2.h>
#include <Windows.h>

#include "RequestHandlerFactory.h"

#include <ctime>
#include <chrono>

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler {
public:
	MenuRequestHandler(RequestHandlerFactory& Factory, LoggedUser& User);
	virtual bool isRequestRelevent(RequestInfo info) override;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) override;

private:
	RequestHandlerFactory& m_handlerFactory;
	LoggedUser m_user;

	RequestResult signout(RequestInfo info);
	RequestResult getRooms(RequestInfo info);
	RequestResult getPlayersInRoom(RequestInfo info);
	RequestResult getPersonalStats(RequestInfo info);
	RequestResult getHighScore(RequestInfo info);
	RequestResult joinRoom(RequestInfo info);
	RequestResult createRoom(RequestInfo info);

	unsigned int generateRoomID(std::string roomName);
};

#endif // MENU_REQUEST_HANDLER_H