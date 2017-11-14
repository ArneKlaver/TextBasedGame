#pragma once
#include "Room.h"


class NormalRoom: public Room
{
public:
	NormalRoom(int roomNumber, RoomDAO* roomDAO);
	~NormalRoom();

	void Draw();

private:

};
