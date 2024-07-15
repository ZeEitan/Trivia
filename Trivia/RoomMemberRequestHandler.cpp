#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(int roomId, LoggedUser user, RequestHandlerFactory& handlerFactory, RoomManager& roomManager)
: m_handlerFactory(handlerFactory), m_roomManager(roomManager), m_room(roomManager.getRoom(roomId)){
		m_user = user;
}

bool RoomMemberRequestHandler::isRequestRelevent(RequestInfo requestInfo)
{
	return requestInfo.id == requestGetRoomStateCode || requestInfo.id == requestLeaveRoomCode || requestInfo.id == requestMoveToMainMenuHandlerCode;
}

/*
* Handle the request and return the result by the request id
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult RoomMemberRequestHandler::RequestHandler(RequestInfo requestInfo, const SOCKET socket) {
	if (!isRequestRelevent(requestInfo))
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(ErrorResponse{"Request is not relevnt for RoomMemberRequestHandler" }), nullptr };
	try {
		switch (requestInfo.id) {
		case requestGetRoomStateCode:
			return getRoomState(requestInfo);
			break;
		case requestLeaveRoomCode:
			return leaveRoom(requestInfo); //logout from room
			break;
		case requestMoveToMainMenuHandlerCode:
			return leaveRoomToMainMenu(requestInfo);
			break;
		default:
			throw std::exception("ERROR: Request is not relevnt for RoomMemberRequestHandler");
			break;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		ErrorResponse error{ e.what() };
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(error), nullptr };
	}
	
}
/*
* Handle the get room state request and returns the result of the request and the new handler if needed
input:
*	RequestInfo info - the request info [buffer - with the request data]
output:
*	RequestResult - the result of the request
*   Example of the response:
	m0091{
		"answerTimeout" : 10,
		"hasGameBegun" : false,
		"players" : ["an", "an"],
		"questionCount" : 10,
		"status" : 1,
	}
*/
RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo requestInfo) {
	std::vector<unsigned char> buffer;

	unsigned int status = 1;
	bool hasGameBegun = m_room.getMetadata().isActive;
	std::vector<std::string> players = this->m_roomManager.getRoom(this->m_room.getId()).getAllUsers();
	unsigned int questionCount = m_room.getMetadata().numOfQuestionsInGame;
	unsigned int answerTimeout = m_room.getMetadata().timePerQuestion;

	std::cout << "mMember-=-=-=--=" << this->m_user.getUsername() << " " << this->m_room.getMetadata().isActive <<"   "<< this->m_room.getId()<< std::endl;


	for (int i = 0; i < players.size(); i++) {
		std::cout << players[i] << std::endl;
	}

	//std::cout << "option1 : " << m_room.getMetadata().isActive << std::endl;
	//std::cout << "option2 : " << this->m_roomManager.getRoomState(this->m_room.getMetadata().id) << std::endl;

	GetRoomStateResponse response{ 
		1,
		this->m_roomManager.getRoomState(this->m_room.getMetadata().id),
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getAllUsers(),
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getMetadata().numOfQuestionsInGame,
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getMetadata().timePerQuestion //Take data from the room maneger
	};
	/*
	struct GetRoomStateResponse {
		unsigned int status;
		bool hasGameBegun;
		std::vector<std::string> players;
		unsigned int questionCount;
		unsigned int answerTimeout;
	};
*/

	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	if (this->m_roomManager.getRoomState(this->m_room.getMetadata().id)) {
		std::cout << "----------------game has started-------------" << this->m_user.getUsername() << std::endl;
		return RequestResult{ buffer, this->m_handlerFactory.createGameRequestHandler(this->m_user, this->m_room.getId(), this->m_roomManager.getRoom(this->m_room.getId())) };
	}
	std::cout << "----------------game not started-------------" << this->m_user.getUsername()<< std::endl;
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the leave room request and returns the result of the request and the new handler if needed
input:
*	RequestInfo info - the request info [buffer - with the request data]
*  Example of the response:
* 	n0054{
* 		"status" : 1,
* 	}
*/
RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo requestInfo) {
	std::vector<unsigned char> buffer;

	this->m_roomManager.getRoom(this->m_room.getMetadata().id).removeUser(this->m_user);

	buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	return RequestResult{ buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomMemberRequestHandler::leaveRoomToMainMenu(RequestInfo requestInfo)
{
	std::vector<unsigned char> buffer;

	buffer = JsonResponsePacketSerializer::serializeResponse(GoBackToMainMenuResponse{ 1 });
	return RequestResult{ buffer, m_handlerFactory.createMenuRequestHandler(m_user) }; 
}


