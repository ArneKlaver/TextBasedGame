
#include "stdafx.h"
#include "Client.h"


#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include  "Ws2tcpip.h"
#include "GUI.h"
#include "ClientManager.h"
#include "binn.h"

Client::Client(std::string ip, int port)
{	
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		GUI::GetInstance()->Wr("Winsock startup failed", 3);
	}

	struct addrinfo *res;
	struct in_addr addr;
	getaddrinfo(ip.c_str(), NULL, 0, &res);
	addr.S_un = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un;
	m_Addr.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
	//m_Addr.sin_addr.s_addr = inet_addr(ip.c_str()); //inet_pton(AF_INET, ip.c_str(), &NetworkIp); //inet_addr(ip.c_str()); //Address = localhost (this pc)
	m_Addr.sin_port = htons(port); //Port = 1111
	m_Addr.sin_family = AF_INET; //IPv4 Socket

	m_pClient = this;


}
Client::~Client()
{

}


bool Client::Connect()
{
	m_Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(m_Connection, (SOCKADDR*)&m_Addr, m_SizeofAddr) != 0) //If we are unable to connect...
	{
		GUI::GetInstance()->Wr("Failed To Connect", 3);
		//exit(0); //Failed to Connect
		return false;
	}
	GUI::GetInstance()->Wr("Connected!\t", 3);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::SendAll(char * data, int totalBytes)
{
	int bytesSend = 0;
	while (bytesSend < totalBytes)
	{
		int RetnCheck = send(m_Connection, data + bytesSend, totalBytes - bytesSend, 0);
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesSend += RetnCheck;
	}
	return true;
}

bool Client::RecvAll(char * data, int totalBytes)
{
	int bytesRecieved = 0;
	while (bytesRecieved < totalBytes)
	{
		int RetnCheck = recv(m_Connection, data + bytesRecieved, totalBytes - bytesRecieved, 0);
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesRecieved += RetnCheck;
	}
	return true;
}

bool Client::SendInt32(int32_t value)
{
	value = htonl(value);
	if (!SendAll((char*)&value, sizeof(int32_t)))
		return false;
	return true;
}
bool Client::GetInt32(int32_t &value)
{
	if (!RecvAll((char*)&value, sizeof(int32_t)))
		return false;
	value = ntohl(value);
	return true;
}

bool Client::SendPacketType(Packet value)
{
	if (!SendInt32(value))
		return false;
	return true;
}
bool Client::GetPacketType(Packet& value)
{
	int packetType_int;
	if (!GetInt32(packetType_int))//Try to receive PacketType type... If PacketType type fails to be recv'd
		return false; //Return false: PacketType type not successfully received
	value = (Packet)packetType_int;
	return true;//Return true if we were successful in retrieving the PacketType type
}
bool Client::SendNameChange(string name)
{
	if (!SendPacketType(P_ChangeName))
		return false;
	int32_t bufferLength = name.size();
	if (!SendInt32(bufferLength))
		return false;
	if (!SendAll((char*)name.c_str(), bufferLength))
		return false;
	return true;
}
bool Client::SendChatMessage(std::string &string)
{
	if (!SendPacketType(P_ChatMessage))
		return false;
	int32_t bufferLength = string.size();
	if (!SendInt32(bufferLength))
		return false;
	if (!SendAll((char*)string.c_str(), bufferLength))
		return false;
	return true;
}
bool Client::GetString(std::string &string)
{
	int bufferLength;
	if (!GetInt32(bufferLength))
		return false;
	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';
	if (!RecvAll(buffer, bufferLength))
	{
		delete[]buffer;
		return false;
	}
	string = buffer;
	delete[] buffer;
	return true;
}

bool Client::CloseConnection()
{
	SendPacketType(P_CloseConnection);
	if (closesocket(m_Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK)
			return true;

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMessage.c_str(), "error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
// for recieving packages from server
void Client::ClientThread()
{
	Packet packettype;// give default value
	while (true)
	{
		if (!m_pClient->GetPacketType(packettype)) //If the packet is not properly processed
			break; //break out of our client handler loop
		if (!m_pClient->ProcessPacket(packettype)) //If the packet is not properly processed
			break; //break out of our client handler loop
	}
	GUI::GetInstance()->Wr("Lost Connection to the server.", 3);
	if (m_pClient->CloseConnection())
	{
		GUI::GetInstance()->Wr("Socket to the server was closed successfuly.", 3);
	}
	else
	{
		GUI::GetInstance()->Wr("Socket was not able to be closed.", 3);
	}
}

bool Client::ProcessPacket(Packet packettype)
{
	switch (packettype)
	{
	case P_ChatMessage:
	{
		std::string Message;
		if (!GetString(Message))
			return false;
		GUI::GetInstance()->Wr(Message, 3);
		break;
	}
	case P_JoinGame:
	{
		// get online ID
		int id;
		if (!GetInt32(id)) {
			ClientManager::GetInstance()->SetIsPlayingOnline(false);
			GUI::GetInstance()->WrPlayerTurn("Failed joining the game");
			return false;
		}
		ClientManager::GetInstance()->SetOnlineID(id);

		ClientManager::GetInstance()->SetIsPlayingOnline(true);
		GUI::GetInstance()->Wr("you joined the game id: " + to_string(id));
		break;
	}
	case P_PlayerTurn:
		ClientManager::GetInstance()->SetIsMyTurn(true);
		GUI::GetInstance()->WrPlayerTurn("Your turn");
		break;
	case P_EnemyTurn:
		ClientManager::GetInstance()->RecvEnemyTurn();
		break;
	case P_PlayerBattleReport:
		ClientManager::GetInstance()->RecvPlayerBattleReport();
		break;
	case P_EnemyBattleReport:
		ClientManager::GetInstance()->RecvEnemyBattleReport();
		break;
	case P_FloorData:
		ClientManager::GetInstance()->RecvFloorData();
		break;
	case P_FloorDataRequest:
		ClientManager::GetInstance()->SendFloorData();
		break;
	case P_NextFloor:
		ClientManager::GetInstance()->RecvNextFloor();
		break;
	default:
		GUI::GetInstance()->Wr("Unknown packet: " + to_string(packettype), 3);
		break;
	}
	return true;
}
// sendGameInfo
bool Client::SendGameData(Packet packetType )
{
	switch (packetType)
	{
	case P_GameMessage:
		break;
	case P_JoinGame:
		if (!SendPacketType(P_JoinGame))
			return false;
		ClientManager::GetInstance()->SetIsPlayingOnline(true);
		GUI::GetInstance()->WrPlayerTurn("Waiting for other players " );
		break;
	case P_LeaveGame: 
		if (!SendPacketType(P_LeaveGame))
			return false;
		ClientManager::GetInstance()->SetIsPlayingOnline(false);
		GUI::GetInstance()->WrPlayerTurn("Left the game server ");
		break;
	case P_PlayerInfo:
		break;
	case P_EnemyTurn:
		ClientManager::GetInstance()->SendEnemyBattleReport();
		break;
	case P_PlayerTurn:
		if (!SendPacketType(P_PlayerTurn))
			return false;
		ClientManager::GetInstance()->SetIsMyTurn(false);
		GUI::GetInstance()->WrPlayerTurn("Waiting for other players");
		break;
	case P_PlayerMoveToRoom:
		if (!SendPacketType(P_PlayerMoveToRoom))
			return false;
		break;
	case P_FloorData:
		ClientManager::GetInstance()->SendFloorData();
		break;
	default: break;
	}
	return true;
}


bool Client::SendBinn(Packet packettype , binn* obj)
{
	if (!SendPacketType(packettype))
		return false;
	void* voidPtr = binn_ptr(obj);
	char* ptr = (char*)voidPtr;
	int32_t bufferLength = binn_size(obj);
	if (!SendInt32(bufferLength))
		return false;
	
	int RetnCheck = send(m_Connection, ptr, binn_size(obj), 0);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}
bool Client::GetBinn( void*& voidBuff)
{
	int32_t bufferLength;
	if (!GetInt32( bufferLength))
		return false;
	char* buffer = new char[bufferLength];

	if (!RecvAll( buffer, bufferLength))
	{
		delete[] buffer;
		return false;
	}
	voidBuff = (void*)buffer;

	//delete[] buffer;
	return true;

}