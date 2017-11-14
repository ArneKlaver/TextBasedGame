#pragma once
#include "Room.h"
#include "Player.h"

class EnemyManager;
class ItemManager;

class RoomDAO
{
public:
	RoomDAO(Player* player, EnemyManager* enemyManager, ItemManager* itemManager);
	~RoomDAO();

	void Update();
	//void LoadRoomData();
	std::vector<Room*>* GetRooms() { return &m_RoomsArr; };
	void MoveToRoom(int dir);
	void MoveInRoom(int dir , int moveAmount);
	void SetActiveRoom(Room* room);

	Room* GetActiveRoom() { return m_pActiveRoom; }
	Int2 GetDrawPos() { return m_RoomDrawPos; }
	Int2 GetPrevRoomDrawPos() { return m_PrevRoomDrawPos; }
	Player* GetPlayerPtr() { return m_pPlayer; }
	void SetRoomsArr(std::vector<Room*>& rooms) { m_RoomsArr = rooms; m_pActiveRoom = m_RoomsArr.at(0); };
	void ResetRoomDrawPos();
private:
	std::vector<Room*> m_RoomsArr;
	Player* m_pPlayer = nullptr;
	Room* m_pActiveRoom = nullptr;
	Room* m_pPrevRoom = nullptr;
	Int2 m_RoomDrawPos = { 0 , 0 };
	Int2 m_PrevRoomDrawPos = { 0 , 0 };
	EnemyManager* m_pEnemyManager = nullptr;
	ItemManager* m_pItemManager = nullptr;
};

