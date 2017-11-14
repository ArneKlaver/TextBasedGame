#include "stdafx.h"

#include "GUI.h"
#include <iostream>
#include "Room.h"
#include <algorithm>


GUI::GUI()
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenSize();
	GetWidnowSize();
	GetScreenDimension();
	DrawFrame();

}


GUI::~GUI()
{
}
void GUI::SetConsoleScreenSize()
{
	int Width = 121;
	int Height = 40;
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	SetConsoleScreenBufferSize(m_ConsoleHandle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(m_ConsoleHandle, TRUE, &Rect);
}
void GUI::GetWidnowSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	GetConsoleScreenBufferInfo(m_ConsoleHandle, &csbi);
	m_DimensionsScreen.x = csbi.srWindow.Right + csbi.srWindow.Left;
	m_DimensionsScreen.y = csbi.srWindow.Bottom + csbi.srWindow.Top;
}
void GUI::DrawFrame()
{
	for (int i = 0; i < m_DimensionsScreen.y ; i++)
	{
		ConsoleSetCursorPosition(0, i);
		std::cout << "|";
		ConsoleSetCursorPosition(m_DimensionsW1.x + m_BorderThickness, i);
		std::cout << "|";
		ConsoleSetCursorPosition(m_DimensionsScreen.x, i);
		std::cout << "|";
	}
	for (int i = 0; i < m_DimensionsScreen.x ; i++)
	{
		ConsoleSetCursorPosition(i, 0);
		std::cout << "=";
		ConsoleSetCursorPosition(i, m_DimensionsW1.y);
		std::cout << "=";
		ConsoleSetCursorPosition(i, m_DimensionsScreen.y);
		std::cout << "=";
	}
	for (int i = 1; i < m_DimensionsW1.x ; i++)
	{
		ConsoleSetCursorPosition(i, m_DimensionsW1.y - 2);
		std::cout << "-";
		ConsoleSetCursorPosition(i, m_DimensionsScreen.y - 2);
		std::cout << "-";
	}
	ConsoleSetCursorPosition(0, 0);
	ConsoleSetCursorPosition(m_InputPosGame.x, m_InputPosGame.y);
}
void GUI::GetScreenDimension()
{
	m_DimensionsW1.x = m_DimensionsScreen.x / 10 * m_TextRatioX;
	m_DimensionsW1.y = m_DimensionsScreen.y / 10 * m_TextRatioY;

	m_DimensionsW2.x = m_DimensionsScreen.x - m_DimensionsW1.x - (2 * m_BorderThickness);
	m_DimensionsW2.y = m_DimensionsW1.y;

	m_DimensionsW3.x = m_DimensionsW1.x;
	m_DimensionsW3.y = m_DimensionsScreen.y - m_DimensionsW1.y - (2 * m_BorderThickness);

	m_DimensionsW4.x = m_DimensionsScreen.x - m_DimensionsW1.x - (2 * m_BorderThickness);
	m_DimensionsW4.y = m_DimensionsScreen.y - m_DimensionsW1.y - (2 * m_BorderThickness);

	m_Frame1Pos = { m_BorderThickness						, m_BorderThickness };
	m_Frame2Pos = { m_DimensionsW1.x + m_BorderThickness + 1	, m_BorderThickness };
	m_Frame3Pos = { m_BorderThickness						, m_DimensionsW1.y + m_BorderThickness };
	m_Frame4Pos = { m_DimensionsW1.x + m_BorderThickness + 1	, m_DimensionsW1.y + m_BorderThickness };

	m_InputPosGame = { m_BorderThickness , m_DimensionsW1.y - 1 };
	m_InputPosChat = { m_BorderThickness ,  m_DimensionsScreen.y - 1 };

	m_EnemyDamageDrawPos = { m_BorderThickness , m_EnemyDrawPos.y - 4 };
	m_PlayerDamageDrawPos = { m_BorderThickness , m_EnemyDamageDrawPos.y - 4 };
}
void GUI::ClearInputChat()
{
	for (int i = 1; i < m_DimensionsW1.x; i++)
	{
		ConsoleSetCursorPosition(i, m_InputPosChat.y);
		std::cout << " ";
	}
}
void GUI::ClearInputGame()
{
	for (int i = 1; i < m_DimensionsW1.x; i++)
	{
		ConsoleSetCursorPosition( i, m_InputPosGame.y);
		std::cout << " ";
	}
}
void GUI::ResetDrawPosition()
{
	switch (GetSelectedWindow())
	{
	case SELECTEDWIDNOW::GAME:
		m_InputPosGame = { m_BorderThickness , m_DimensionsW1.y - 1 };
		ConsoleSetCursorPosition(m_InputPosGame.x , m_InputPosGame.y);
		m_Frame1Pos = { m_BorderThickness						, m_BorderThickness };
		m_Frame2Pos = { m_DimensionsW1.x + m_BorderThickness +1	, m_BorderThickness };
		m_Frame4Pos = { m_DimensionsW1.x + m_BorderThickness +1	, m_DimensionsW1.y + m_BorderThickness };
		m_EnemyDamageDrawPos = { m_BorderThickness , m_EnemyDrawPos.y - 4 };
		m_PlayerDamageDrawPos = { m_BorderThickness , m_EnemyDamageDrawPos.y - 4 };
		break;
	case SELECTEDWIDNOW::CHAT:
		m_InputPosChat = { m_BorderThickness ,  m_DimensionsScreen.y - 1 };
		ConsoleSetCursorPosition(m_InputPosChat.x, m_InputPosChat.y);
		m_Frame3Pos = { m_BorderThickness	, m_DimensionsW1.y + m_BorderThickness };
		break;
	}
}
void GUI::WrOnReservedSpot(string text , int SpotId , bool permanent)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	auto it = m_ReservedSpot.find(SpotId);
	if (it != m_ReservedSpot.end())
	{
		ConsoleSetCursorPosition(1, m_ReservedSpot[SpotId]);
		ClearLine();
		ConsoleSetCursorPosition(1, m_ReservedSpot[SpotId]);
		cout << text;

		if (!permanent)
		{
			m_ReservedSpot.erase(m_ReservedSpot[SpotId]);
		}		
	}
	else
	{
		m_IsWriting = false;
		Wr(text);
	}
	m_IsWriting = false;

	Int2 pos;
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		pos = *GetInputPosGame();
		break;
	case SELECTEDWIDNOW::CHAT:
		pos = *GetInputPosChat();
	}
	ConsoleSetCursorPosition(pos.x, pos.y);

}
void GUI::WrPlayerTurn(string text)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

		ConsoleSetCursorPosition(1, 1);
		ClearLine();
		ConsoleSetCursorPosition(1, 1);
		cout << text;


	Int2 pos;
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		pos = *GetInputPosGame();
		break;
	case SELECTEDWIDNOW::CHAT:
		pos = *GetInputPosChat();
	}
	ConsoleSetCursorPosition(pos.x, pos.y);
	m_IsWriting = false;

}
void GUI::ReserveWrSpot(int SpotId)
{
	m_ReservedSpot[SpotId] = m_Frame1Pos.y;
	m_Frame1Pos.y++;
}
Int2 GUI::GetFramePos(int frame)
{
	switch (frame)
	{
		case 1:
			return m_Frame1Pos;
			break;
		case 2:
			return m_Frame2Pos;
			break;
		case 3:
			return m_Frame3Pos;
			break;
		case 4:
			return m_Frame4Pos;
			break;
		default:
			return Int2();
	}
}
Int2 GUI::GetFrameDimensions(int frame)
{
	switch (frame)
	{
	case 1:
		return m_DimensionsW1;
		break;
	case 2:
		return m_DimensionsW2;
		break;
	case 3:
		return m_DimensionsW3;
		break;
	case 4:
		return m_DimensionsW4;
		break;
	default:
		return Int2();
	}
}
void GUI::ClearWindow(int window)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	int clearLenght = 0;

	Int2 printPos;
	Int2 frameSize;
	switch (window)
	{
	case 1:
		printPos = { m_BorderThickness	, m_BorderThickness };
		frameSize = m_DimensionsW1;
		clearLenght -= 2;
		break;
	case 2:
		printPos = {m_DimensionsW1.x + m_BorderThickness +1, m_BorderThickness};
		frameSize = m_DimensionsW2;
		break;
	case 3:
		printPos = m_Frame3Pos;
		frameSize = m_DimensionsW3;
		break;
	case 4:
		printPos = m_Frame4Pos;
		frameSize = m_DimensionsW4;
		break;
	default:
		break;
	}

	clearLenght += frameSize.y - 2;

	for (int j = 0; j <= clearLenght ; j++)
	{
		ConsoleSetCursorPosition(printPos.x, printPos.y + j);
		ClearLine(window);
	}
	m_IsWriting = false;
}
void GUI::ClearLine(int window, int lenght)
{

	Int2 printPos ;
	Int2 frameSize = m_DimensionsW1;
	switch (window)
	{
	case 1:
		printPos = m_Frame1Pos;
		frameSize = m_DimensionsW1;
		break;
	case 2:
		printPos = m_Frame2Pos;
		frameSize = m_DimensionsW2;
		break;
	case 3:
		printPos = m_Frame3Pos;
		frameSize = m_DimensionsW3;
		break;
	case 4:
		printPos = m_Frame4Pos;
		frameSize = m_DimensionsW4;
		break;
	default:
		break;
	}
	string tempstring;
	for (int i = 1; i < frameSize.x ; i++ )
	{
		tempstring += " ";
	}
	std::cout << tempstring;
}

void GUI::ConsoleSetCursorPosition(int column, int row)
{
	//cool trick to avoid the use of the lettersX andY on COORD --> those are not compatible with the renaming used in the template classesXandY class templates
	struct coord {
		SHORT column;
		SHORT row;
	};
	union MyUnion
	{
		coord cursorPosition;
		COORD COORD;
	}myUnion;

	myUnion.cursorPosition.column = (SHORT)column;
	myUnion.cursorPosition.row = (SHORT)row;

	SetConsoleCursorPosition(m_ConsoleHandle, myUnion.COORD);

}
void GUI::ConsoleSetCursorPosition(Int2 pos)
{
	//cool trick to avoid the use of the lettersX andY on COORD --> those are not compatible with the renaming used in the template classesXandY class templates
	struct coord {
		SHORT column;
		SHORT row;
	};
	union MyUnion
	{
		coord cursorPosition;
		COORD COORD;
	}myUnion;

	myUnion.cursorPosition.column = (SHORT)pos.x;
	myUnion.cursorPosition.row = (SHORT)pos.y;

	SetConsoleCursorPosition(m_ConsoleHandle, myUnion.COORD);

}

// write finction
void GUI::Wr(std::string text, int window, int row)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
		m_IsWriting = true;

		Int2* printPos = nullptr;
		Int2 frameSize;
		switch (window)
		{
		case 1:
			printPos = &m_Frame1Pos;
			frameSize = m_DimensionsW1;
			break;
		case 2:
			printPos = &m_Frame2Pos;
			frameSize = m_DimensionsW2;
			break;
		case 3:
			m_IsWriting = false;
			PrintChat(text);
			return;
			break;
		case 4:
			printPos = &m_Frame4Pos;
			frameSize = m_DimensionsW4;
			break;
		default:
			break;
		}

		if ((int)text.length() > frameSize.x - 2) // string to long (only checked once so max lenght = m_WidthFrame1*2)
		{
			ClearLine();
			ConsoleSetCursorPosition(printPos->x, printPos->y);
			std::cout << text.substr(0, frameSize.x - 2);
			printPos->y++;
			ClearLine();
			ConsoleSetCursorPosition(printPos->x, printPos->y);
			std::cout << text.substr(frameSize.x);
			printPos->y++;
		}
		else  // if string is not to long
		{
			ConsoleSetCursorPosition(printPos->x, printPos->y);
			ClearLine();
			ConsoleSetCursorPosition(printPos->x, printPos->y);
			std::cout << text;
			printPos->y++;
		}

		Int2 pos;
		switch (m_SelectedWindow)
		{
		case SELECTEDWIDNOW::GAME:
			pos = *GetInputPosGame();
			break;
		case SELECTEDWIDNOW::CHAT:
			pos = *GetInputPosChat();
		}
		ConsoleSetCursorPosition(pos.x, pos.y);
		m_IsWriting = false;

}
// chat
void GUI::PrintChat(std::string text)
{	
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	m_Frame3Pos;
	m_DimensionsW3;
	// put text in buffer
	m_ChatBuffer.push_back(text);
	// if buffer to big remove a message
	if ((int)m_ChatBuffer.size() > m_DimensionsW3.y - 1)
	{
		m_IsWriting = false;
		ClearWindow(3);
		m_IsWriting = true;
		m_ChatBuffer.erase(m_ChatBuffer.begin());
	}
	for (string temp : m_ChatBuffer)
	{
		ConsoleSetCursorPosition(m_Frame3Pos.x, m_Frame3Pos.y);
		cout << temp;
		m_Frame3Pos.y++;
	}

	//print bugger

	m_Frame3Pos = { m_BorderThickness, m_DimensionsW1.y + m_BorderThickness };
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosGame()->x, GUI::GetInstance()->GetInputPosGame()->y);
		break;
	case SELECTEDWIDNOW::CHAT:
		ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosChat()->x, GUI::GetInstance()->GetInputPosChat()->y);
		break;
	}
	m_IsWriting = false;

}
// enemy info
void GUI::WriteEnemyInfo(std::vector<Enemy*> enemysInRoom)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	Int2 pos = { 2, m_DimensionsW1.y - 7 };
	for (size_t i = 0; i <= 4; i++)
	{
		ConsoleSetCursorPosition(pos);
		string tempstring;
		for (int j = 1; j < m_DimensionsW1.x; j++)
		{
			tempstring += " ";
		}
		std::cout << tempstring;
		pos.y += 1;
	}

	m_EnemyDrawPos = { 2, m_DimensionsW1.y - 7 };

	// print description of data
	ConsoleSetCursorPosition(m_EnemyDrawPos);
	cout << "target     :";
	m_EnemyDrawPos.y += 1;
	ConsoleSetCursorPosition(m_EnemyDrawPos);
	cout << "name       :";
	m_EnemyDrawPos.y += 1;
	ConsoleSetCursorPosition(m_EnemyDrawPos);
	cout << "health     :";
	m_EnemyDrawPos.y += 1;
	ConsoleSetCursorPosition(m_EnemyDrawPos);
	cout << "Damage     :";
	m_EnemyDrawPos.y += 1;
	ConsoleSetCursorPosition(m_EnemyDrawPos);
	cout << "AtackSpeed :";

	m_EnemyDrawPos.y -= 4;
	m_EnemyDrawPos.x += 15;

	int deads = 0;
	// print data per enemy
	for (size_t i = 0; i < enemysInRoom.size(); i++)
	{
		if (enemysInRoom[i]->GetInfo().Health <= 0)
		{
			deads++;
			continue;
		}
		// if more then 6 enemys print ... to show there are more
		if (i >= 6)
		{
			m_EnemyDrawPos.x -= 3;
			ConsoleSetCursorPosition(m_EnemyDrawPos);
			cout << "...";
			m_EnemyDrawPos.y += 1;
			ConsoleSetCursorPosition(m_EnemyDrawPos);
			cout << "...";
			m_EnemyDrawPos.y += 1;
			ConsoleSetCursorPosition(m_EnemyDrawPos);
			cout << "...";
			m_EnemyDrawPos.y += 1;
			ConsoleSetCursorPosition(m_EnemyDrawPos);
			cout << "...";
			m_EnemyDrawPos.y += 1;
			ConsoleSetCursorPosition(m_EnemyDrawPos);
			cout << "...";
			return;
		}
		ConsoleSetCursorPosition(m_EnemyDrawPos);
		enemysInRoom[i]->SetTargetNumber(i + 1 - deads);
		cout << enemysInRoom[i]->GetInfo().TargetNumber;
		m_EnemyDrawPos.y += 1;
		ConsoleSetCursorPosition(m_EnemyDrawPos);
		cout << enemysInRoom[i]->GetInfo().Name;
		m_EnemyDrawPos.y += 1;
		ConsoleSetCursorPosition(m_EnemyDrawPos);
		cout << enemysInRoom[i]->GetInfo().Health;
		m_EnemyDrawPos.y += 1;
		ConsoleSetCursorPosition(m_EnemyDrawPos);
		cout << enemysInRoom[i]->GetInfo().AtackDamage;
		m_EnemyDrawPos.y += 1;
		ConsoleSetCursorPosition(m_EnemyDrawPos);
		cout << enemysInRoom[i]->GetInfo().AtackSpeed;


		m_EnemyDrawPos.y -= 4;

		m_EnemyDrawPos.x += 10;
	}
	m_Frame3Pos = { m_BorderThickness, m_DimensionsW1.y + m_BorderThickness };
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosGame()->x, GUI::GetInstance()->GetInputPosGame()->y);
		break;
	case SELECTEDWIDNOW::CHAT:
		ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosChat()->x, GUI::GetInstance()->GetInputPosChat()->y);
		break;
	}
	m_IsWriting = false;
}
// enemy damage
void GUI::WriteEnemyDamage(string text)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	ConsoleSetCursorPosition(m_EnemyDamageDrawPos.x, m_EnemyDamageDrawPos.y);
	ClearLine();
	ConsoleSetCursorPosition(m_EnemyDamageDrawPos.x, m_EnemyDamageDrawPos.y);
	std::cout << text;

	m_EnemyDamageDrawPos.y-- ;

	Int2 pos;
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		pos = *GetInputPosGame();
		break;
	case SELECTEDWIDNOW::CHAT:
		pos = *GetInputPosChat();
	}
	ConsoleSetCursorPosition(pos.x, pos.y);
	m_IsWriting = false;
}
void GUI::ClearEnemyDamage()
{
	Int2 pos = { m_BorderThickness, m_EnemyDrawPos.y - 4 };
	for (size_t i = 0; i < 3; i++)
	{
		ConsoleSetCursorPosition(m_EnemyDamageDrawPos.x, m_EnemyDamageDrawPos.y);
		ClearLine();
		m_EnemyDamageDrawPos.y--;
	}
}
// player info
void GUI::WritePlayerInfo(PlayerInfo playerInfo)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	m_IsWriting = false;
	ClearWindow(4);
	m_IsWriting = true;

	m_PlayerDrawPos = { m_DimensionsW3.x + 3, m_DimensionsW1.y + 1 };

	// print description of data
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "name       :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "Level      :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "SkillPoints:";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "health     :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "HealthRegen:";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "Damage     :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "AtackSpeed :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "CritChance :";
	m_PlayerDrawPos.y += 1;
	ConsoleSetCursorPosition(m_PlayerDrawPos);
	cout << "CritDamage :";

	m_PlayerDrawPos.y -= 8;
	m_PlayerDrawPos.x += 13;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.Name;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.Level;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.SkillPoints;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.Health << " / " << playerInfo.MaxHealth;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.HealthRegen;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.AtackDamage;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.AtackSpeed;
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.CritChance << "%";
		m_PlayerDrawPos.y += 1;
		ConsoleSetCursorPosition(m_PlayerDrawPos);
		cout << playerInfo.CritDamage *100 << "%";

		m_PlayerDrawPos.y -= 8;
		m_PlayerDrawPos.x += 10;

		m_Frame3Pos = { m_BorderThickness, m_DimensionsW1.y + m_BorderThickness };
		switch (m_SelectedWindow)
		{
		case SELECTEDWIDNOW::GAME:
			ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosGame()->x, GUI::GetInstance()->GetInputPosGame()->y);
			break;
		case SELECTEDWIDNOW::CHAT:
			ConsoleSetCursorPosition(GUI::GetInstance()->GetInputPosChat()->x, GUI::GetInstance()->GetInputPosChat()->y);
			break;
		}
		m_IsWriting = false;
}
// player damage taken
void GUI::WritePlayerDamage(string text)
{
	while (m_IsWriting)
	{
		Sleep(100);
	}
	m_IsWriting = true;

	ConsoleSetCursorPosition(m_PlayerDamageDrawPos.x, m_PlayerDamageDrawPos.y);
	ClearLine();
	ConsoleSetCursorPosition(m_PlayerDamageDrawPos.x, m_PlayerDamageDrawPos.y);
	std::cout << text;

	m_PlayerDamageDrawPos.y--;

	Int2 pos;
	switch (m_SelectedWindow)
	{
	case SELECTEDWIDNOW::GAME:
		pos = *GetInputPosGame();
		break;
	case SELECTEDWIDNOW::CHAT:
		pos = *GetInputPosChat();
	}
	ConsoleSetCursorPosition(pos.x, pos.y);
	m_IsWriting = false;
}
// prev input
void GUI::AddInput(string input)
{
	m_PrevInputs.push_back(input);
	if (m_PrevInputs.size() >= unsigned(m_MaxPrevInputs - 1))
	{
		m_PrevInputs.erase(m_PrevInputs.end());
	}

}