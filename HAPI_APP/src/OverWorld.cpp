#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorld::OverWorld()
	: m_overWorldGUI(std::make_unique<OverWorldGUI>(*this)),
	m_battle()
{
	//Large movement size because of the fact its easier 
	//to test with developing movement
	for (int i = 0; i < 20; i++)
	{
		
		//15, i + 1, i + 2, i + 3
		EntityProperties newEntity;
		newEntity.m_movementPoints = 15;
		newEntity.m_healthMax = i + 1;
		newEntity.m_currentHealth = i + 2;
		newEntity.m_range = i + 3;
		newEntity.m_damage = 1;
	
		m_entities.push_back(newEntity);
	}
}

void OverWorld::render()
{
	m_overWorldGUI->render();
}

void OverWorld::update(float deltaTime)
{
	if (OverWorldGUI::CURRENT_WINDOW == OverWorldWindow::eBattle)
	{
		m_battle.update(deltaTime);
	}
}
