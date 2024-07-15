#include "JsonResponsePacketSerializer.h"

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse response) {
    nlohmann::json json;
    json["status"] = response.message;
    std::string serialized = json.dump();

    serialized = (char)ResponseCode::ErrorCode + getPaddedNumber(serialized.length(), 4) + serialized;
    std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

    return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse response) {
    nlohmann::json json;
    json["status"] = std::to_string(response.status);
    std::string serialized = json.dump();

    serialized = (char)ResponseCode::SingupCode + getPaddedNumber(serialized.length(), 4) + serialized;
    std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

    return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse response) {
    nlohmann::json json;
    json["status"] = std::to_string(response.status);
    std::string serialized = json.dump();

    serialized = (char)ResponseCode::LoginCode + getPaddedNumber(serialized.length(), 4) + serialized;
    std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

    return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LogoutResponse response) {
    nlohmann::json json;
    json["status"] = std::to_string(response.status);
    std::string serialized = json.dump();

    serialized = (char)ResponseCode::LogoutCode + getPaddedNumber(serialized.length(), 4) + serialized;
    std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

    return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);

	std::vector<nlohmann::json> jsonRooms;

	for (auto room : response.rooms) {
		nlohmann::json roomJson;

		roomJson["id"] = room.id;
		roomJson["name"] = room.name;
		roomJson["maxPlayers"] = room.maxPlayers;
		roomJson["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
		roomJson["timePerQuestion"] = room.timePerQuestion;

		jsonRooms.push_back(roomJson);
	}

	json["rooms"] = jsonRooms;
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::GetRoomsCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());
	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);

	std::string playersString = "[";
	for (auto player : response.players) {
		playersString +=  player + ",";
	}
	playersString = playersString.substr(0, playersString.length() - 1);

	json["players"] = playersString;
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::GetPlayersInRoomCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse response){
    nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::JoinRoomCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	json["roomId"] = std::to_string(response.roomId);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::CreateRoomCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);

	json["usernames"] = response.usernames;
	json["correctAnswers"] = response.correctAnswerCounters;
	json["averageAnswerTimes"] = response.averageAnswerTimes;


	std::string serialized = json.dump();
	serialized = (char)ResponseCode::GetHighScoreCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
ootput:
*	buffer of bytes representing the response
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(getPersonalStatusResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);

	//std::string statisticsString = "[";
	//for (auto statistic : response.statistics) {
	//	statisticsString += statistic + ",";
	//}
	//statisticsString = statisticsString.substr(0, statisticsString.length() - 1);
	//statisticsString += "]";

	json["statistics"] = response.statistics;
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::GetPersonalStatisctics + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Get a string representation of a number with a fixed number of digits.
input:
*	num - the number to represent
*	digits - the number of digits to represent
output:
* string representation of the number with a fixed number of digits
*/
std::string JsonResponsePacketSerializer::getPaddedNumber(int num, int digits) {
    std::ostringstream ostr;
    ostr << std::setw(digits) << std::setfill('0') << num;
    return ostr.str();
}


/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the response {"status": "0"}
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(StartGameResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::StartGameCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the response {"status": "0"}
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse response){
		nlohmann::json json;
		json["status"] = std::to_string(response.status);

		std::vector<nlohmann::json> jsonResults;

		for (auto result : response.results) {
			nlohmann::json resultJson;

			resultJson["averageAnswerTime"] = result.averageAnswerTime;
			resultJson["correctAnswerCount"] = result.correctAnswerCount;
			resultJson["username"] = result.username;
			resultJson["wrongAnswerCount"] = result.wrongAnswerCount;

			jsonResults.push_back(resultJson);
		}

		json["results"] = jsonResults;
		std::string serialized = json.dump();

		serialized = (char)ResponseCode::GetGameResultsCode + getPaddedNumber(serialized.length(), 4) + serialized;
		std::vector<unsigned char> buffer(serialized.begin(), serialized.end());
		std::cout << serialized << std::endl;
		return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the respons
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse response){
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	
	std::string serialized = json.dump(); 

	serialized = (char)ResponseCode::SubmitAnswerCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}


/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the respons
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetQuestionsResponse response){
	nlohmann::json json;
	json["status"] = std::to_string(response.status);

	json["answers"] = response.answers;

	json["question"] = response.question;
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::GetQuestionsCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}


/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the respons
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse response){
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::LeaveGameCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GoBackToMainMenuResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();
	serialized = (char)ResponseCode::MoveToMainMenuHandlerCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}




std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInGameResponse response) {
	nlohmann::json json; 
	json["status"] = std::to_string(response.status); 
	json["players"] = response.players;

	std::string serialized = json.dump(); 
	serialized = (char)ResponseCode::GetPlayersInGameCode + getPaddedNumber(serialized.length(), 4) + serialized; 
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end()); 

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetHowManyAnsweredResponse response) {
	nlohmann::json json;
	json["answered"] = std::to_string(response.answered);
	json["outOf"] = std::to_string(response.outOf);


	std::string serialized = json.dump();
	serialized = (char)ResponseCode::GetHowManyAnsweredCode + getPaddedNumber(serialized.length(), 4) + serialized; 
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(EndGameResponse response)
{
	nlohmann::json json;
	json["status"] = std::to_string(response.status); 

	std::string serialized = json.dump();
	serialized = (char)ResponseCode::EndGameCode + getPaddedNumber(serialized.length(), 4) + serialized; 
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end()); 
	return buffer; 
}














/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the response {"status": "0"}
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse response) {
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::LeaveRoomCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the response {"status": "0", "players": [], "hasGameBegun": "0", "questionCount": "0", "answerTimeout": "0"}
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse response){
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	json["players"] = response.players;
	json["hasGameBegun"] = response.hasGameBegun;
	json["questionCount"] = response.questionCount;
	json["answerTimeout"] = response.answerTimeout;
	
	std::string serialized = json.dump();
	serialized = (char)ResponseCode::GetRoomStateCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

/*
Serialize the response to a buffer of bytes.
input:
*	response - the response to serialize
output:
*	buffer of bytes representing the response {"status": "0"}
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse response){
	nlohmann::json json;
	json["status"] = std::to_string(response.status);
	std::string serialized = json.dump();

	serialized = (char)ResponseCode::CloseRoomCode + getPaddedNumber(serialized.length(), 4) + serialized;
	std::vector<unsigned char> buffer(serialized.begin(), serialized.end());

	return buffer;
}

