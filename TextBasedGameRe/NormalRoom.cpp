#include "stdafx.h"
#include "NormalRoom.h"
#include "GUI.h"
#include "RoomDAO.h"
#include <iostream>

NormalRoom::NormalRoom(int roomNumber, RoomDAO* roomDAO) :Room(roomNumber , roomDAO)
{
}


NormalRoom::~NormalRoom()
{
}
//enum exitDirections { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };

void NormalRoom::Draw()
{
	GUI::GetInstance()->ConsoleSetCursorPosition(m_pRoomDao->GetPrevRoomDrawPos());
	std::cout << "O";
	Int2 pos = m_pRoomDao->GetDrawPos();
	GUI::GetInstance()->ConsoleSetCursorPosition(pos.x , pos.y);
	std::cout << "X";
	if (m_ExitsArr[0] != 0) //NORTH
	{
		GUI::GetInstance()->ConsoleSetCursorPosition(pos.x , pos.y-1);
		std::cout << "|";
	}
	if (m_ExitsArr[2] != 0) //EAST
	{
		GUI::GetInstance()->ConsoleSetCursorPosition(pos.x+1 , pos.y );
		std::cout << "--";
	}
	if (m_ExitsArr[1] != 0) //SOUTH
	{
		GUI::GetInstance()->ConsoleSetCursorPosition(pos.x , pos.y+1 );
		std::cout << "|";
	}
	if (m_ExitsArr[3] != 0) // WEST
	{
		GUI::GetInstance()->ConsoleSetCursorPosition(pos.x-2, pos.y );
		std::cout << "--";
	}
}