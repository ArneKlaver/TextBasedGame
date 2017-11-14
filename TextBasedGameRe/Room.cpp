#include "stdafx.h"
#include "Room.h"
#include <algorithm>
#include "GUI.h"


Room::Room(int roomNumber, RoomDAO* roomDAO)
{
	m_RoomNumber = roomNumber; 
	m_pRoomDao = roomDAO; 
}

Room::~Room()
{

}


bool Room::IsEnemyAlive()
{
	bool isAlive = false;
	for_each(m_pEnemysInRoom.begin(), m_pEnemysInRoom.end(),
		[&](Enemy* enemy)
	{
		if (enemy->GetInfo().IsAlive) { isAlive = true; }
	});

	return isAlive;
};
void Room::Update()
{
	string names;
	string health;

	if (IsEnemyAlive())
	{
		GUI::GetInstance()->WrOnReservedSpot("Enemy Encounter : " + to_string(m_pEnemysInRoom.size()) + " enemy's spotted !!!" , 1);
		GUI::GetInstance()->WriteEnemyInfo(m_pEnemysInRoom);
	}

}
