#pragma once

#include <vector>
#include <ctime>
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class IRequestHandler;

enum RequestCode {
    requestLoginCode = 'a',              // 97
    requestSingupCode = 'b',             // 98
    requestErrorCode = 'c',              // 99
    requestLogoutCode = 'd',             // 100
    requestGetRoomsCode = 'e',           // 101
    requestGetPlayersInRoomCode = 'f',   // 102
    requestJoinRoomCode = 'g',           // 103
    requestCreateRoomCode = 'h',         // 104
    requestGetHighScoreCode = 'i',       // 105
    requestGetPersonalStatiscticsCode = 'j', // 106
    requestStartGameCode = 'k',          // 107
    requestCloseRoomCode = 'l',          // 108
    requestGetRoomStateCode = 'm',       // 109
    requestLeaveRoomCode = 'n',          // 110
    requestLeaveGameCode = 'o',          // 111
    requestGetQuestionsCode = 'p',       // 112
    requestSubmitAnswerCode = 'q',       // 113
    requestGetGameResultsCode = 'r',     // 114
    requestMoveToMainMenuHandlerCode = 's', // 115
    requestGetPlayersInGameCode = 't',   // 116
    requestGetGameStatusCode = 'u',      // 117
    requestGetHowManyAnsweredCode = 'v', // 118
    requestEndGameCode = 'w'             // 119
};

struct RequestInfo {
	int id;
	std::time_t recivedTime;
	std::vector<unsigned char> buffer;
};

struct RequestResult {
	std::vector<unsigned char> responseBuffer;
	IRequestHandler* newHandler;
};

class IRequestHandler {
public:
	IRequestHandler() = default;
	virtual bool isRequestRelevent(RequestInfo info) = 0;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) = 0;
};