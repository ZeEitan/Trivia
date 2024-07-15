#pragma once
#include<string>
#include<iostream>

#include <WinSock2.h>
#include <Windows.h>

class LoggedUser
{
private:
	std::string m_username;
	SOCKET _socket;

public:
	LoggedUser() = default;
	LoggedUser(std::string username, SOCKET socket);

	std::string getUsername() const;
	SOCKET getSocket() const;
	
	bool operator<(const LoggedUser& other) const {
		//funtion for compilation
		//return this->m_username.length() < other.m_username.length();
		return this->m_username < other.m_username;
	}

	bool operator==(const LoggedUser user) const
	{
		//funtion for compilation
		return this->m_username == user.m_username;
	}

};

