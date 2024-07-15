#ifndef JSON_RESPONSE_PACKET_SERIALIZER_H
#define JSON_RESPONSE_PACKET_SERIALIZER_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "json.hpp"
#include "Room.h"

enum ResponseCode {
	LoginCode = 97,
	SingupCode = 98,
	ErrorCode = 99,
	LogoutCode = 100,
	GetRoomsCode = 101,
	GetPlayersInRoomCode = 102,
	JoinRoomCode = 103,
	CreateRoomCode = 104,
	GetHighScoreCode = 105,
	GetPersonalStatisctics = 106,
	StartGameCode = 107,
	CloseRoomCode = 108,
	GetRoomStateCode = 109,
	LeaveRoomCode = 110,

	LeaveGameCode = 111,
	GetQuestionsCode = 112,
	SubmitAnswerCode = 113,
	GetGameResultsCode = 114,

	MoveToMainMenuHandlerCode = 115,

	GetPlayersInGameCode = 116,
	GetGameStatusCode = 117,
	GetHowManyAnsweredCode = 118,
	EndGameCode = 119
};

struct LoginResponse {
	unsigned int status;
};

struct SignupResponse {
	unsigned int status;
};

struct ErrorResponse {
	std::string message;
};

struct LogoutResponse {
	unsigned int status;
};

struct GetRoomsResponse {
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse {
	unsigned int status;
	std::vector<std::string> players;
};

struct getHighScoreResponse {
	unsigned int status;
	std::vector<std::string> usernames;
	std::vector<int> correctAnswerCounters;
	std::vector<double> averageAnswerTimes;

};

struct getPersonalStatusResponse {
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse {
	unsigned int status;
};

struct CreateRoomResponse {
	unsigned int status;
	unsigned int roomId;
};

struct CloseRoomResponse {
	unsigned int status;
};

struct StartGameResponse {
	unsigned int status;
};

struct GetRoomStateResponse {
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct LeaveRoomResponse {
	unsigned int status;
};

//v4

struct EndGameResponse {
	unsigned int status;

};

struct GetHowManyAnsweredResponse {
	unsigned int answered;
	unsigned int outOf;
};

struct GetPlayersInGameResponse {
	unsigned int status;
	std::vector<std::string> players;
};

struct GetGameStatusResponse {
	unsigned int status;//true - running, false - closed
};

struct GoBackToMainMenuResponse {
	unsigned int status;
};


struct GetQuestionsResponse {
	unsigned int status;
	std::string question;
	std::map<std::string, std::string> answers;
};

struct SubmitAnswerResponse {
	unsigned int status;
};


struct PlayerResults {
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	double averageAnswerTime;
};

struct GetGameResultsResponse {
	unsigned int status;
	std::vector<PlayerResults> results;
};

struct LeaveGameResponse {
	unsigned int status;
};


class JsonResponsePacketSerializer {
public:
	//version 4
	static std::vector<unsigned char> serializeResponse(GetGameResultsResponse response );
	static std::vector<unsigned char> serializeResponse(SubmitAnswerResponse response );
	static std::vector<unsigned char> serializeResponse(GetQuestionsResponse response );//
	static std::vector<unsigned char> serializeResponse(LeaveGameResponse response);
	static std::vector<unsigned char> serializeResponse(GoBackToMainMenuResponse response);
	static std::vector<unsigned char> serializeResponse(GetPlayersInGameResponse response);
	static std::vector<unsigned char> serializeResponse(GetHowManyAnsweredResponse response);
	static std::vector<unsigned char> serializeResponse(EndGameResponse response);

	//version 3
	static std::vector<unsigned char> serializeResponse(LeaveRoomResponse response);
	static std::vector<unsigned char> serializeResponse(GetRoomStateResponse response);
	static std::vector<unsigned char> serializeResponse(StartGameResponse response);
	static std::vector<unsigned char> serializeResponse(CloseRoomResponse response);

	//Version 2
	static std::vector<unsigned char> serializeResponse(LogoutResponse response);
	static std::vector<unsigned char> serializeResponse(GetRoomsResponse response);
	static std::vector<unsigned char> serializeResponse(GetPlayersInRoomResponse response);
	static std::vector<unsigned char> serializeResponse(JoinRoomResponse response);
	static std::vector<unsigned char> serializeResponse(CreateRoomResponse response);
	static std::vector<unsigned char> serializeResponse(getHighScoreResponse response);
	static std::vector<unsigned char> serializeResponse(getPersonalStatusResponse response);

	//Version 1
	static std::vector<unsigned char> serializeResponse(ErrorResponse response);
	static std::vector<unsigned char> serializeResponse(SignupResponse response);
	static std::vector<unsigned char> serializeResponse(LoginResponse response);
private:
	static std::string getPaddedNumber(int num, int digits);
};

#endif // JSON_RESPONSE_PACKET_SERIALIZER_H