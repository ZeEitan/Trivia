#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include <string>
#include <vector>
#include <iostream>

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler {
public:
	LoginRequestHandler(RequestHandlerFactory& factory);
	virtual bool isRequestRelevent(RequestInfo info) override;
	virtual RequestResult RequestHandler(RequestInfo info, const SOCKET socket) override;
private:
	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;
	RequestResult login(RequestInfo info, const SOCKET socket);
	RequestResult signup(RequestInfo info);
};

