#pragma once
#include "Entity.h"
#include "string"
class Npc : public Entity
{
public:
	Npc();
	void update();
	std::vector<std::string> conversation;
};

