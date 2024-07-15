#include "JsonRequestPacketDeserializer.h"

/*
Parse the buffer to a json object and extract the username and password from it
input:
*	buffer - vector of unsigned char
ootput:
*	LoginRequest object with username and password
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char>& buffer) {
    std::string jsonString = bufferToJsonString(buffer); // convert buffer to string
    const auto json = nlohmann::json::parse(jsonString.substr(5)); // parse string to json object
    std::string username = json.at("username"); // get username from json object
    std::string password = json.at("password"); // get password from json object

    return LoginRequest{ username, password }; // return LoginRequest object
}

/*
Parse the buffer to a json object and extract the username, password and email from it
input:
*	buffer - vector of unsigned char
ootput:
*	SignupRequest object with username, password and email
*/
SignupRequest JsonRequestPacketDeserializer::deserializerSignupRequest(std::vector<unsigned char>& buffer) {
    std::string jsonString = bufferToJsonString(buffer); // convert buffer to string

    const auto json = nlohmann::json::parse(jsonString.substr(5)); // parse string to json object
    std::string username = json.at("username"); // get username from json object
    std::string password = json.at("password"); // get password from json object
    std::string mail = json.at("mail"); // get email from json object

    return SignupRequest{ username, password, mail }; // return SignupRequest object

} 

/*
Parse the buffer to a json object and extract the correctAnswerId from it
input:
*	buffer - the buffer to parse
output:
*	SubmitAnswerRequest object with correctAnswerId
*/
SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(std::vector<unsigned char>& buffer){
    std::string jsonString = bufferToJsonString(buffer);

    const auto json = nlohmann::json::parse(jsonString.substr(5));
    unsigned int answerTime = json.at("answerTime");
    std::string answer = json.at("answer");
    return SubmitAnswerRequest{ answerTime ,answer };
}

GetPlayersInGameRequest JsonRequestPacketDeserializer::deserializeGetPlayersInGameRequest(std::vector<unsigned char>& buffer)
{
    std::string jsonString = bufferToJsonString(buffer);
    const auto json = nlohmann::json::parse(jsonString.substr(5));
    unsigned int gameId = json.at("gameId");

    return GetPlayersInGameRequest{ gameId };
}

GetHowManyAnsweredRequest JsonRequestPacketDeserializer::deserializeGetHowManyAnsweredRequest(std::vector<unsigned char>& buffer)
{
    std::string jsonString = bufferToJsonString(buffer);
    const auto json = nlohmann::json::parse(jsonString.substr(5));


    unsigned int questionNum = json.at("questionNum");


    return GetHowManyAnsweredRequest{ questionNum }; 
}

EndGameRequest JsonRequestPacketDeserializer::deserializeEndGameRequest(std::vector<unsigned char>& buffer)
{
    std::string jsonString = bufferToJsonString(buffer); 
    const auto json = nlohmann::json::parse(jsonString.substr(5)); 

    unsigned int gameId = json.at("gameId");
    return EndGameRequest{ gameId };
}




/*
Parse the buffer to a json object and extract the RoomId from it
input:
*	buffer - the buffer to parse
output:
*	JoinRoomRequest object with roomId
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(std::vector<unsigned char>& buffer) {
    std::string jsonString = bufferToJsonString(buffer); // convert buffer to string
    const auto json = nlohmann::json::parse(jsonString.substr(5)); // parse string to json object

    unsigned int roomId = json.at("roomId"); // get roomId from json object

    return GetPlayersInRoomRequest{ roomId }; // return GetPlayersInRoomRequest object
}

/*
Parse the buffer to a json object and extract the RoomId from it
input:
*	buffer - the buffer to parse
output:
*	JoinRoomRequest object with roomId
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char>& buffer){
    std::string jsonString = bufferToJsonString(buffer); // convert buffer to string
    const auto json = nlohmann::json::parse(jsonString.substr(5)); // parse string to json object

    unsigned int roomId = json.at("roomId"); // get roomId from json object

    return JoinRoomRequest{ roomId }; // return JoinRoomRequest object
}

/*
Parse the buffer to a json object and extract the roomName, maxUsers, questionCount and answerTimeout from it
input:
*	buffer - the buffer to parse
output:
*	CreateRoomRequest object with roomName, maxUsers, questionCount and answerTimeout
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char>& buffer){
    std::string jsonString = bufferToJsonString(buffer); // convert buffer to string
    const auto json = nlohmann::json::parse(jsonString.substr(5)); // parse string to json object

    std::string roomName = json.at("roomName"); // get roomName from json object
    unsigned int maxUsers = json.at("maxUsers"); // get maxUsers from json object
    unsigned int questionCount = json.at("questionCount"); // get questionCount from json object
    unsigned int answerTimeout = json.at("answerTimeout"); // get answerTimeout from json object


    return CreateRoomRequest{ roomName, maxUsers, questionCount, answerTimeout }; // return CreateRoomRequest object
}

/*
Convert the buffer to a string
input:
*	buffer - vector of unsigned char
output:
*	string
*/
std::string JsonRequestPacketDeserializer::bufferToJsonString(std::vector<unsigned char>& buffer) {
    std::string jsonData = ""; // create an empty string
    for (auto it = buffer.begin(); it != buffer.end(); it++) {
        jsonData += (char)*it; // convert buffer to string
    }
    return jsonData; // return the string
}