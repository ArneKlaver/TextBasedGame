#include "stdafx.h"
#include "FileParser.h"
#include <iostream>
#include <fstream>
#include "NormalRoom.h"
#include "BossRoom.h"
#include "RoomDAO.h"
#include <algorithm>

FileParser::FileParser(Player* player, EnemyManager* enemyManager, ItemManager* itemManager, RoomDAO* roomDao)
	:m_pPlayer(player), m_pEnemyManager(enemyManager), m_pItemManager(itemManager), m_pRoomDAO(roomDao)
{
	GetFolderNames();
	SetFloor(0);
}


FileParser::~FileParser()
{
}

bool FileParser::SetFloor(int floorNumber)
{
	if (floorNumber >= m_FolderNames.size())
	{
		return false;
	}
	filePath = m_FolderNames.at(floorNumber);
	m_EnemyInfoArr.clear();
	return true;
}

void FileParser::GetFolderNames()
{
	std::ifstream roomsFile;

	roomsFile.open("Resources/FolderNames.txt", std::ios::out);

	std::string line;
	if (roomsFile.is_open())
	{
		while (getline(roomsFile, line))
		{
			m_FolderNames.push_back( CreateSubstrBetween(line, "{", "}") );
		}
	}
	else {
		std::string ErrorMessage = "File not found: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	roomsFile.close();
}

std::vector<Room*> FileParser::GetRoomsDataFromFille()
{
	std::vector<Room*> m_RoomsArr;
	std::ifstream roomsFile;
	roomsFile.open("Resources/" + filePath + "/Rooms.txt", std::ios::out);
	Room* tempRoom = nullptr;
	Enemy* tempEnemy = nullptr;

	std::string line;
	if (roomsFile.is_open())
	{
		while (getline(roomsFile, line))
		{
			if (line.find("room{") != std::string::npos)
			{
				// type and number
				getline(roomsFile, line);
				int roomNumber = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				std::string roomType = CreateSubstrBetween(line, "{", "}");

				if (roomType == "NORMAL")
				{
					tempRoom = new NormalRoom(roomNumber, m_pRoomDAO);
				}
				else if (roomType == "BOSSROOM")
				{
					tempRoom = new BossRoom(roomNumber, m_pRoomDAO);
				}
				// exits
				getline(roomsFile, line);
				std::vector<std::string> exits;
				std::string TempString = CreateSubstrBetween(line, "{", "}");
				exits = FilterString(TempString, ',');
				if (exits.size() > 3)
				{
					tempRoom->m_ExitsArr[NORTH] = stoi(exits.at(0));
					tempRoom->m_ExitsArr[EAST] = stoi(exits.at(1));
					tempRoom->m_ExitsArr[SOUTH] = stoi(exits.at(2));
					tempRoom->m_ExitsArr[WEST] = stoi(exits.at(3));
				}
				else
				{
					cout << "error parser " << endl;
				}

				// enemys
				getline(roomsFile, line);
				std::vector<std::string> enemys;
				TempString = CreateSubstrBetween(line, "{", "}");
				enemys = FilterString(TempString, ',');
				// names
				getline(roomsFile, line);
				std::vector<std::string> names;
				TempString = CreateSubstrBetween(line, "{", "}");
				names = FilterString(TempString, ',');

				for (size_t i = 0; i < enemys.size(); i++)
				{
					for (size_t j = 0; j < m_EnemyInfoArr.size(); j++)
					{
						if (enemys.at(i) == m_EnemyInfoArr.at(j).EnemyType)
						{

							if (names.size() != m_EnemyInfoArr.size() && names.size() != 0)
							{
								std::string ErrorMessage = "error with enemy names put a , for every enemy or insert a name there for every enemy : " + std::to_string(WSAGetLastError());
								MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
								exit(1);
							}

							if (names.size() != 0 && names.at(i) != "")
							{
								m_EnemyInfoArr.at(j).Name = names.at(i);
							}
							else { m_EnemyInfoArr.at(j).Name = m_EnemyInfoArr.at(j).EnemyType; }
							tempEnemy = new Enemy(m_pRoomDAO->GetPlayerPtr(), m_EnemyInfoArr.at(j));
							m_pEnemyManager->AddEnemy(tempEnemy, tempRoom);

						}
					}

				}

				// items
				getline(roomsFile, line);
				std::vector<std::string> items;
				TempString = CreateSubstrBetween(line, "{", "}");
				items = FilterString(TempString, ',');
				getline(roomsFile, line);

				m_RoomsArr.push_back(tempRoom);
			}
		}

	}
	else {
		std::string ErrorMessage = "File not found: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	roomsFile.close();

	return m_RoomsArr;
}

actionArrays FileParser::GetActionDataFromFille()
{
	std::ifstream roomsFile;
	roomsFile.open("Resources/ActionNames.txt", std::ios::out);
	std::string line;
	vector<action> actions;
	vector<action> secondActions;
	vector<action> chatActions;
	action tempAction;
	bool secondAction = false;
	bool chatAction = false;
	if (roomsFile.is_open())
	{
		while (getline(roomsFile, line))
		{
			if (line.find("SecondAction") != string::npos)
			{
				secondAction = true;
			}
			if (line.find("ChatActions") != string::npos)
			{
				secondAction = false;
				chatAction = true;
			}
			if (line.find("//") != string::npos || line.length() == 0)
			{
				continue;
			}
			else
			{
				for (size_t i = 0; i < line.length(); i++) {
					if (line[i] == ' ')
						line.erase(i, 1);
				}
				tempAction.ActionID = stoi(CreateSubstrBetween(line, "=", "="));
				vector<string> tempString = FilterString(CreateSubstrBetween(line, "{", "}"), ',');
				for (size_t i = 0; i < tempString.size(); i++)
				{
					tempAction.ActionCommand = (tempString.at(i));
					if (secondAction == true)
					{
						secondActions.push_back(tempAction);
					}
					else if (chatAction == true)
					{
						// add // to the chat inputs
						tempAction.ActionCommand = "//" + tempAction.ActionCommand;
						chatActions.push_back(tempAction);
					}
					else {
						actions.push_back(tempAction);
					}
				}
			}
		}
	}
	else {
		std::string ErrorMessage = "File not found: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	roomsFile.close();
	return actionArrays{ actions  , secondActions , chatActions };
}

std::vector<EnemyInfo>& FileParser::GetEnemyDataFromFile()
{
	std::ifstream roomsFile;
	roomsFile.open("Resources/" + filePath + "/Enemy.txt", std::ios::out);
	EnemyInfo tempInfo;

	std::string line;
	if (roomsFile.is_open())
	{
		while (getline(roomsFile, line))
		{
			if (line.find("enemy{") != std::string::npos)
			{
				// type and number
				getline(roomsFile, line);
				tempInfo.EnemyType = CreateSubstrBetween(line, "{", "}");
				getline(roomsFile, line);
				tempInfo.Level = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.Health = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.AtackDamage = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.AtackSpeed = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.CritChance = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.CritDamage = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.MinGainedSkillPoints = std::stoi(CreateSubstrBetween(line, "{", "}"));
				getline(roomsFile, line);
				tempInfo.MaxGainedSkillPoints = std::stoi(CreateSubstrBetween(line, "{", "}"));


				m_EnemyInfoArr.push_back(tempInfo);
			}
		}

	}
	else {
		std::string ErrorMessage = "File not found: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	roomsFile.close();

	return m_EnemyInfoArr;
}

vector<string> FileParser::FilterString(std::string & text, char seperation)
{
	std::vector<std::string> filteredString;

	if (text.size() == 0)
	{
		return filteredString;
	}

	int pos1 = 0;
	size_t it = 0;
	while (text.size() > it)
	{
		if (text.at(it) == seperation)
		{
			filteredString.push_back(text.substr(pos1, it - pos1));
			pos1 = it + 1;
		}

		it++;
	}

	if (pos1 != std::string::npos) {
		filteredString.push_back(text.substr(pos1, text.size()));
	}
	else { return filteredString; }
	return filteredString;
}

string FileParser::CreateSubstrBetween(string text, std::string first, std::string second)
{
	int startPos = text.find_first_of(first) + 1;
	int endPos = text.find_last_of(second);
	int diff = endPos - startPos;
	if (diff < 0)
	{
		std::string ErrorMessage = "error in fille: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	return text.substr(startPos, diff);
}
