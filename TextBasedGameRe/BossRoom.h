#pragma once
#include "Room.h"

enum tileTypes { OPEN, PLAYER, ENEMY, TRAP, BLOCKED };
class Player;

class BossRoom: public Room
{
public:
	BossRoom(int roomNumber, RoomDAO* roomDAO);
	~BossRoom();
	
	void SetTilles();
	void UpdateRoom();
	void DrawRoom();
	void Update();
	std::vector< std::vector<int> >& GetRoomLayout() { return (m_RoomLayout); };
	Int2 GetStartPos() { return m_PlayerStartPos; };
	void SetStartPos(Int2 pos) { m_PlayerStartPos = pos; };

private:
	Player* m_pPlayer = nullptr;
	Int2 m_PlayerStartPos = Int2{1,1};
	Int2 RoomSize = Int2{ 8,8 };
	Int2 m_LastPlayerPos = m_PlayerStartPos;
	std::vector<Int2> m_LastEnemyPosArr;

	std::vector< std::vector<int> > m_RoomLayout
	{
		{ BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN, BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN ,BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN ,BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN ,BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN ,BLOCKED } ,
		{ BLOCKED,OPEN,OPEN,OPEN,OPEN,OPEN,OPEN ,BLOCKED } ,
		{ BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED,BLOCKED }
	};
	
};

