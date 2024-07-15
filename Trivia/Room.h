#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "LoggedUser.h"

struct RoomData {
	std::string name;
	unsigned int id;
	unsigned int maxPlayers;
	unsigned int timePerQuestion;
	unsigned int numOfQuestionsInGame;
	unsigned int isActive;
};

class Room {
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
public:
	
	Room(LoggedUser admin, RoomData metadata);
	int getId();
	void addUser(LoggedUser user);
	void removeUser(LoggedUser user);
	RoomData getMetadata();
	std::vector<std::string> getAllUsers();
	std::vector<LoggedUser> getLoggedUsers();
	void setRoomActiveStatus(unsigned int status);
};

