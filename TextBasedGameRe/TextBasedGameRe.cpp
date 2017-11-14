// TextBasedGameRe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GUI.h"
#include "Client.h"
#include "Time.h"
#include "Player.h"

#include <iostream>
#include "Input.h"
#include <signal.h>
#include "Actions.h"
#include "GameMain.h"


#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <ctime>
#include "ClientManager.h"


Client* m_Client = nullptr;
GameMain* m_pGame = nullptr;

bool g_exit = false;
BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{	
	if (CTRL_CLOSE_EVENT == dwCtrlType) {
		g_exit = true;
		return TRUE;
	}
	if (CTRL_C_EVENT == dwCtrlType) {
		g_exit = true;
		return TRUE;
	}
	if (CTRL_BREAK_EVENT == dwCtrlType) {
		g_exit = true;
		return TRUE;
	}
	if (CTRL_LOGOFF_EVENT == dwCtrlType) {
		g_exit = true;
		return TRUE;
	}
	if (CTRL_SHUTDOWN_EVENT == dwCtrlType) {
		g_exit = true;
		return TRUE;
	}
	return FALSE;
}

int main()
{
	// memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// console handler
	if (FALSE == SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE)) {
		return 1;
	}
	// set the console title
	SetConsoleTitle(L"TextAdventure");
	// initialize  random seed
	srand((unsigned int)time(NULL));
	// ceating gui instance
	GUI::GetInstance();
	ClientManager::GetInstance();
	// game loop
	m_pGame = new GameMain();
	while (!g_exit && m_pGame->CheckEndOfGame() == false)
	{		
		m_pGame->Update();
		// sleep to save performance
		Sleep(100);
	}
	ClientManager::GetInstance()->Disconnect();
	GUI::DestroyInstance();
	ClientManager::DestroyInstance();
	delete m_pGame;
	return 0;
}

