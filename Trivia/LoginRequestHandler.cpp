#include "LoginRequestHandler.h"

/*
LoginRequestHandler constructor 
input:
*	factory - the request handler factory
output:
*	void
*/
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) : 
	IRequestHandler(), m_handlerFactory(factory), m_loginManager(factory.getLoginManager()) {}

/*
Checks if the request is relevant to the handler
input:
*	info - the request info
output:
*	true if the request is relevant
*   false if the request is not relevant
*/
bool LoginRequestHandler::isRequestRelevent(RequestInfo info) {
    return info.id == RequestCode::requestLoginCode || info.id == RequestCode::requestSingupCode;
}

/*
Handles the request and returns the result of the request 
input:
*	info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult LoginRequestHandler::RequestHandler(RequestInfo info, const SOCKET socket) {
	if (!isRequestRelevent(info)) {
		std::cout << "Error: You need to signup or Login" << std::endl;
		ErrorResponse error{ "You need to signup or Login" }; // TODO: return error
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse(error), nullptr };
	}

	if (info.id == RequestCode::requestLoginCode)
		return login(info, socket);
	return signup(info);
}

/*
Handles the login request and returns the result of the request
input:
*	info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult LoginRequestHandler::login(RequestInfo info, const SOCKET socket) {
	std::vector<unsigned char> buffer;
	IRequestHandler* newHandler = nullptr;
	LoggedUser user;
	LoginRequest userInfo = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	std::cout << "username: " << userInfo.username << ", password: " << userInfo.password << std::endl;

	std::string error;

	if (m_loginManager.login(userInfo.username, userInfo.password, error, socket)) {
		std::cout << "Login success" << std::endl;
		buffer = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ 1 });
		user = LoggedUser(userInfo.username, socket);
		return RequestResult{ buffer, this->m_handlerFactory.createMenuRequestHandler(user)};
	}

	std::cout << "Login failed" << std::endl;
	buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ error });
	return RequestResult{ buffer, nullptr };
}
	
/*
Handles the signup request and returns the result of the request
input:
*	info - the request info
output:
*	RequestResult - the result of the request
*/
RequestResult LoginRequestHandler::signup(RequestInfo info) {
	std::vector<unsigned char> buffer;
	IRequestHandler* newHandler = nullptr;
	LoggedUser user;

	SignupRequest userInfo = JsonRequestPacketDeserializer::deserializerSignupRequest(info.buffer);
	std::cout << "username: " << userInfo.username << ", password: " << userInfo.password << std::endl;

	if (m_loginManager.signup(userInfo.username, userInfo.password, userInfo.email)) {
		std::cout << "Signup success" << std::endl;
		buffer = JsonResponsePacketSerializer::serializeResponse(SignupResponse{ 1 });
		newHandler = nullptr; //Need to be create new menu handler here
		return RequestResult{ buffer, newHandler };
	}

	std::cout << "Signup failed" << std::endl;
	buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Signup failed" });
	return RequestResult{ buffer, nullptr };
}
