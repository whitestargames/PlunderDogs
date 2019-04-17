#include  "Overworld.h"
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
	: m_selectedEntities(),
	m_player1(assignEntities(), PlayerName::Player1),
	m_player2(assignEntities(), PlayerName::Player2),
	m_GUI(m_player1),
	m_battle(),
	m_startBattle(false),
	m_selectedNextPlayer(false),
	m_currentPlayerSelected(PlayerName::Player1)
{}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		if (m_currentPlayerSelected == PlayerName::Player1)
		{
			m_GUI.onLeftClick(mouseData, m_player1, m_startBattle, m_selectedNextPlayer, m_player2);
		}
		else
		{
			m_GUI.onLeftClick(mouseData, m_player2, m_startBattle, m_selectedNextPlayer, m_player2);
		}
		
	}
	if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		if (m_currentPlayerSelected == PlayerName::Player1)
		{
			m_GUI.onRightClick(mouseData, m_player1);
		}
		else
		{
			m_GUI.onRightClick(mouseData, m_player2);
		}
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (m_currentPlayerSelected == PlayerName::Player1)
	{
		m_GUI.onMouseMove(mouseData, m_player1);
	}
	else
	{
		m_GUI.onMouseMove(mouseData, m_player2);
	}
}

void OverWorld::render()
{
	m_GUI.render(m_battle);
}

void OverWorld::update(float deltaTime)
{
	if (m_selectedNextPlayer)
	{
		assert(m_currentPlayerSelected == PlayerName::Player1);
		m_currentPlayerSelected = PlayerName::Player2;
		m_selectedNextPlayer = false;
	}

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
	if (m_currentPlayerSelected == PlayerName::Player1)
	{
		//m_GUI.reset(m_player2);
		m_currentPlayerSelected = PlayerName::Player2;
	}

	if (m_startBattle)
	{
		OverWorldGUI::CURRENT_WINDOW = eBattle;
		
		m_battle = std::make_unique<Battle>(m_player1.m_selectedEntities, m_player2.m_selectedEntities);
		m_startBattle = false;
	}
}