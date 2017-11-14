#include "stdafx.h"
#include "ClientManager.h"
#include "Client.h"
#include "Enemy.h"
#include "RoomDAO.h"
#include "GameMain.h"
ClientManager::ClientManager()
{
}


ClientManager::~ClientManager()
{
}

void ClientManager::Join()
{
	m_pClient->SendGameData(P_JoinGame);
	m_pClient->SendNameChange(m_pRoomDAO->GetPlayerPtr()->GetInfo().Name);
}

void ClientManager::Leave()
{
	m_pClient->SendGameData(P_LeaveGame);
}
void ClientManager::PlayerTurnDone(bool enemyAlive)
{
	if (!m_pClient->SendGameData(P_PlayerTurn))
		return;
	if (!m_pClient->SendInt32(enemyAlive))
		return;
}
void ClientManager::EnemyTurnDone()
{
	m_pClient->SendGameData(P_EnemyTurn);
}
void ClientManager::MoveToRoom(int roomNumber , bool hasEnemys)
{
	m_pClient->SendGameData(P_PlayerMoveToRoom);
	if (!m_pClient->SendInt32(roomNumber))
		return ;
}
void ClientManager::Disconnect()
{
	if (m_MyTurn)
	{
		m_pClient->SendGameData(P_PlayerTurn);
	}
	m_pClient->SendGameData(P_CloseConnection);
}


// battle reports
void ClientManager::SendPlayerBattleReport(PlayerBattleReport playerReport)
{
	binn* data = binn_object();
	binn_object_set_int32(data, "enemysInfoSize", playerReport.enemysInfo.size());
	binn_object_set_int32(data, "RoomNumber", playerReport.enemysInfo.at(0).roomID);
	for (size_t i = 0; i < playerReport.enemysInfo.size(); i++)
	{
		binn_object_set_int32(data, (char*)string(("enemyID") + i).c_str() , playerReport.enemysInfo.at(i).enemyID);
		binn_object_set_str(data, (char*)string(("playerName") + i).c_str() , (char*)playerReport.enemysInfo.at(i).playerName.c_str());
		binn_object_set_str(data, (char*)string(("enemyName") + i).c_str() , (char*)playerReport.enemysInfo.at(i).enemyName.c_str());
		binn_object_set_int32(data, (char*)string(("damage") + i).c_str() , playerReport.enemysInfo.at(i).damage);
	}
	

	m_pClient->SendBinn(P_PlayerBattleReport , data);
	binn_free(data);
}

void ClientManager::RecvPlayerBattleReport()
{
	void* data = nullptr;
	m_pClient->GetBinn(data);
	binn* dataObj;
	dataObj = binn_open(data);
	int size = binn_object_int32(dataObj, "enemysInfoSize");
	int roomNumber = binn_object_int32(dataObj, "RoomNumber");
	// see if player is in the same room as the recieved battle report
	bool isInSameRoom = roomNumber == m_pRoomDAO->GetActiveRoom()->m_RoomNumber;
	for (int i = 0; i < size; i++)
	{
		int id = binn_object_int32(dataObj, (char*)string(("enemyID") + i).c_str());
		string playerName = binn_object_str(dataObj, (char*)string(("playerName") + i).c_str());
		string enemyName = binn_object_str(dataObj, (char*)string(("enemyName") + i).c_str());
		int damage = binn_object_int32(dataObj, (char*)string(("damage") + i).c_str());
		m_pEnemyManager->GetEnemy(id)->TakeDamage(damage);
		if (!m_pEnemyManager->GetEnemy(id)->GetInfo().IsAlive)
		{
			m_pEnemyManager->ReDrawEnemyInfo();
		}
		if (isInSameRoom)
		{
			GUI::GetInstance()->WritePlayerDamage(playerName + " does " + to_string(damage) + " on target " +
				to_string(m_pEnemyManager->GetEnemy(id)->GetInfo().TargetNumber) + " atack " + to_string(1));
		}	
	}
	// update screen info
	if (isInSameRoom)
	{
		m_pEnemyManager->ReDrawEnemyInfo();
		//GUI::GetInstance()->ClearEnemyDamage();
	}
	binn_free(dataObj);
}

void ClientManager::SendEnemyBattleReport()
{
	binn* data = binn_object();
	size_t size = m_onlinePlayersInRoom.size();
	binn_object_set_int32(data, "AmountOfPlayersInRoom", size);
	for (size_t i = 0; i < size; i++)
	{
		binn_object_set_str(data, (char*)string(("name") + i).c_str(), (char*)m_onlinePlayersInRoom.at(i).name.c_str());
		binn_object_set_int32(data, (char*)string(("playerID") + i).c_str(), m_onlinePlayersInRoom.at(i).ID);
		binn_object_set_int32(data, (char*)string(("damageTaken") + i).c_str(), m_onlinePlayersInRoom.at(i).damageTaken);
	}

	SetIsEnemyTurn(false);

	m_pClient->SendBinn(P_EnemyBattleReport, data);
	binn_free(data);
}
void ClientManager::RecvEnemyBattleReport()
{
	GUI::GetInstance()->LeaveOpenLine();

	void* data = nullptr;
	m_pClient->GetBinn(data);
	binn* dataObj;
	dataObj = binn_open(data);
	int size = binn_object_int32(dataObj, "AmountOfPlayersInRoom");
	// see if player is in the same room as the recieved battle report
	for (int i = 0; i < size; i++)
	{
		string name = binn_object_str(dataObj, (char*)string(("name") + i).c_str());
		int id = binn_object_int32(dataObj, (char*)string(("playerID") + i).c_str());
		int damage = binn_object_int32(dataObj, (char*)string(("damageTaken") + i).c_str());

		if (damage > 0)
		{
			if (id == m_pRoomDAO->GetPlayerPtr()->GetPlayerOnlineID())
			{
				m_pRoomDAO->GetPlayerPtr()->TakeDamage(damage);
				GUI::GetInstance()->WriteEnemyDamage("YOU take " + to_string(damage) + " damage");
			}
			else {
				GUI::GetInstance()->WriteEnemyDamage(name + ": takes " + to_string(damage) + " damage");
			}
		}
	}
	// update screen info
	GUI::GetInstance()->WritePlayerInfo(m_pRoomDAO->GetPlayerPtr()->GetInfo());

	binn_free(dataObj);

}
// server data
void ClientManager::RecvFloorData()
{

	void* data = nullptr;
	m_pClient->GetBinn(data);
	binn* dataObj;
	dataObj = binn_open(data);
	int newFloorNumber = binn_object_int32(dataObj, "FloorNumber");
	// load correct floor
	if (newFloorNumber != m_FloorNumber)
	{
		m_FloorNumber = newFloorNumber;
		m_pMainGame->LoadFloor(m_FloorNumber);
	}
	int size = binn_object_int32(dataObj, "enemysInfoSize");
	// see if player is in the same room as the recieved battle report
	for (int i = 0; i < size; i++)
	{
		int id = binn_object_int32(dataObj, (char*)string(("enemyID") + i).c_str());
		int posInArray = binn_object_int32(dataObj, (char*)string(("posInArray") + i).c_str());
		int enemyHealth = binn_object_int32(dataObj, (char*)string(("enemyHealth") + i).c_str());

		m_pEnemyManager->UpdateEnemy(posInArray , enemyHealth);

	}
	binn_free(dataObj);
}
void ClientManager::SendFloorData()
{
	binn* data = binn_object();
	size_t size = m_pEnemyManager->GetAllEnemys()->size();
	binn_object_set_int32(data, "FloorNumber", m_FloorNumber);
	binn_object_set_int32(data, "enemysInfoSize", size);
	for (size_t i = 0; i < size ; i++)
	{
		binn_object_set_int32(data, (char*)string(("enemyID") + i ).c_str()  , m_pEnemyManager->GetEnemy(i)->GetInfo().enemyID);
		binn_object_set_int32(data, (char*)string("posInArray" + i).c_str(), i);
		binn_object_set_int32(data, (char*)string("enemyHealth" + i).c_str(), m_pEnemyManager->GetEnemy(i)->GetInfo().Health);
	}

	m_pClient->SendBinn(P_FloorData, data);
	binn_free(data);
}

void ClientManager::RecvEnemyTurn()
{
	SetIsEnemyTurn(true);

	m_onlinePlayersInRoom.clear();

	void* data = nullptr;
	m_pClient->GetBinn(data);
	binn* dataObj;
	dataObj = binn_open(data);

	int size = binn_object_int32(dataObj, "AmountOfPlayersInRoom");
	if (size == 0)
	{
		GUI::GetInstance()->Wr("ClientManager::RecvEnemyTurn()  Error no players in room ");
	}
	for (int i = 0; i < size; i++)
	{
		m_onlinePlayersInRoom.push_back({binn_object_str(dataObj , (char*)std::string(("PlayerName") + i).c_str()), binn_object_int32(dataObj,(char*)string(("playerID") + i).c_str()) , 0});
	}	
	GUI::GetInstance()->WrOnReservedSpot("Enemy turn", 2, true);

}

void ClientManager::ChangeName(string newName)
{
	if (m_PlayingOnline)
	{
		m_pClient->SendNameChange(newName);
	}
}

void ClientManager::NextFloor()
{
	if (m_PlayingOnline)
	{
		m_pClient->SendPacketType(P_NextFloor);
		m_pClient->SendInt32(m_FloorNumber + 1);
		m_pMainGame->LoadFloor(m_FloorNumber + 1);
		m_FloorNumber++;
	}
	else { m_pMainGame->LoadFloor(m_FloorNumber+1); m_FloorNumber++;	}
}
void ClientManager::RecvNextFloor()
{

	int floorNumber = 0;
	if (!m_pClient->GetInt32(floorNumber))
		return ;

	m_pMainGame->LoadFloor(floorNumber);

}