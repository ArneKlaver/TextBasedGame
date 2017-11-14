#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "GUI.h"
#include "ClientManager.h"

Enemy::Enemy(Player* target , EnemyInfo info) :m_pTarget(target) ,m_EnemyInfo(info)
{

}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (m_EnemyInfo.Health <= 0)
	{
		m_EnemyInfo.IsAlive = false;
		return;
	}
	// check states
	CheckStates();
}

void Enemy::TakeDamage(int damage)
{
	m_EnemyInfo.Health -= damage;
	if (m_EnemyInfo.Health <= 0 && m_EnemyInfo.IsAlive)
	{
		m_EnemyInfo.IsAlive = false;
		GUI::GetInstance()->WriteEnemyDamage(m_EnemyInfo.Name + " " + to_string(m_EnemyInfo.TargetNumber) + " is dead ");
		int points;
		if (GetInfo().MaxGainedSkillPoints == GetInfo().MinGainedSkillPoints)
		{
			points = GetInfo().MaxGainedSkillPoints;
		}
		else {
				points = (rand() % (GetInfo().MaxGainedSkillPoints - GetInfo().MinGainedSkillPoints)) + GetInfo().MinGainedSkillPoints;
			}
		m_pTarget->AddSkillPoint(points);	
	}
}
void Enemy::Atack()
{
	if (m_CanAtack)
	{
		for (size_t i = 0; i < (unsigned int)(m_EnemyInfo.AtackSpeed + m_LeftoverAtackSpeed); i++)
		{
			//reset leftover atack when i is higher the it should be
			if (i > (unsigned int)m_EnemyInfo.AtackSpeed - 1)
			{
				m_LeftoverAtackSpeed = 0;
			}
			int damage = m_EnemyInfo.AtackDamage;
			int crit = rand() % 101;
			if (crit <= m_EnemyInfo.CritChance)
			{
				damage = int(damage *m_EnemyInfo.CritDamage);
				if (!ClientManager::GetInstance()->GetIsPlayingOnline())
				{
					GUI::GetInstance()->WriteEnemyDamage(m_EnemyInfo.Name + " " + to_string(m_EnemyInfo.TargetNumber) + " does " + to_string(damage) +
						" damage on " + m_pTarget->GetInfo().Name + "   CRITICAL STRIKE");
				}
			}
			else {
				if (!ClientManager::GetInstance()->GetIsPlayingOnline())
				{
					GUI::GetInstance()->WriteEnemyDamage(m_EnemyInfo.Name + " " + to_string(m_EnemyInfo.TargetNumber) + " does " + to_string(damage) +
						" damage on " + m_pTarget->GetInfo().Name);
				}
			}

			if (ClientManager::GetInstance()->GetIsPlayingOnline())
			{
				vector<OnlinePlayer>* pPlayersArr = ClientManager::GetInstance()->GetOnlinePlayersInRoom();

				int pos = 0;
				if (pPlayersArr->size() > 0)
				{
					pos = rand() % pPlayersArr->size();
					pPlayersArr->at(pos).damageTaken += damage;
				}
				else
				{
					GUI::GetInstance()->Wr("ERROR void Enemy::Atack()");
				}
			}
			else {m_pTarget->TakeDamage(damage);}
		}
		m_LeftoverAtackSpeed += m_EnemyInfo.AtackSpeed - (int)m_EnemyInfo.AtackSpeed;
	}
}
void Enemy::CheckStates()
{
	m_CanAtack = true;
	if (m_EnemyInfo.EnemyState.Waiting > 0)
	{
		m_EnemyInfo.EnemyState.Waiting--;
		m_CanAtack = false;
	}
}