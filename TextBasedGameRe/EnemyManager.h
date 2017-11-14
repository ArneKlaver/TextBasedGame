#pragma once
#include "GUI.h"
#include "Room.h"
class Player;
class Enemy;

class EnemyManager
{
public:
	EnemyManager(Player** player);
	~EnemyManager();

	void Update();
	void AddEnemy(Enemy* enemy, Room* enemyRoom);
	void SetActiveRoom(Room** pActiveRoom) { m_pActiveRoom = pActiveRoom; }
	Enemy* GetEnemy(int enemyID){ return m_pEnemys.at(enemyID); }
	vector<Enemy*>* GetAllEnemys() { return &m_pEnemys; }
	void ReDrawEnemyInfo() { GUI::GetInstance()->WriteEnemyInfo((*m_pActiveRoom)->GetEnemys()); }
	void UpdateEnemy(int posInArray, int health);
	void ResetTotalEnemys() { m_TotalEnemys = 0; }
private:
	Player** m_pPlayer = nullptr;
	vector<Enemy*> m_pEnemys;
	Room** m_pActiveRoom = nullptr;
	int m_TotalEnemys = 0;
};
