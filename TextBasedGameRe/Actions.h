#pragma once
#include "Singleton.h"
#include <vector>
#include "RoomDAO.h"

class Actions
{
public:
	Actions(RoomDAO* roomDAO, Player* player);
	~Actions();


	// action functions
	void Move(vector<string> words);
	void Search();
	void Atack(vector<string> words);
	void Level(vector<string> words);
	void Join();
	void Leave();
	void Focus(vector<string> words);

	void ChangeName(vector<string> words);

	void ExecuteGameCommand(vector<string>* words);
	void ExecuteChatCommand(vector<string>*words);
	void FindCommandToExecute(std::string CommandToSearch, vector<action> ActionsarrToSearch);
	void EndOfGame() { m_EndOfGame = true; }

	bool CheckEndOfGame() const { return m_EndOfGame; }
	void SetActionsArray(actionArrays& actions) { m_ActionArr = actions.m_ActionArr; m_SecondActionArr = actions.m_SecondActionArr; m_ChatActionsArr = actions.m_ChatActionsArr; }
private:
	bool m_EndOfGame = false;
	std::vector<action> m_ActionArr;
	std::vector<action> m_SecondActionArr;
	std::vector<action> m_ActionsToExecuteArr;
	std::vector<action> m_ChatActionsArr;
	RoomDAO* m_pRoomDao;
	Player* m_pPlayer = nullptr;
	//void LoadActions();

};

