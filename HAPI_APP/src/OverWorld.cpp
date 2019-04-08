#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

std::vector<EntityProperties> assignEntities()
{
	//Large movement size because of the fact its easier 
	//to test with developing movement
	std::vector<EntityProperties> entities;
	for (int i = 0; i < 20; i++)
	{
		EntityProperties newEntity;
		newEntity.m_movementPoints = 15;
		newEntity.m_healthMax = i + 1;
		newEntity.m_currentHealth = i + 2;
		newEntity.m_range = i + 3;
		newEntity.m_damage = 1;

		entities.push_back(newEntity);
	}
	assert(!entities.empty());
	return entities;
}

OverWorld::OverWorld()
	: m_entities(assignEntities()),
	m_GUI(m_entities),
	m_battle()
{}

void OverWorld::render()
{
	m_GUI.render(m_battle);

	if (CURRENT_WINDOW == OverWorldWindow::Battle)
	{
		m_battle.render();
	}
}

void OverWorld::update(float deltaTime)
{
	if (OverWorldGUI::CURRENT_WINDOW == OverWorldWindow::eBattle)
	{
		m_battle.update(deltaTime);
	}
}


