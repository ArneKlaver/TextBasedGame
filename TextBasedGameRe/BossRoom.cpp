#include "stdafx.h"
#include "BossRoom.h"
#include "Room.h"
#include <iostream>
#include "Player.h"
#include <algorithm>
#include "GUI.h"
#include "RoomDAO.h"
BossRoom::BossRoom(int roomNumber, RoomDAO* roomDAO):Room(roomNumber , roomDAO)
{
	m_pPlayer = m_pRoomDao->GetPlayerPtr();
}


BossRoom::~BossRoom()
{
}

void BossRoom::Update()
{
	UpdateRoom();
	DrawRoom();
}

void BossRoom::UpdateRoom()
{
	// reset room
	m_RoomLayout[m_LastPlayerPos.x][m_LastPlayerPos.y] = tileTypes::OPEN;

	std::for_each(m_LastEnemyPosArr.begin(), m_LastEnemyPosArr.end(), [this](Int2 tempPos) { m_RoomLayout[tempPos.x][tempPos.y] = tileTypes::OPEN; });

	//EnemyLocations
	for (Enemy* tempEnemy : GetEnemys())
	{
		if (tempEnemy != nullptr)
		{
			Int2 tempPos = tempEnemy->GetPosition();
			m_RoomLayout[(int)tempPos.x][(int)tempPos.y] = ENEMY;
			m_LastEnemyPosArr.push_back(tempPos);
		}
	}	
	//Player Location
	m_RoomLayout[m_pPlayer->GetPlayerPosition().x][m_pPlayer->GetPlayerPosition().y] = PLAYER;
	m_LastPlayerPos = Int2{ m_pPlayer->GetPlayerPosition().x , m_pPlayer->GetPlayerPosition().y };
}

void BossRoom::DrawRoom()
{
	for (int i = RoomSize.y -1 ; i >= 0; i--)
	{
		std::string tempString;
		for (int j=0; j < RoomSize.x; j++)
		{
			tempString.push_back(m_RoomLayout[j][i]);
			//std::cout << m_RoomLayout[j][i];
		}
		GUI::GetInstance()->Wr(tempString, 2);
	}
}

