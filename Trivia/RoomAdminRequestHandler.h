#ifndef ROOMADMINREQUESTHANDLER_H
#define ROOMADMINREQUESTHANDLER_H

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "Helper.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler {
private:
	Room m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;

	RequestResult getRoomState(RequestInfo requestInfo);
	RequestResult startGame(RequestInfo requestInfo);
	RequestResult closeRoom(RequestInfo requestInfo);


public:
	RoomAdminRequestHandler(int roomId, LoggedUser user, RequestHandlerFactory& handlerFactory, RoomManager& roomManager);
	virtual bool isRequestRelevent(RequestInfo info) override;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) override;
};

#endif // !ROOMADMINREQUESTHANDLER_H