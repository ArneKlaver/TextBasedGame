#pragma once
#include "GUI.h"
#include "EnemyManager.h"
#include "RoomDAO.h"
class Client;
class RoomDAO;
class GameMain;
class ClientManager : public Singleton<ClientManager>
{
public:
	ClientManager();
	~ClientManager();


	void Join();
	void Leave();
	void PlayerTurnDone(bool enemyAlive);
	void EnemyTurnDone();
	void Disconnect();

	// battle reports
	void SendPlayerBattleReport(PlayerBattleReport playerReport);
	void RecvPlayerBattleReport();
	void SendEnemyBattleReport();
	void RecvEnemyBattleReport();
	void SendFloorData();
	void RecvFloorData();
	void RecvEnemyTurn();
	void RecvNextFloor();
	// move to room
	void MoveToRoom(int roomNumber, bool hasEnemys);

	void ChangeName(string newName);
	void NextFloor();
	// setters / getters	
		void SetClientPtr(Client* client) { m_pClient = client; }
		void SetEnemyManagerPtr(EnemyManager* enemyManager) { m_pEnemyManager = enemyManager; }
		void SetRoomDAOPtr(RoomDAO* roomDAO) { m_pRoomDAO = roomDAO; }
		void SetIsPlayingOnline(bool isPlayingOnline) { m_PlayingOnline = isPlayingOnline; }
		void SetIsMyTurn(bool isMyTurn) { m_MyTurn = isMyTurn; }
		void SetIsEnemyTurn(bool isEnemyTurn) { m_EnemyTurn = isEnemyTurn; }
		void SetPlayerAmount(int playerAmount) { m_PlayerAmount = playerAmount; }
		void SetIsFirstPlayer(bool isFirstPlayer) { m_IsFirstPlayer = isFirstPlayer; }
		void SetOnlineID(int onlineID) { m_pRoomDAO->GetPlayerPtr()->SetPlayerOnlineID(onlineID); }
		bool GetIsPlayingOnline() { return m_PlayingOnline; }
		bool GetIsMyTurn() { return m_MyTurn; }
		bool GetIsEnemyTurn() { return m_EnemyTurn; }
		int GetPlayerAmount() { return m_PlayerAmount; }
		bool GetIsFirstPlayer() { return  m_IsFirstPlayer; }
		void SetFloorNumber(int floorNumber) {m_FloorNumber = floorNumber; }
		void SetGameMain(GameMain* mainGame) { m_pMainGame = mainGame; };

		vector<OnlinePlayer>* GetOnlinePlayersInRoom() { return &m_onlinePlayersInRoom; }
private:
	Client* m_pClient = nullptr;
	EnemyManager* m_pEnemyManager = nullptr;
	RoomDAO* m_pRoomDAO = nullptr;
	GameMain* m_pMainGame = nullptr;
	bool m_MyTurn = true;
	int m_PlayerAmount = 1;
	bool m_PlayingOnline = false;
	bool m_IsFirstPlayer = false;
	bool m_EnemyTurn = false;
	int m_FloorNumber = 0;
	vector<OnlinePlayer> m_onlinePlayersInRoom;
};

