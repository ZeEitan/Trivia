#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(int roomId, LoggedUser user, RequestHandlerFactory& handlerFactory, RoomManager& roomManager)
: m_handlerFactory(handlerFactory), m_roomManager(roomManager), m_room(roomManager.getRoom(roomId)){
	this->m_user = user;
}

bool RoomAdminRequestHandler::isRequestRelevent(RequestInfo info) {
	if(info.id == requestCloseRoomCode)
		return true;

	if(info.id == requestStartGameCode)
		return true;

	if(info.id == requestGetRoomStateCode)
		return true;

	return false;
}

/*
* Handle the request and return the result by the request id
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult RoomAdminRequestHandler::RequestHandler(RequestInfo requestInfo, SOCKET socket) {
	try {

		switch (requestInfo.id){
			case requestCloseRoomCode:
				return closeRoom(requestInfo);
				break;
			case requestStartGameCode:
				return startGame(requestInfo);
				break;
			case requestGetRoomStateCode:
				return getRoomState(requestInfo);
				break;
		default:
			throw std::exception("ERROR: Not a room admin! | No relevent req - " + requestInfo.id);
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
RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo requestInfo) {
	std::vector<unsigned char> buffer;

	unsigned int status = 1;
	bool hasGameBegun = m_room.getMetadata().isActive;

	std::cout << "mAdmin-=-=-=--="<<this->m_user.getUsername() << " " << this->m_room.getMetadata().isActive << "   " << this->m_room.getId() << std::endl;

	std::vector<std::string> players = this->m_roomManager.getRoom(this->m_room.getId()).getAllUsers();


	for (int i = 0; i < players.size(); i++) {
		std::cout << players[i] << std::endl;
	}

	unsigned int questionCount = m_room.getMetadata().numOfQuestionsInGame;
	unsigned int answerTimeout = m_room.getMetadata().timePerQuestion;
	//std::cout << "time:" << m_room.getMetadata().timePerQuestion << std::endl;
	GetRoomStateResponse response{
		1,
		this->m_roomManager.getRoomState(this->m_room.getMetadata().id),
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getAllUsers(),
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getMetadata().numOfQuestionsInGame,
		this->m_roomManager.getRoom(this->m_room.getMetadata().id).getMetadata().timePerQuestion //Take data from the room maneger
	};
	//std::cout << "time:"<<response.answerTimeout << std::endl;
	
	buffer = JsonResponsePacketSerializer::serializeResponse(response);

	return RequestResult{ buffer, nullptr };
}

/*
* Handle the start game request and returns the result of the request and the new handler if needed
input:
*	RequestInfo info - the request info [buffer - with the request data]
output:
*	RequestResult - the result of the request [buffer - with the response data]
*/
RequestResult RoomAdminRequestHandler::startGame(RequestInfo requestInfo){
	std::vector<unsigned char> buffer;
	
	m_room.setRoomActiveStatus(true); //Change the room status to active
	this->m_roomManager.setActiveStatus(this->m_room.getMetadata().id, true);

	//std::cout << "-=-=-=--=" << this->m_user.getUsername() << " " << this->m_room.getMetadata().isActive <<"   "<< this->m_room.getId()<< std::endl;
	//std::cout << "option1 : " << m_room.getMetadata().isActive << std::endl;
	//std::cout << "option2 : " << this->m_roomManager.getRoomState(this->m_room.getMetadata().id) << std::endl;


	this->m_roomManager.getRoom(this->m_room.getMetadata().id).setRoomActiveStatus(true); //TODO: Need to update all the room socketst
	this->m_room.getLoggedUsers();
	StartGameResponse response{ 1 };
	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	
	this->m_handlerFactory.getGameManager().createGame(    this->m_roomManager.getRoom(this->m_room.getId()), this->m_user   );//creates the game
	return RequestResult{ buffer, this->m_handlerFactory.createGameRequestHandler(this->m_user, this->m_room.getId(), this->m_roomManager.getRoom(this->m_room.getId()))};

}

/*
* Handle the close room request and returns the result of the request and the new handler if needed
input:
*	RequestInfo info - the request info [buffer - with the request data]
iuotput:
*	RequestResult - the result of the request [buffer - with the response data]
*/
RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo requestInfo) {
	std::cout << "closing room" << std::endl;
	std::vector<unsigned char> buffer;

	for (auto& user : this->m_room.getLoggedUsers()) { //Update all the users in the room

		if (user.getSocket() == this->m_user.getSocket())
			continue;

		this->m_roomManager.getRoom(this->m_room.getMetadata().id).removeUser(user);

		buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{1});
		std::string message(buffer.begin(), buffer.end());
		Helper::sendToSocket(this->m_user.getSocket(), message);
	}
	
	this->m_roomManager.deleteRoom(m_room.getMetadata().id);
	buffer = JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse{ 1 });
	return RequestResult{ buffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user)};
}