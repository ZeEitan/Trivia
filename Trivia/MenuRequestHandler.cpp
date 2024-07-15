#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& Factory, LoggedUser& User)
	: m_handlerFactory(Factory), m_user(User.getUsername(), User.getSocket())
{};
bool MenuRequestHandler::isRequestRelevent(RequestInfo info) {
	if (info.id == requestCreateRoomCode)
		return true;

	if (info.id == requestGetHighScoreCode)
		return true;

	if (info.id == requestGetPersonalStatiscticsCode)
		return true;

	if (info.id == requestGetPlayersInRoomCode)
		return true;

	if (info.id == requestGetRoomsCode)
		return true;

	if (info.id == requestJoinRoomCode)
		return true;

	if (info.id == requestLogoutCode)
		return true;

	return false;
}

/*
*Handle the request and return the result by the request id
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult MenuRequestHandler::RequestHandler(RequestInfo info, const SOCKET socket) {
	try {
		switch (info.id) {
		case requestLogoutCode:
			return this->signout(info);
		case requestGetRoomsCode:
			return this->getRooms(info);
		case requestGetPlayersInRoomCode:
			return this->getPlayersInRoom(info);
		case requestJoinRoomCode:
			return this->joinRoom(info);
		case requestCreateRoomCode:
			return this->createRoom(info);
		case requestGetHighScoreCode:
			return this->getHighScore(info);
		case requestGetPersonalStatiscticsCode:
			return this->getPersonalStats(info);
		default:
			std::cout << "Error: Allready authentication" << std::endl;
			ErrorResponse error{ "Allready authentication for MenuRequestHandler" };
			return RequestResult{ JsonResponsePacketSerializer::serializeResponse(error), nullptr };
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		ErrorResponse error{ e.what() };
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(error), nullptr };
	}
}

/*
* Handle the signout request
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult MenuRequestHandler::signout(RequestInfo info) {
	std::vector<unsigned char> buffer;

	LoginManager& loginManager = this->m_handlerFactory.getLoginManager();
	if (loginManager.logout(m_user.getUsername())) {
		std::cout << "Logout success" << std::endl;
		buffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ 1});
		return RequestResult{ buffer,  this->m_handlerFactory.createLoginRequestHandler()};
	}

	buffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ 1 });
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the get rooms request
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult MenuRequestHandler::getRooms(RequestInfo info) {
	RoomManager& roomManager = this->m_handlerFactory.getRoomManager();
	std::vector<RoomData> rooms = roomManager.getRooms();

	GetRoomsResponse response = GetRoomsResponse{ 1, rooms };
	std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the get players in room request
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*	buffer data [player1, player2, ...]
*/
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info) {
	RoomManager& roomManager = this->m_handlerFactory.getRoomManager();
	GetPlayersInRoomRequest getPlayersReq = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);

	Room room = roomManager.getRoom(getPlayersReq.roomId);
	std::vector<std::string> players = room.getAllUsers();

	GetPlayersInRoomResponse response = GetPlayersInRoomResponse{ 1, players };
	std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the get personal stats request
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*	buffer data [playerGames, playerCorrectAnswers, playerWrongAnswers, playerAverageAnswerTime]
*/
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info){
	StatisticsManager& statisticsManager = this->m_handlerFactory.getStatisticsManager();
	std::vector<std::string> statistics = statisticsManager.getUserStatistics(this->m_user.getUsername());

	getPersonalStatusResponse response = getPersonalStatusResponse{ 1, statistics };
	std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the get high score request
input:
*	RequestInfo info - the request info
output:
*	RequestResult - the result of the request
*   The first 5 high scores
*/
RequestResult MenuRequestHandler::getHighScore(RequestInfo info) {
	StatisticsManager& statisticsManager = this->m_handlerFactory.getStatisticsManager();
	std::vector<Score> highScores = statisticsManager.getHighScore();

	getHighScoreResponse response;
	response.status = 1;

	for (int i = 0; i < highScores.size(); i++) {
		response.usernames.push_back(highScores[i].username); 
		response.correctAnswerCounters.push_back(highScores[i].correctAnswerCount);
		response.averageAnswerTimes.push_back(highScores[i].averageAnswerTime); 
	}
	


	std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, nullptr };
}

/*
* Handle the join room request
input:
*	RequestInfo info - the request info
* output:
*  RequestResult - the result of the request
*/
RequestResult MenuRequestHandler::joinRoom(RequestInfo info){
	std::vector<unsigned char> buffer;
	RoomManager& roomManager = this->m_handlerFactory.getRoomManager();
	JoinRoomRequest joinRoomReq = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);

	try {
		Room& room = roomManager.getRoom(joinRoomReq.roomId);
		room.addUser(this->m_user);
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() });
		return RequestResult{ buffer, nullptr };
	}

	JoinRoomResponse response = JoinRoomResponse{ 1 };
	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	return RequestResult{ buffer, m_handlerFactory.createRoomMemberRequestHandler(this->m_user, joinRoomReq.roomId)};
}

/*
* Handle the create room request
input:
*	RequestInfo info - the request info
output:
* RequestResult - the result of the request
* 1 - success
* Error - error message
*/
RequestResult MenuRequestHandler::createRoom(RequestInfo info) {
	std::vector<unsigned char> buffer;
	RoomManager& roomManager = this->m_handlerFactory.getRoomManager();
	CreateRoomRequest createRoomReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);

	RoomData metadata = RoomData{
		createRoomReq.roomName,
		this->generateRoomID(createRoomReq.roomName),
		createRoomReq.maxUsers,
		createRoomReq.answerTimeout,
		createRoomReq.questionCount,
		false
	};
	std::cout << "--------" << this->m_user.getUsername() << "     " << metadata.id << std::endl;

	try {
		roomManager.createRoom(this->m_user, metadata); 
	}
	catch (std::exception e) { 
		std::cout << e.what() << std::endl; 
		buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }); 
		std::cout << "in catch" << std::endl;
		return RequestResult{ buffer, nullptr }; 
	} 

	CreateRoomResponse response = CreateRoomResponse{
		1,
		metadata.id
	}; 
	buffer = JsonResponsePacketSerializer::serializeResponse(response);
	std::cout << "outside catch" << std::endl;
	return RequestResult{ buffer, m_handlerFactory.createRoomAdminRequestHandler(m_user, metadata.id)};
}

/*
* Generate a room id by the room name using the hash function
input:
*	std::string roomName - the room name
output:
*	unsigned int - the room id generated by the hash function
*/
unsigned int MenuRequestHandler::generateRoomID(std::string roomName) {
	std::hash<std::string> hash;
	return hash(roomName);
}
