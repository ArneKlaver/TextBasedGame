#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Room.h"
#include <algorithm>
#include "GUI.h"

EnemyManager::EnemyManager(Player** player) : m_pPlayer(player)
{
}

EnemyManager::~EnemyManager()
{
	for (size_t i = 0; i < m_pEnemys.size(); i++)
	{
		delete m_pEnemys[i];
		m_pEnemys[i] = nullptr;
	}
}

void EnemyManager::Update()
{
	//vector<Enemy*>* enemys = &(*m_pActiveRoom)->GetEnemys();
	vector<Enemy*> enemys = (*m_pActiveRoom)->GetEnemys();
	for_each(enemys.begin(), enemys.end(),
		[&](Enemy* enemy) {if (enemy->GetInfo().IsAlive) { enemy->Update(); } });
	GUI::GetInstance()->LeaveOpenLine();

	for_each(enemys.begin(), enemys.end(),
		[&](Enemy* enemy) {if (enemy->GetInfo().IsAlive) { enemy->Atack(); } });
}

void EnemyManager::AddEnemy(Enemy* enemy, Room* enemyRoom)
{
	enemy->SetID(m_TotalEnemys);
	m_TotalEnemys++;
	m_pEnemys.push_back(enemy);
	enemyRoom->AddEnemy(enemy); 
}

void EnemyManager::UpdateEnemy(int posInArray, int health)
{
	m_pEnemys.at(posInArray)->SetHealth(health);
}

