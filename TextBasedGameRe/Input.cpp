#include "stdafx.h"
#include "Input.h"
#include "GUI.h"
#include "Client.h"
#include "Actions.h"
#include <algorithm>
#include <conio.h>
#include <iostream>
#include "Player.h"
#include "ClientManager.h"

Input::Input(Client* client , Player* player):m_pClient(client),m_pPlayer(player)
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GetInput, (void*) this, NULL, NULL); //Create the client thread that will receive any data that the server sends.
}


Input::~Input()
{
}



// replaced by tab
POINT Input::GetMousePos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(FindWindow(NULL, L"TextAdventure"), &mousePos);
	return mousePos;
}

void Input::FilterStringToWords(string enteredString , vector<std::string>& words)
{
	words.clear();
	//m_ActionsToExecuteArr.clear();
	string subStr;
	char space = ' ';
	// ------- put the words in a words vector --------
	for (unsigned int i = 0; i < enteredString.size() + 1; i++)
	{
		if (i == enteredString.size())// or if the end is reached 
		{
			words.push_back(subStr);
			subStr.clear();
		}
		else if (enteredString.at(i) != space)// if no space detected add letter to subStr untill a space is detected
		{
			subStr.insert(subStr.end(), enteredString.at(i));
		}
		else if (enteredString.at(i) == space) // if a space is detected add the subStr to the Words vectorand clear the subStr
		{
			words.push_back(subStr);
			subStr.clear();
		}
	}
	//-------------- make all uppercase ---------------
	for (unsigned int i = 0; i < words.size(); i++)
	{
		string tempString = words.at(i);
		//lambda
		for_each(tempString.begin(), tempString.end(), [](char &tempchar) { if (islower(tempchar)) tempchar = toupper(tempchar); });

		words.at(i) = tempString;
	}
}

// THREAD
void Input::GetInput(void* Param)
{
	Input* pInput = (Input*) Param;
	bool wasTabPressed = false;
	// inf loop for the input
	while (true)
	{
		// if tab is pressed switch the input window (game or chat)
		if (GetAsyncKeyState(VK_TAB))
		{
			if (wasTabPressed == false)
			{
				wasTabPressed = true;
				// switch selected
				switch (GUI::GetInstance()->GetSelectedWindow())
				{
					case SELECTEDWIDNOW::GAME :
						GUI::GetInstance()->SetSelectedWindow(SELECTEDWIDNOW::CHAT) ;
						GUI::GetInstance()->ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosChat()->x, GUI::GetInstance()->GetInputPosChat()->y);
						break;
					case SELECTEDWIDNOW::CHAT:
						GUI::GetInstance()->SetSelectedWindow(SELECTEDWIDNOW::GAME);
						GUI::GetInstance()->ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosGame()->x, GUI::GetInstance()->GetInputPosGame()->y);
						break;
					default:
						MessageBoxA(NULL, "ERROR input -> Input::GetInput()", "Error", MB_OK | MB_ICONERROR);
						exit(1); //Failed to Connect
				}
			}
		}
		// else if there is  char in the input get the input and procces it
		// if there was more then 1 char clear the input and do nothing with it ( arrow key , home button .... )
		else
		{
			wasTabPressed = false;
			if (_kbhit())
			{
				int inputCode;
				char input;
				inputCode = _getch();
				input = inputCode;
				if (!_kbhit())
				{
					// enter pressed (send command and clear line)
					auto clientInst = ClientManager::GetInstance();
					if ((input == '\r' || input == '\n') )
					{
						switch (GUI::GetInstance()->GetSelectedWindow())
						{
						case SELECTEDWIDNOW::GAME:
							if (clientInst->GetIsMyTurn())
							{
								pInput->FilterStringToWords(pInput->m_InputGame, pInput->m_GameWords);
								pInput->m_HasGameInput = true;
								pInput->m_InputGame.clear();
							}			
							break;
						case SELECTEDWIDNOW::CHAT:
							if (pInput->m_InputChat.find("//") != std::string::npos)
							{
								pInput->FilterStringToWords(pInput->m_InputChat, pInput->m_ChatWords);
							}
							else
							{
								pInput->m_pClient->SendChatMessage(pInput->m_pPlayer->GetInfo().Name + " : " + pInput->m_InputChat);
							}
					
							pInput->m_HasChatInput = true;

							GUI::GetInstance()->Wr(pInput->m_pPlayer->GetInfo().Name + " : " + pInput->m_InputChat, 3);
							GUI::GetInstance()->ClearInputChat();
							pInput->m_InputChat.clear();
							GUI::GetInstance()->ResetDrawPosition();
							break;
						default:
							MessageBoxA(NULL, "ERROR input -> Input::GetInput()", "Error", MB_OK | MB_ICONERROR);
							exit(1); //Failed to Connect
						}
					}
					// delete a letter
					else if (GetAsyncKeyState(VK_BACK))
					{
						switch (GUI::GetInstance()->GetSelectedWindow())
						{
						case SELECTEDWIDNOW::GAME:
							if (pInput->m_InputGame.size() > 0)
							{
								cout << '\b' << " " << '\b';
								pInput->m_InputGame.pop_back();
								GUI::GetInstance()->GetInputPosGame()->x--;
							}
							break;
						case SELECTEDWIDNOW::CHAT:
							if (pInput->m_InputChat.size() > 0)
							{
								cout << '\b' << " " << '\b';
								pInput->m_InputChat.pop_back();
								GUI::GetInstance()->GetInputPosChat()->x--;
							}
							break;
						default:
							MessageBoxA(NULL, "ERROR input -> Input::GetInput()", "Error", MB_OK | MB_ICONERROR);
							exit(1); //Failed to Connect
						}
					}
					// normal text
					else if (inputCode >= 32 && inputCode <= 255)
					{
						switch (GUI::GetInstance()->GetSelectedWindow())
						{
						case SELECTEDWIDNOW::GAME:
							if ((int)pInput->m_InputGame.size() <= GUI::GetInstance()->GetFrameDimensions(1).x - 2)
							{
								cout << input;
								pInput->m_InputGame.push_back(input);
								GUI::GetInstance()->GetInputPosGame()->x++;
							}
							break;
						case SELECTEDWIDNOW::CHAT:
							if ((int)pInput->m_InputChat.size() <= (GUI::GetInstance()->GetFrameDimensions(1).x - 2) - pInput->m_pPlayer->GetInfo().Name.size() - 4)
							{
								cout << input;
								pInput->m_InputChat.push_back(input);
								GUI::GetInstance()->GetInputPosChat()->x++;
							}
							break;
						default:
							MessageBoxA(NULL, "ERROR input -> Input::GetInput()", "Error", MB_OK | MB_ICONERROR);
							exit(1); //Failed to Connect
						}
					}
				}
				else { _getch(); }
			}
		}	
	}
}