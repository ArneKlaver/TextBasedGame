#pragma once
#include "Room.h"
#include "ItemManager.h"
#include "EnemyManager.h"

class FileParser
{
public:
	FileParser(Player* player, EnemyManager* enemyManager, ItemManager* itemManager, RoomDAO* roomDAO);
	~FileParser();

	std::vector<Room*> GetRoomsDataFromFille();
	actionArrays GetActionDataFromFille();
	std::vector<EnemyInfo>& GetEnemyDataFromFile();
	void GetFolderNames();
	bool SetFloor(int floorNumber);
private:
	vector<string> FilterString(std::string& text, char seperation);
	string CreateSubstrBetween(string text, std::string first, std::string second);
	
	std::vector<EnemyInfo> m_EnemyInfoArr;
	RoomDAO* m_pRoomDAO = nullptr;
	Player* m_pPlayer = nullptr;
	EnemyManager* m_pEnemyManager = nullptr;
	ItemManager* m_pItemManager = nullptr;

	vector<string> m_FolderNames;
	string filePath;
	int m_CurrentFolder = -1;
};

