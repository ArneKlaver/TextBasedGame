#pragma once

class Item;
class Player;
class ItemManager
{
public:
	ItemManager(Player* player);
	~ItemManager();

private:
	vector<Item*> m_pItems;
	Player* m_pPlayer = nullptr;
};

