#pragma once
#include <iostream>
#include <string>
#include "LoggedUser.h"
#include "SqliteDatabase.h"
#include <vector>

class LoginManager {
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

public:
	LoginManager(IDatabase* database);
	bool signup(std::string username, std::string password, std::string mail);
	bool login(std::string username, std::string password, std::string& error, const SOCKET socket);
	bool logout(std::string username);
};

