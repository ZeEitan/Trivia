#include "RoomManager.h"

/*
Create a room
input:
*	admin - the admin of the room
*	metadata - the metadata of the room
* output:
* 	void
*/
void RoomManager::createRoom(LoggedUser admin, RoomData metadata){
	Room room(admin, metadata);

	if(metadata.maxPlayers <= 0)
		throw std::exception("Invalid room max users");

	if(metadata.numOfQuestionsInGame <= 0)
		throw std::exception("Invalid room question count");

	if(metadata.timePerQuestion <= 0)
		throw std::exception("Invalid room answer timeout");


	if (this->m_rooms.find(metadata.id) != this->m_rooms.end()) {
		throw std::exception("Room with this name already exists");
	}

	this->m_rooms.insert(std::pair<unsigned int, Room>(metadata.id, room));
}

/*
Delete the room by id
input:
*	id - the id of the room
output:
* 	void
*/
void RoomManager::deleteRoom(unsigned int id){
	std::map<unsigned int, Room>::iterator it = this->m_rooms.find(id);
	if (it != this->m_rooms.end()) {
		this->m_rooms.erase(it);
	}
	else
		throw std::exception("Room not found");
}

/*
Get the state of the room
input:
*	id - the id of the room
output:
* 	unsigned int - the state of the room the room active or not
*/
unsigned int RoomManager::getRoomState(unsigned int id){
	std::map<unsigned int, Room>::iterator it = this->m_rooms.find(id);
	if (it != this->m_rooms.end()){
		return it->second.getMetadata().isActive;
	}
	throw std::exception("Room not found");
}

/*
Get all the rooms
input:
*	None
output:
* 	vector<RoomData> - the metadata of all the rooms
*/
std::vector<RoomData> RoomManager::getRooms(){
	std::vector<RoomData> rooms;
	for (std::map<unsigned int, Room>::iterator it = m_rooms.begin(); it != m_rooms.end(); ++it) {
		if(it->second.getMetadata().isActive == false)
			rooms.push_back(it->second.getMetadata());
	}	

	return rooms;
}

/*
Get the room reference by id
input:
*	id - the id of the room
output:
* 	Room - the room
*/
Room& RoomManager::getRoom(unsigned int id) {
	std::map<unsigned int, Room>::iterator it = this->m_rooms.find(id);
	if (it != m_rooms.end()){
		std::cout << "-------------------" << it->second.getId() << std::endl;
		return it->second;
	}
	std::cout << "------------------- Room not found" << std::endl;
	throw std::exception("Room not found");

}

void RoomManager::setActiveStatus(unsigned int roomId, bool status)
{
	std::map<unsigned int, Room>::iterator it = this->m_rooms.find(roomId);
	if (it != m_rooms.end()) {
		std::cout << "-------------------" << it->second.getId() << std::endl;
		it->second.setRoomActiveStatus(status);
	}
}
