#include "stdafx.h"
#include "ItemManager.h"
#include "Item.h"

ItemManager::ItemManager(Player* player):m_pPlayer(player)
{
}


ItemManager::~ItemManager()
{

	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
}
