// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <WinSock2.h>
#include <string>

#include <vector>

// TODO: reference additional headers your program requires here
using namespace std;
struct OnlinePlayer
{
	string name;
	int ID;
	int damageTaken;
};
struct EnemyBattleInfo
{
	int roomID;
	int enemyID;
	string enemyName;
	string playerName;
	int damage;
};
struct PlayerBattleReport
{
	std::vector<EnemyBattleInfo> enemysInfo;
};
struct PlayerBattleInfo
{
	int playerID;
	string name;
	string health;
};
struct EnemyBattleReport
{
	std::vector<PlayerBattleInfo> playersInfo;
};
struct Int2
{
	int x = 0;
	int y = 0;
	Int2(int X, int Y) :x(X), y(Y) {}
	Int2() :x(0), y(0) {}

};
struct action2
{
	std::string ActionCommand;

	int ActionID;
};

struct action
{
	std::string ActionCommand;
	vector<action2> SecondActionCommand;
	int ActionID;
};
struct actionArrays
{
	std::vector<action> m_ActionArr;
	std::vector<action> m_SecondActionArr;
	std::vector<action> m_ChatActionsArr;
};
struct States
{
	int Waiting = 1;
	int Stunned = 0;
	int Bleed = 0;
	int Slowed = 0;
};
struct PlayerInfo
{
	string Name = "Character";
	int Level = 1;
	int SkillPoints = 5;
	int MaxHealth = 100;
	int Health = 100;
	int HealthRegen = 0;
	int AtackDamage = 20; // normal atack damage
	float AtackSpeed = 1.5; 
	float MaxAtackSpeed = 3;
	float CritChance = 10; // x% crit chance
	float CritDamage = 2; // damage x2 (damage multiplier)
	bool IsAlive = true;
	// int health regen
};
struct EnemyInfo
{
	int enemyID;
	int TargetNumber = 0;
	string EnemyType = "";
	string Name = "Character";
	int Level = 1;
	int Health = 50;
	int AtackDamage = 20; // normal atack damage
	float AtackSpeed = 1; // MAX 3
	float CritChance = 0; // x% crit chance
	float CritDamage = 2; // damage x2 (damage multiplier)
	bool IsAlive = true;
	int MinGainedSkillPoints = 0;
	int MaxGainedSkillPoints = 0;
	States EnemyState;
};


enum SELECTEDWIDNOW { DEFAULT = 0 ,GAME = 1 , CHAT = 2 };