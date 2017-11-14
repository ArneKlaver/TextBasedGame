#pragma once
#include <string>
#include <vector>
#include "Enemy.h"
#include "item.h"

enum exitDirections { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };
enum roomTypes{NORMAL = 0,BOSS = 1};

class RoomDAO;

class Room
{
public:
	Room(int roomNumber, RoomDAO* roomDAO);
	virtual ~Room();

	virtual void Update();
	virtual std::vector< std::vector<int> >& GetRoomLayout() { return std::vector< std::vector<int> >{0}; };
	virtual Int2 GetStartPos() { return Int2{0,0}; };
	virtual void SetStartPos(Int2 pos) { ; };
	virtual void Draw() {};
	virtual bool IsEnemyAlive();
	virtual std::vector<Enemy*> GetEnemys() { return m_pEnemysInRoom; }
	virtual void AddEnemy(Enemy* enemy) { m_pEnemysInRoom.push_back(enemy); }

	virtual std::vector<Item*> GetItems() { return m_pItemsInRoom; }

	//std::string GetName() { return m_Name; };
	//std::vector<std::string>* GetExits() { return &m_ExitsArr; };
	//int GetRoomType() { return m_RoomType; };

	RoomDAO* m_pRoomDao = nullptr;
	int m_RoomNumber;
	std::vector<int> m_ExitsArr {0,0,0,0};  // -1 = none
	int m_RoomType = NORMAL;

private:
	//items , actions , enemys , ...
	std::vector<Enemy*> m_pEnemysInRoom;
	std::vector<Item*> m_pItemsInRoom;
};
