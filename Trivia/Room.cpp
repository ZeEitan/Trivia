#include "Room.h"

/*
Room constructor
input:
	LoggedUser admin - the admin of the room
	RoomData metadata - the metadata of the room
output:
	None
*/
Room::Room(LoggedUser admin, RoomData metadata) : m_metadata(metadata){
	addUser(admin);
}

int Room::getId() {
	return this->m_metadata.id;
}

/*
Add a user to the room
input:
	LoggedUser user - the user to add
output:
	Void
*/
void Room::addUser(LoggedUser user) {
	if(m_users.size() >= m_metadata.maxPlayers)
		throw std::exception("Room is full");
	m_users.push_back(user);
}

/*
Remove a user from the room
input:
	LoggedUser user - the user to remove
ouyput:
	Void
*/
void Room::removeUser(LoggedUser user) {
	for (int i = 0; i < m_users.size(); i++) {
		if (m_users[i].getUsername() == user.getUsername()) {
			m_users.erase(m_users.begin() + i);
			return;
		}
	}
	throw std::exception("User not found");
}

/*
Get the metadata of the room
input:
	None
output:
	RoomData - the metadata of the room
*/
RoomData Room::getMetadata(){
	return this->m_metadata;
}

/*
Get all the users in the room
input:
	None
output:
	vector<string> - the usernames of all the users in the room
*/
std::vector<std::string> Room::getAllUsers() {
	std::vector<std::string> users;
	for (int i = 0; i < m_users.size(); i++) {
		users.push_back(m_users[i].getUsername());
	}
	return users;
}

std::vector<LoggedUser> Room::getLoggedUsers()
{
	return this->m_users;
}

void Room::setRoomActiveStatus(unsigned int status){
	this->m_metadata.isActive = status;
}

