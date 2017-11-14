#include "stdafx.h"
#include "RoomDAO.h"
#include "Room.h"
#include <iostream>
#include "BossRoom.h"
#include "Player.h"
#include "GUI.h"
#include "NormalRoom.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Enemy.h"
#include "ClientManager.h"

RoomDAO::RoomDAO(Player* player , EnemyManager* enemyManager , ItemManager* itemManager) 
		: m_pPlayer(player) , m_pEnemyManager(enemyManager) , m_pItemManager(itemManager)
{
	ResetRoomDrawPos();
}


RoomDAO::~RoomDAO()
{
	for (size_t i = 0; i < m_RoomsArr.size(); i++)
	{
		delete m_RoomsArr[i];
		m_RoomsArr[i] = nullptr;
	}
	
}
void RoomDAO::ResetRoomDrawPos()
{
	m_RoomDrawPos = GUI::GetInstance()->GetFramePos(2);
	Int2 dimensions = GUI::GetInstance()->GetFrameDimensions(2);
	m_pEnemyManager->SetActiveRoom(&m_pActiveRoom);

	m_RoomDrawPos.x += dimensions.x / 2;
	m_RoomDrawPos.y += dimensions.y / 2;
	m_PrevRoomDrawPos = m_RoomDrawPos;
}

void RoomDAO::Update()
{
	 m_pActiveRoom->Update();
	 m_pActiveRoom->Draw(); 
}

void RoomDAO::MoveToRoom(int dir)
{
	// is er een weg?
	if (m_pActiveRoom->m_ExitsArr[dir] == 0) { GUI::GetInstance()->Wr("can't go this way"); return; }

	for (Room* tempRoomPtr : m_RoomsArr)
	{
		if (tempRoomPtr->m_RoomNumber == m_pActiveRoom->m_ExitsArr[dir])
		{
			m_PrevRoomDrawPos = m_RoomDrawPos;
			SetActiveRoom(tempRoomPtr);
			if (tempRoomPtr->m_RoomNumber == m_RoomsArr.size()-1)
			{
				ClientManager::GetInstance()->NextFloor();
				return;
			}
			if (ClientManager::GetInstance()->GetIsPlayingOnline())
			{
				ClientManager::GetInstance()->SetIsMyTurn(false);
				GUI::GetInstance()->WrPlayerTurn("Waiting for other players");
				ClientManager::GetInstance()->MoveToRoom(tempRoomPtr->m_RoomNumber , tempRoomPtr->IsEnemyAlive());
			}				
				switch (dir)
				{
				case NORTH:
					m_RoomDrawPos.y-=2;
					break;
				case SOUTH:
					m_RoomDrawPos.y+=2;
					break;
				case EAST:
					m_RoomDrawPos.x+=3;
					break;
				case WEST:
					m_RoomDrawPos.x-=3;
					break;
				}
				if (tempRoomPtr->m_RoomType == BOSS)
				{
					m_pPlayer->SetPlayerPosition(tempRoomPtr->GetStartPos());
				}
				return;
		}
	}
}

void RoomDAO::MoveInRoom(int dir , int moveAmount)
{
	for (int i = 0; i < moveAmount; i++)
	{
		std::vector< std::vector<int> > roomLayour = m_pActiveRoom->GetRoomLayout();
		Int2 playerLoc = m_pPlayer->GetPlayerPosition();
		switch (dir)
		{
		case exitDirections::NORTH:
			playerLoc.y += 1;
			if (roomLayour[playerLoc.x][playerLoc.y] != tileTypes::OPEN)
			{
				playerLoc.y -= 1;
				GUI::GetInstance()->Wr("cant move to that position Tile is not accessible");
				return;
			}
			break;
		case exitDirections::EAST:
			playerLoc.x += 1;
			if (roomLayour[playerLoc.x][playerLoc.y] != tileTypes::OPEN)
			{
				playerLoc.x -= 1;
				GUI::GetInstance()->Wr("cant move to that position Tile is not accessible");
				return;
			}
			break;
		case exitDirections::SOUTH:
			playerLoc.y -= 1;
			if (roomLayour[playerLoc.x][playerLoc.y] != tileTypes::OPEN)
			{
				playerLoc.y += 1;
				GUI::GetInstance()->Wr("cant move to that position Tile is not accessible");
				return;
			}
			break;
		case exitDirections::WEST:
			playerLoc.x -= 1;
			if (roomLayour[playerLoc.x][playerLoc.y] != tileTypes::OPEN)
			{
				playerLoc.x += 1;
				GUI::GetInstance()->Wr("cant move to that position Tile is not accessible");
				return;
			}
			break;
		}
		m_pPlayer->SetPlayerPosition(playerLoc);
		GUI::GetInstance()->Wr(std::to_string(m_pPlayer->GetPlayerPosition().x) + std::to_string(m_pPlayer->GetPlayerPosition().y));
	}
}

void RoomDAO::SetActiveRoom(Room* room)
{
	m_pPrevRoom = m_pActiveRoom;
	m_pActiveRoom = room;
	m_pPlayer->SetCurrentRoom(m_pActiveRoom);
};