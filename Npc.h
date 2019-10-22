#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "string"
class Npc : public Entity
{
public:
	Npc();
	std::string name = "";
	void update();
	std::vector<std::string> conversation;
	std::vector<std::string> endConversation;
	Enemy stand;
};

