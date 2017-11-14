#include "stdafx.h"
#include "Actions.h"
#include <vector>
#include "GUI.h"
#include "Room.h"
#include "GameMain.h"
#include "ClientManager.h"


Actions::Actions(RoomDAO* roomDAO , Player* player):m_pRoomDao(roomDAO) , m_pPlayer(player)
{
}


Actions::~Actions()
{
}


void Actions::ExecuteGameCommand(vector<string>* words)
{
	m_ActionsToExecuteArr.clear();

	if (words->size() <= 0) { return; }

	FindCommandToExecute(words->at(0), m_ActionArr);

	if (m_ActionsToExecuteArr.size() <= 0) { return; }

	switch (m_ActionsToExecuteArr[0].ActionID)
	{
	case 0: // quit
		m_EndOfGame = true;
		break;
	case 1: // move
		Move(*words);
		break;
	case 2: // search
		Search();
		break;
	case 3: // attack
		Atack(*words);
		break;
	case 4: // Level
		Level(*words);
		break;
	case 5: // join
		Join();
		break;
	case 6: // leave
		Leave();
		break;
	case 7: // focus target
		Focus(*words);
	default:
		break;
	}
}
void Actions::ExecuteChatCommand(vector<string>*words)
{
	m_ActionsToExecuteArr.clear();

	if (words->size() <= 0) { return; }

	FindCommandToExecute(words->at(0), m_ChatActionsArr);

	if (m_ActionsToExecuteArr.size() <= 0) { return; }

	switch (m_ActionsToExecuteArr[0].ActionID)
	{
	case 0: // quit
		m_EndOfGame = true;
		break;
	case 1: // name change
		ChangeName(*words);
		break;
	default:
		break;
	}
}

void Actions::FindCommandToExecute(std::string CommandToSearch, vector<action> ActionsarrToSearch)
{
	if (CommandToSearch == "")
	{
		return;
	}
	// search first action array 
	action ToExecuteAction;
	for (action tempAction : ActionsarrToSearch)
	{
		if (CommandToSearch == tempAction.ActionCommand)
		{
			ToExecuteAction = tempAction;
		}
	}

	// if still nothing is found there is no action to execute
	// ---------- print out for feedback ------------
	if (ToExecuteAction.ActionCommand != "")
	{
		GUI::GetInstance()->Wr("execute action : " + ToExecuteAction.ActionCommand);
		m_ActionsToExecuteArr.push_back(ToExecuteAction);

	}
	else {	GUI::GetInstance()->Wr("no valid command "); }

}
//void Actions::LoadActions()
//{
//	action tempAction;
//	tempAction.ActionID = 0;
//	tempAction.ActionCommand = "QUIT";
//	m_ActionArr.push_back(tempAction);
//	//move
//	tempAction.ActionID = 1;
//	tempAction.ActionCommand = "MOVE";
//	m_ActionArr.push_back(tempAction);
//	{
//		//move second actions
//		tempAction.ActionID = 0;
//		tempAction.ActionCommand = "NORTH";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "UP";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionID = 1;
//		tempAction.ActionCommand = "SOUTH";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "DOWN";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionID = 2;
//		tempAction.ActionCommand = "EAST";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "RIGHT";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionID = 3;
//		tempAction.ActionCommand = "WEST";
//		m_SecondActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "LEFT";
//		m_SecondActionArr.push_back(tempAction);
//	}
//
//
//	//search
//	{
//		tempAction.ActionID = 2;
//		tempAction.ActionCommand = "SEARCH";
//		m_ActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "LOOK";
//		m_ActionArr.push_back(tempAction);
//	}
//
//	//attack
//	{
//		tempAction.ActionID = 3;
//		tempAction.ActionCommand = "ATTACK";
//		m_ActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "ATACK";
//		m_ActionArr.push_back(tempAction);
//	}
//	// upgrade
//	{
//		tempAction.ActionID = 4;
//		tempAction.ActionCommand = "UPGRADE";
//		m_ActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "LEVEL";
//		m_ActionArr.push_back(tempAction);
//		tempAction.ActionCommand = "TRAIN";
//		m_ActionArr.push_back(tempAction);
//		{
//			// upgrade SecondCommands
//			tempAction.ActionID = 4;
//			tempAction.ActionCommand = "HEALTH";
//			m_SecondActionArr.push_back(tempAction);
//			tempAction.ActionID = 5;
//			tempAction.ActionCommand = "HEALTHREGEN";
//			m_SecondActionArr.push_back(tempAction);
//			tempAction.ActionID = 6;
//			tempAction.ActionCommand = "DAMAGE";
//			m_SecondActionArr.push_back(tempAction);
//			tempAction.ActionID = 7;
//			tempAction.ActionCommand = "ATACKSPEED";
//			m_SecondActionArr.push_back(tempAction);
//			tempAction.ActionID = 8;
//			tempAction.ActionCommand = "CRITCHANCE";
//			m_SecondActionArr.push_back(tempAction);
//			tempAction.ActionID = 9;
//			tempAction.ActionCommand = "CRITDAMAGE";
//			m_SecondActionArr.push_back(tempAction);
//		}
//	}
//	//********* ONLINE ***********
//	// JOIN
//	tempAction.ActionID = 5;
//	tempAction.ActionCommand = "JOIN";
//	m_ActionArr.push_back(tempAction);
//	// LEAVE
//	tempAction.ActionID = 6;
//	tempAction.ActionCommand = "LEAVE";
//	m_ActionArr.push_back(tempAction);
//}
//------------------- Action Fucntions ---------------------
//----------------------------------------------------------
void Actions::Move(vector<string> words)
{
	// check if there is a second command for the direction
	int MoveAmount = 1;
	if (words.size() == 3 && words[2] != "")
	{
		MoveAmount = atol(words[2].c_str());
		if (MoveAmount == 0)
		{
			GUI::GetInstance()->Wr("Move amount value was Wrong (needs to be a numeric value. Command was : " + words[2]);
			GUI::GetInstance()->Wr("Player was moved by 1");
		}
	}

	if (words.size() <= 1) { return; }
	FindCommandToExecute(words[1], m_SecondActionArr);

	if (m_ActionsToExecuteArr.size() <= 1) { return; }

	if (m_pRoomDao->GetActiveRoom()->m_RoomType == NORMAL)
	{
		if (m_pRoomDao->GetActiveRoom()->IsEnemyAlive())
		{
			GUI::GetInstance()->Wr("kill all the enemy's first");
			return;
		}
		switch (m_ActionsToExecuteArr[1].ActionID)
		{
		case 0: // NORTH
			m_pRoomDao->MoveToRoom(NORTH);
			break;
		case 1: // SOUTH
			m_pRoomDao->MoveToRoom(SOUTH);
			break;
		case 2: // EAST
			m_pRoomDao->MoveToRoom(EAST);
			break;
		case 3: // WEST
			m_pRoomDao->MoveToRoom(WEST);
			break;
		default:
			GUI::GetInstance()->Wr("error switch default case Parser::move()");
			//std::cout << "error switch default case Parser::move()" << endl;
			break;
		}
	}
	else if (m_pRoomDao->GetActiveRoom()->m_RoomType == BOSS && m_pRoomDao->GetActiveRoom()->IsEnemyAlive() == false)
	{
		switch (m_ActionsToExecuteArr[1].ActionID)
		{
		case 0: // NORTH
			m_pRoomDao->MoveInRoom(NORTH, MoveAmount);
			break;
		case 1: // SOUTH
			m_pRoomDao->MoveInRoom(SOUTH, MoveAmount);
			break;
		case 2: // EAST
			m_pRoomDao->MoveInRoom(EAST, MoveAmount);
			break;
		case 3: // WEST
			m_pRoomDao->MoveInRoom(WEST, MoveAmount);
			break;
		default:
			GUI::GetInstance()->Wr("error switch default case Parser::move()");
			//std::cout << "error switch default case Parser::move()" << endl;
			break;
		}
	}
	else
	{
		GUI::GetInstance()->Wr("not yet created;");
		//cout << "not yet created;" << endl;
	}
}

void Actions::Search()
{

}
void Actions::Atack(vector<string> words)
{
	if (m_pRoomDao->GetActiveRoom()->GetEnemys().size() < 0)
	{
		GUI::GetInstance()->Wr("No enemy in this room ");
		return;
	}
	GUI::GetInstance()->ReserveWrSpot(1);
	int target = m_pPlayer->GetFocus();

	if (words.size() == 2 )
	{
		target = atol(words[1].c_str());
	}
	if (target != 0)
	{
		GUI::GetInstance()->Wr("Targeted enemy is : " + to_string(target));
	}
	m_pRoomDao->GetPlayerPtr()->Atack(target , m_pRoomDao->GetActiveRoom()->GetEnemys());
}
void Actions::Level(vector<string> words)
{

	// check if there is a 3 command for the amount
	int UpgradeAmount = 1;
	if (words.size() == 3 && words[2] != "")
	{
		UpgradeAmount = atol(words[2].c_str());
		if (UpgradeAmount == 0)
		{
			GUI::GetInstance()->Wr("Upgrade amount value was Wrong (needs to be a numeric value. Command was : " + words[2]);
		}
	}
	// check if there are enough skill points to spend
	if ( m_pPlayer->GetInfo().SkillPoints < UpgradeAmount )
	{
		GUI::GetInstance()->Wr("Not enough skill points : " + to_string(m_pPlayer->GetInfo().SkillPoints) + " available ");
		return;
	}
	if (words.size() <= 1) { return; }
	FindCommandToExecute(words[1], m_SecondActionArr);

	if (m_ActionsToExecuteArr.size() <= 1) { return; }
	GUI::GetInstance()->LeaveOpenLine();
	
		if (m_pRoomDao->GetActiveRoom()->IsEnemyAlive())
		{
			GUI::GetInstance()->Wr("kill all the enemy's first");
			return;
		}
		switch (m_ActionsToExecuteArr[1].ActionID)
		{
		case 4: //HEALTH
			m_pPlayer->UpgradeHealth(UpgradeAmount);
			break;
		case 5: //HEALTHREGEN
			m_pPlayer->UpgradeHealthRegen(UpgradeAmount);
			break;
		case 6: //DAMAGE
			m_pPlayer->UpgradeDamage(UpgradeAmount);
			break;
		case 7: //ATACKSPEED
			m_pPlayer->UpgradeAtackSpeed(UpgradeAmount);
			break;
		case 8: //CRITCHANCE
			m_pPlayer->UpgradeCritChance(UpgradeAmount);
			break;
		case 9: //CRITDAMAGE
			m_pPlayer->upgradeCritDamage(UpgradeAmount);
			break;

		default:
			GUI::GetInstance()->Wr("Cant upgrade this.");
			//std::cout << "error switch default case Parser::move()" << endl;
			break;
		}
	
}
void Actions::Join()
{
	// only if in the first room
	if (m_pRoomDao->GetActiveRoom() == m_pRoomDao->GetRooms()->at(0))
	{
		ClientManager::GetInstance()->Join();// SetIsPlatingOnline(true);

	}
	else { GUI::GetInstance()->Wr("you made to mutch progress to join restart the game to join"); }
	// todo : send message to server
}
void Actions::Leave()
{
	ClientManager::GetInstance()->Leave();
	// todo : send message to server
}
void Actions::Focus(vector<string> words)
{
	int target;
	// check if there is a 2 command for the focus
	if (words.size() == 2)
	{
		target = atol(words[1].c_str());
	}
	m_pPlayer->SetFocus(target);
	Atack(vector<string>{});
}

void Actions::ChangeName(vector<string> words)
{
	if (words.size() >= 2)
	{
		m_pPlayer->SetName(words.at(1));
		ClientManager::GetInstance()->ChangeName(words.at(1));
	}
	GUI::GetInstance()->WritePlayerInfo(m_pPlayer->GetInfo());
}