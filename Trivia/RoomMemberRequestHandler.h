#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;


class RoomMemberRequestHandler : public IRequestHandler {
private:
	Room m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;
public:
	RoomMemberRequestHandler(int roomId, LoggedUser user, RequestHandlerFactory& handlerFactory, RoomManager& roomManager) ;
	virtual bool isRequestRelevent(RequestInfo requestInfo) override;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) override;

	RequestResult getRoomState(RequestInfo requestInfo);
	RequestResult leaveRoom(RequestInfo requestInfo);
	RequestResult leaveRoomToMainMenu(RequestInfo requestInfo);
};

