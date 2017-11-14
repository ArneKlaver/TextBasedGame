#include "stdafx.h"
#include "Player.h"
#include "GUI.h"
#include "Enemy.h"
#include "ClientManager.h"
#include <algorithm>

Player::Player()
{
	m_PlayerInfo.Name = "guest";
	GUI::GetInstance()->WritePlayerInfo(m_PlayerInfo);
}

Player::~Player()
{

}

void Player::update()
{
	if (m_PlayerInfo.Health <= 0)
	{
		GUI::GetInstance()->ClearWindow(1);
		GUI::GetInstance()->Wr("YOUR ARE DEAD !!!");
		GUI::GetInstance()->Wr("respawning at startroom and u loose your unspend skillpoints");

		m_PlayerInfo.Health = 100;
		m_PlayerInfo.SkillPoints = 0;
		m_pCurrentRoom->m_pRoomDao->ResetRoomDrawPos();

		m_pCurrentRoom->m_pRoomDao->SetActiveRoom(m_pCurrentRoom->m_pRoomDao->GetRooms()->at(0));
	}
	GUI::GetInstance()->WritePlayerInfo(m_PlayerInfo);
	m_PlayerInfo.Health += m_PlayerInfo.HealthRegen;
	// if overhealing set the health back on max health
	if (m_PlayerInfo.Health > m_PlayerInfo.MaxHealth)
	{
		m_PlayerInfo.Health = m_PlayerInfo.MaxHealth;
	}
}
void Player::Atack(int target, vector<Enemy*> enemys)
{
	PlayerBattleReport report; // for online data
	target -= 1;
	if (enemys.size() <= 0)
	{
		GUI::GetInstance()->Wr("No enemys in the room !!!");
		return;
	}

	vector<Enemy*> enemysAlive;
	for_each(enemys.begin(), enemys.end(),
		[&](Enemy* enemy) {if (enemy->GetInfo().IsAlive) { enemysAlive.push_back(enemy); }});

	if ((unsigned)target >= enemysAlive.size() || target < 0)
	{
		GUI::GetInstance()->Wr("Wrong Target !!!");
		return;
	}
	if (enemysAlive[target]->GetInfo().IsAlive)
	{
		
		GUI::GetInstance()->LeaveOpenLine();

		for (int i = 0; i < int((m_PlayerInfo.AtackSpeed + m_LeftoverAtackSpeed)); i++)
		{
			//reset leftover atack when i is higher the it should be
			if (i > int(m_PlayerInfo.AtackSpeed - 1))
			{
				m_LeftoverAtackSpeed = -0.01f;
			}
			int damage = m_PlayerInfo.AtackDamage;
			int crit = rand() % 101;
			if (crit <= m_PlayerInfo.CritChance)
			{
				damage = int(damage * m_PlayerInfo.CritDamage);
				GUI::GetInstance()->WritePlayerDamage("CRITICAL STRIKE !");
			}
			GUI::GetInstance()->WritePlayerDamage(m_PlayerInfo.Name + " does " + to_string(damage) + " on target " + to_string(target + 1) + " atack " + to_string(i + 1));

			enemysAlive[target]->TakeDamage(damage);
	
			report.enemysInfo.push_back({ m_pCurrentRoom->m_RoomNumber , enemysAlive[target]->GetInfo().enemyID , enemysAlive[target]->GetInfo().Name , m_PlayerInfo.Name , damage });

		}
		m_LeftoverAtackSpeed += m_PlayerInfo.AtackSpeed - (int)m_PlayerInfo.AtackSpeed;
		// if playing online  send battle report to server
		if (ClientManager::GetInstance()->GetIsPlayingOnline())
		{
			ClientManager::GetInstance()->SendPlayerBattleReport(report);
		}
	}
}

void Player::TakeDamage(int damage)
{
	m_PlayerInfo.Health -= damage;
}

// ********** UPGRADES ************
void Player::UpgradeHealth(int amount)
{
	int upgrade = amount * 10;
	GUI::GetInstance()->Wr("Upgrading Health :  " + to_string(m_PlayerInfo.MaxHealth) + " -> " + to_string(m_PlayerInfo.MaxHealth + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.MaxHealth += upgrade;
	m_PlayerInfo.SkillPoints -= amount;
}

void Player::UpgradeHealthRegen(int amount)
{
	int upgrade = amount * 1;
	GUI::GetInstance()->Wr("Upgrading HealthRegen :   " + to_string(m_PlayerInfo.HealthRegen) + " -> " + to_string(m_PlayerInfo.HealthRegen + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.HealthRegen += amount * 1;
	m_PlayerInfo.SkillPoints -= amount;
}

void Player::UpgradeDamage(int amount)
{
	int upgrade = amount * 5;
	GUI::GetInstance()->Wr("Upgrading AtackDamage :  " + to_string(m_PlayerInfo.AtackDamage) + " -> " + to_string(m_PlayerInfo.AtackDamage + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.AtackDamage += amount * 2;
	m_PlayerInfo.SkillPoints -= amount;
}

void Player::UpgradeAtackSpeed(int amount)
{
	float upgrade = float(amount) * 0.2f;
	if ((m_PlayerInfo.AtackSpeed + float(amount) * 0.2f) > m_PlayerInfo.MaxAtackSpeed)
	{
		m_PlayerInfo.AtackSpeed = m_PlayerInfo.MaxAtackSpeed;
		GUI::GetInstance()->Wr("Max AtackSpeed reached !!!");
		return;
	}
	GUI::GetInstance()->Wr("Upgrading AtackSpeed :  " + to_string(m_PlayerInfo.AtackSpeed) + " -> " + to_string(m_PlayerInfo.AtackSpeed + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.AtackSpeed += float(amount) * 0.2f;
	m_PlayerInfo.SkillPoints -= amount;
}

void Player::UpgradeCritChance(int amount)
{
	int upgrade = amount * 1;
	if ((m_PlayerInfo.CritChance + amount * 1) > 100)
	{
		m_PlayerInfo.CritChance = m_PlayerInfo.MaxAtackSpeed;
		GUI::GetInstance()->Wr("Max AtackSpeed reached !!!");
		return;
	}
	GUI::GetInstance()->Wr("Upgrading CritChance :  " + to_string(m_PlayerInfo.CritChance) + " -> " + to_string(m_PlayerInfo.CritChance + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.CritChance += amount * 1;
	m_PlayerInfo.SkillPoints -= amount;
}

void Player::upgradeCritDamage(int amount)
{
	float upgrade = float(amount) * 0.1f;
	GUI::GetInstance()->Wr("Upgrade CritDamage :  " + to_string(m_PlayerInfo.CritDamage) + " -> " + to_string(m_PlayerInfo.CritDamage + upgrade));
	GUI::GetInstance()->Wr("SkillPoints left :  " + to_string(m_PlayerInfo.SkillPoints) + " -> " + to_string(m_PlayerInfo.SkillPoints - amount));
	m_PlayerInfo.CritDamage += float(amount) * 0.1f;
	m_PlayerInfo.SkillPoints -= amount;
}

