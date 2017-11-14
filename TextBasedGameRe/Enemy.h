#pragma once

class Player;
class Room;

class Enemy
{
public:
	Enemy(Player* target , EnemyInfo info);
	~Enemy();
	EnemyInfo GetInfo() { return m_EnemyInfo; }
	virtual Int2 GetPosition() { return Int2(); }
	virtual void Update();
	virtual void SetTargetNumber(int targetNumber) { m_EnemyInfo.TargetNumber = targetNumber; }
	virtual void SetID(int id) { m_EnemyInfo.enemyID = id; }
	virtual void Atack();
	virtual void SetHealth(int health) { m_EnemyInfo.Health = health; };
	virtual void SetOnlineTarget(int targetID) { m_OnlineTargetID = targetID; }
	virtual void TakeDamage(int damage);

private:
	void CheckStates();
	int m_OnlineTargetID = 0;
	EnemyInfo m_EnemyInfo;
	Player* m_pTarget = nullptr;
	float m_LeftoverAtackSpeed = 0;
	bool m_CanAtack = true;
};
