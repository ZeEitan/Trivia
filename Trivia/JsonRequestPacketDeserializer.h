#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "json.hpp"

struct LoginRequest {
	std::string username;
	std::string password;
};

struct SignupRequest {
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest {
	unsigned int roomId;
};

struct GetPlayersInGameRequest {
	unsigned int gameId;
};

struct JoinRoomRequest {
	unsigned int roomId;
};

struct SubmitAnswerRequest {
	unsigned int answerTime;
	std::string answer;
};


struct GetHowManyAnsweredRequest {
	unsigned int questionNnm;
};

struct CreateRoomRequest {
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct EndGameRequest {
	unsigned int gameId;
};



class JsonRequestPacketDeserializer {
public:
	//Version 4
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(std::vector<unsigned char>& buffer);
	static GetPlayersInGameRequest deserializeGetPlayersInGameRequest(std::vector<unsigned char>& buffer);
	static GetHowManyAnsweredRequest deserializeGetHowManyAnsweredRequest(std::vector<unsigned char>& buffer);
	static EndGameRequest deserializeEndGameRequest(std::vector<unsigned char>& buffer);
	
	//Version 2
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(std::vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char>& buffer);

	//Version 1
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char>& buffer);
	static SignupRequest deserializerSignupRequest(std::vector<unsigned char>& buffer);
private:
	static std::string bufferToJsonString(std::vector<unsigned char>& buffer);
};

