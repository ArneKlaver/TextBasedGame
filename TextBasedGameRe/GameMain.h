#pragma once
#include "Actions.h"
#include "FileParser.h"

class RoomDAO;
class Player;
class Input;
class Client;
class EnemyManager;
class ItemManager;


class GameMain
{
public:
	GameMain();
	~GameMain();
	void Update();
	bool CheckEndOfGame() const { return m_pActions->CheckEndOfGame(); }
	void LoadFloor(int floor);

private:
	RoomDAO* m_pRoomDao = nullptr;
	Player* m_pPlayer = nullptr;
	Actions* m_pActions = nullptr;
	Input* m_pInput = nullptr;
	Client* m_pClient = nullptr;
	EnemyManager* m_pEnemyManager = nullptr;
	ItemManager* m_pItemManager = nullptr;
	FileParser* m_pParser = nullptr;

	int FloorNumber = 0;
};

