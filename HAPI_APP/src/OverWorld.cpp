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
	m_battle(),
	m_startBattle(false)
{}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_GUI.onLeftClick(mouseData, m_entities, m_selectedEntities, m_startBattle);
	}
	if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		m_GUI.onRightClick(mouseData, m_entities, m_selectedEntities);
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	m_GUI.onMouseMove(mouseData, m_entities, m_selectedEntities);
}

void OverWorld::render()
{
	m_GUI.render(m_battle);
}

void OverWorld::update(float deltaTime)
{
	if (m_startBattle)
	{
		startBattle();
	}

	if (OverWorldGUI::CURRENT_WINDOW == eBattle)
	{
		
		assert(m_battle.get());
		m_battle->update(deltaTime);
	}
}

void OverWorld::startBattle()
{
	if (m_startBattle)
	{
		OverWorldGUI::CURRENT_WINDOW = eBattle;
		m_battle = std::make_unique<Battle>(m_selectedEntities);
		m_startBattle = false;
	}
}


