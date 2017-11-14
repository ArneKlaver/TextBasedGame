#pragma once
#include <string>
#include <vector>

class Client;
class Player;
class Input
{
public:
	Input(Client* client , Player* player);
	~Input();
	vector<std::string>* GetGameWords() { return &m_GameWords; }
	vector<std::string>* GetChatWords() { return &m_ChatWords; }

	bool HasGameInput() { return m_HasGameInput; };
	bool HasChatInput() { return m_HasChatInput; };

	void ResetHasGameInput() { m_HasGameInput = false; }
	void ResetHasChatInput() { m_HasChatInput = false; }

private:
	POINT Input::GetMousePos();
	static void GetInput(void* Param);
	void FilterStringToWords(string enteredString , vector<std::string>& words);

private:
	Player* m_pPlayer = nullptr;
	HANDLE m_ConsoleHandle = NULL;
	std::string m_InputGame;
	std::string m_InputChat;
	vector<std::string> m_GameWords;
	vector<std::string> m_ChatWords;
	std::vector<std::string> m_Commands;
	Client* m_pClient = nullptr;
	bool m_HasGameInput = false;
	bool m_HasChatInput = false;
};

