#pragma once
#include <windows.h>
#include "Singleton.h"
#include <vector>
#include <map>

class Room;
class Enemy;

class GUI : public Singleton<GUI>
{
public:
	GUI();
	~GUI();

	// set console cursor on given position
	void ConsoleSetCursorPosition(int column, int row);
	void ConsoleSetCursorPosition(Int2 pos);

	Int2 GetFramePos(int frame);
	Int2 GetFrameDimensions(int frame);

	void ClearWindow(int window);
	void ClearLine(int window = 1, int lenght = 0);

	void GUI::SetConsoleScreenSize();

	void ClearInputChat();
	void ClearInputGame();
	void ResetDrawPosition();
	void GUI::WriteEnemyDamage(string message);
	// only works on window 1 !!
	void LeaveOpenLine() { m_Frame1Pos.y++; }
	// reserving only works in window 1
	void WrOnReservedSpot(string text, int SpotId , bool Permanent = false);
	// reserving only works in window 1
	void ReserveWrSpot(int SpotId);
	void WrPlayerTurn(string text);
	// write finction
	void Wr(std::string text, int window = 1, int row = 0);
	// enemy info
	void WriteEnemyInfo(std::vector<Enemy*> enemysInRoom);
	void ClearEnemyDamage();
	// player info
	void WritePlayerInfo(PlayerInfo playerInfo);
	void WritePlayerDamage(string text);
	// get input positions
	Int2* GetInputPosGame() { return &m_InputPosGame; }
	Int2* GetInputPosChat() { return &m_InputPosChat; }
	Int2 GetDimensionsScreen() { return m_DimensionsScreen; }
	SELECTEDWIDNOW GetSelectedWindow() { return m_SelectedWindow; }
	void SetSelectedWindow(SELECTEDWIDNOW selectedWindow) { m_SelectedWindow = selectedWindow; }

	// prev input
	void AddInput(string input);
private:
	void DrawFrame();
	void GetScreenDimension();
	void GetWidnowSize();
	void PrintChat(std::string text);
private:
	// **************
	//			 WINDOW
	//   **************************
	//   *				*		  *
	//   *				*		  *
	//   *		1		*	2	  *
	//   *				*		  *
	//	 *				*		  *
	//   **************************
	//   *				*		  *
	//   *		3		*	4	  *
	//   *				*		  *
	//   **************************
	bool m_IsWriting = false;
	Int2 m_DimensionsW1 = { 0,0 };
	Int2 m_DimensionsW2 = { 0,0 };
	Int2 m_DimensionsW3 = { 0,0 };
	Int2 m_DimensionsW4 = { 0,0 };

	SELECTEDWIDNOW m_SelectedWindow = SELECTEDWIDNOW::CHAT;
	std::map<int, int> m_ReservedSpot;
	HANDLE m_ConsoleHandle = NULL;

	vector<string> m_ChatBuffer;

	int m_BorderThickness = 1;

	Int2 m_DimensionsScreen;

	int m_TextRatioX = 7; // /10
	int m_TextRatioY = 9; // /10

	Int2 m_InputPosGame;
	Int2 m_InputPosChat;

	Int2 m_PlayerDrawPos;
	Int2 m_EnemyDrawPos;
	Int2 m_EnemyDamageDrawPos;
	Int2 m_PlayerDamageDrawPos;

	Int2 m_Frame1Pos;
	Int2 m_Frame2Pos;
	Int2 m_Frame3Pos;
	Int2 m_Frame4Pos;

	vector<string> m_ExtraText;
	vector<string> m_PrevInputs; // max of 10;
	int m_MaxPrevInputs = 10;

};

