#pragma once
#include <string>
#include "Singleton.h"
#include "binn.h"

enum Packet
{
	P_ChatMessage,
	P_GameMessage,
	P_JoinGame,
	P_LeaveGame,
	P_PlayerInfo,
	P_EnemyTurn,
	P_PlayerTurn,
	P_CloseConnection,
	P_PlayerBattleReport,
	P_EnemyBattleReport,
	P_PlayerMoveToRoom,
	P_FloorData,
	P_FloorDataRequest,
	P_ChangeName,
	P_NextFloor
};
class Client
{
public:
	Client(std::string ip, int port);
	~Client();

	bool Connect();
	bool SendChatMessage(std::string &string);

	bool CloseConnection();
	bool SendGameData(Packet packetType);

	bool GetBinn( void*& voidBuff);
	bool SendBinn(Packet packettype , binn* obj);
	bool SendInt32(int32_t value);
	bool SendPacketType(Packet value);
	bool SendNameChange(string name);
	bool GetInt32(int32_t &value);

private:
	bool ProcessPacket(Packet packettype);
	static void ClientThread();

	bool SendAll(char * data, int totalBytes);
	bool RecvAll(char * data, int totalBytes);


	bool GetPacketType(Packet& value);
	bool GetString(std::string &string);

private:
	SOCKET m_Connection;
	SOCKADDR_IN m_Addr;
	int m_SizeofAddr = sizeof(m_Addr);

};

static Client* m_pClient;