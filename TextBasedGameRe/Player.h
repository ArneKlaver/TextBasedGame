#pragma once
#include "Singleton.h"
#include <vector>

class Enemy;
class Room;

class Player
{
public:
	Player();
	~Player();

	void update();
	PlayerInfo& GetInfo() { return m_PlayerInfo; }
	void Atack(int target, vector<Enemy*> enemys);
	void TakeDamage(int damage);
	void AddSkillPoint(int skillPoints) { m_PlayerInfo.SkillPoints += skillPoints; };
	void SetCurrentRoom(Room* room) { m_pCurrentRoom = room; m_PlayerTarget = 1; } // also reset the standart target
	// bossroom
	Int2 GetPlayerPosition() { return m_Position; };
	void SetPlayerPosition(Int2 pos) { m_Position = pos; };
	// UPGRADES
	void UpgradeHealth(int amount);
	void UpgradeHealthRegen(int amount);
	void UpgradeDamage(int amount);
	void UpgradeAtackSpeed(int amount);
	void UpgradeCritChance(int amount);
	void upgradeCritDamage(int amount);

	void SetPlayerOnlineID(int playerOnlineID) { m_PlayerOnlineID = playerOnlineID; }
	void SetName(string name) { m_PlayerInfo.Name = name; }
	int GetPlayerOnlineID() { return m_PlayerOnlineID; }
	int GetFocus() { return m_PlayerTarget; } void SetFocus(int target) { m_PlayerTarget = target; }
private:

	int m_PlayerTarget = 1;
	PlayerInfo m_PlayerInfo;
	// bossroom
	Int2 m_Position = { 0,0 };
	float m_LeftoverAtackSpeed = 0;
	Room* m_pCurrentRoom = nullptr;
	int m_PlayerOnlineID = 0;
};
