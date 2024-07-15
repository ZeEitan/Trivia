#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

#include "Room.h"



class RoomManager {
private:
	std::map<unsigned int, Room> m_rooms;

public:
	RoomManager() = default;
	void createRoom(LoggedUser admin, RoomData metadata);
	void deleteRoom(unsigned int id);
	unsigned int getRoomState(unsigned int id);
	std::vector<RoomData> getRooms();
	Room& getRoom(unsigned int id);
	void setActiveStatus(unsigned int roomId, bool status);
};

