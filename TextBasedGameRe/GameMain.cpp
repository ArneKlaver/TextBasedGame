#include "stdafx.h"
#include "GameMain.h"
#include "RoomDAO.h"
#include "Player.h"
#include "Actions.h"
#include "Input.h"
#include "Client.h"
#include "GUI.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "ClientManager.h"
#include "FileParser.h"
GameMain::GameMain()
{
	// localhost
	//m_pClient = new Client("127.0.0.1", 27090);
	// serverdenar.no-ip.org
	m_pClient = new Client("serverdenar.no-ip.org",27090);
	m_pPlayer = new Player();
	m_pEnemyManager = new EnemyManager(&m_pPlayer);
	m_pItemManager = new ItemManager(m_pPlayer);
	m_pRoomDao = new RoomDAO(m_pPlayer , m_pEnemyManager , m_pItemManager );
	m_pParser = new FileParser(m_pPlayer, m_pEnemyManager, m_pItemManager, m_pRoomDao);
	m_pActions = new Actions(m_pRoomDao , m_pPlayer);
	m_pInput = new Input(m_pClient , m_pPlayer);

	ClientManager::GetInstance()->SetClientPtr(m_pClient);
	ClientManager::GetInstance()->SetEnemyManagerPtr(m_pEnemyManager);
	ClientManager::GetInstance()->SetRoomDAOPtr(m_pRoomDao);
	ClientManager::GetInstance()->SetGameMain(this);

	//TODO: should happen in a thread
	if (!m_pClient->Connect())
	{
		GUI::GetInstance()->Wr("NO CONNECTION", 3);
		//system("pauze");
	}
	GUI::GetInstance()->Wr("Floor : " + to_string(1), 2);

	m_pParser->GetEnemyDataFromFile();
	std::vector<Room*> tempRooms = m_pParser->GetRoomsDataFromFille();
	m_pRoomDao->SetRoomsArr(tempRooms);

	m_pActions->SetActionsArray(m_pParser->GetActionDataFromFille());

	m_pRoomDao->Update();

	GUI::GetInstance()->ResetDrawPosition();
}


GameMain::~GameMain()
{
	delete m_pRoomDao;
	m_pRoomDao = nullptr;
	delete m_pPlayer;
	m_pPlayer = nullptr;
	delete m_pActions;
	m_pActions = nullptr;
	delete m_pInput;
	m_pInput = nullptr;
	delete m_pClient;
	m_pClient = nullptr;
	delete m_pEnemyManager;
	m_pEnemyManager = nullptr;
	delete m_pItemManager;
	m_pItemManager = nullptr;
	delete m_pParser;
	m_pParser = nullptr;
}

void GameMain::Update()
{

	auto ClientManagerInst = ClientManager::GetInstance();
	//  offline
	if (m_pInput->HasGameInput() && !ClientManagerInst->GetIsPlayingOnline())
	{
		GUI::GetInstance()->ClearInputGame();
  		GUI::GetInstance()->ClearWindow(1);

		m_pActions->ExecuteGameCommand(m_pInput->GetGameWords());
		m_pRoomDao->Update(); 
		m_pEnemyManager->Update();
		m_pPlayer->update();

		// reset needs to be last
		GUI::GetInstance()->ResetDrawPosition();
		m_pInput->ResetHasGameInput();
	}
	//  Onnline
	if (ClientManagerInst->GetIsPlayingOnline())
	{
		if (m_pInput->HasGameInput())
		{
			if (ClientManagerInst->GetIsMyTurn())
			{
				GUI::GetInstance()->LeaveOpenLine();
				GUI::GetInstance()->ClearInputGame();
				GUI::GetInstance()->ClearWindow(1);
				Room* firstRoom = m_pRoomDao->GetActiveRoom();
				m_pActions->ExecuteGameCommand(m_pInput->GetGameWords());
				m_pRoomDao->Update();
				m_pPlayer->update();
				Room* SecondRoom = m_pRoomDao->GetActiveRoom();

				// reset needs to be last
				GUI::GetInstance()->ResetDrawPosition();
				m_pInput->ResetHasGameInput();
				// if player moved
				if (firstRoom != SecondRoom)
				{	
				}
				else if (ClientManagerInst->GetIsPlayingOnline())// if still online send turn is done
				{
					ClientManager::GetInstance()->PlayerTurnDone(m_pRoomDao->GetActiveRoom()->IsEnemyAlive());
				}
			}
			// if waiting for other players clear the input
			m_pInput->ResetHasGameInput();
		}

		
		if (ClientManagerInst->GetIsEnemyTurn())
		{
			m_pEnemyManager->Update();
			m_pPlayer->update();
			ClientManager::GetInstance()->EnemyTurnDone();
		}
	}
	
	if (m_pInput->HasChatInput())
	{
		m_pActions->ExecuteChatCommand(m_pInput->GetChatWords());

		//GUI::GetInstance()->ClearInputChat();
		//GUI::GetInstance()->ResetInputPosition(); 
		m_pInput->GetChatWords()->clear();
		m_pInput->ResetHasChatInput();
	}
}


void GameMain::LoadFloor(int floor)
{
	if (!m_pParser->SetFloor(floor))
	{
		GUI::GetInstance()->Wr("NO MORE ROOMS TO LOAD !!!");
		GUI::GetInstance()->Wr("End of the Game !!!");
		GUI::GetInstance()->Wr("press enter to exit");
		getchar();
		m_pActions->EndOfGame();
		return;
	}

	// reset data
	vector<Room*>* rooms = m_pRoomDao->GetRooms();
	for (Room* room : *rooms)
	{
		delete room;
		room = nullptr;
	}
	rooms->clear();
	vector<Enemy*>* enemys = m_pEnemyManager->GetAllEnemys();
	for (Enemy* enemy : *enemys)
	{
		delete enemy;
		enemy = nullptr;
	}
	enemys->clear();
	m_pEnemyManager->ResetTotalEnemys();

	//reset screen
	GUI::GetInstance()->ClearWindow(1);
	GUI::GetInstance()->ClearWindow(2);
	m_pRoomDao->ResetRoomDrawPos();

	GUI::GetInstance()->Wr("Floor : " + to_string(floor + 1), 2);
	GUI::GetInstance()->Wr("The end room has been reached !!!");
	//GUI::GetInstance()->Wr("The end boss has been defeated!!!");
	GUI::GetInstance()->Wr("you where send to the next floor");

	// reset position
	// get new data
	m_pParser->GetEnemyDataFromFile();
	std::vector<Room*> tempRooms = m_pParser->GetRoomsDataFromFille();
	m_pRoomDao->SetRoomsArr(tempRooms);

}