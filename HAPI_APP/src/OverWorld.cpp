#include  "Overworld.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"


//TODO: Will change
std::vector<EntityProperties> assignEntities(FactionName name)
{
	std::vector<EntityProperties> entities;
	for (int i = 0; i < 20; i++)
	{
		EntityProperties newEntity(name);
		newEntity.m_movementPoints = 15;
		newEntity.m_healthMax = 3;
		newEntity.m_currentHealth = 3;
		newEntity.m_range = i + 3;
		newEntity.m_damage = 1;

		entities.push_back(newEntity);
	}
	assert(!entities.empty());
	return entities;
}

Player::Player(FactionName name)
	: m_entities(assignEntities(name)),
	m_selectedEntities(),
	m_factionName(name)
{}

OverWorld::OverWorld()
	: m_totalPlayers(4),
	m_currentPlayer(0),
	m_selectNextPlayer(false),
	m_players(),
	m_GUI(),
	m_battle(),
	m_startBattle(false),
	m_selectedNextPlayer(false),
	m_currentFactionSelected(FactionName::Yellow)
{
	m_players.emplace_back(FactionName::Blue);
	m_players.emplace_back(FactionName::Red);
	m_players.emplace_back(FactionName::Green);
	m_players.emplace_back(FactionName::Yellow);

	m_GUI.reset(m_players[m_currentPlayer].m_entities);
}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_GUI.onLeftClick(mouseData, m_players[m_currentPlayer], m_selectNextPlayer);		
	}
	if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		if (m_currentFactionSelected == FactionName::Yellow)
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
	if (m_currentFactionSelected == FactionName::Yellow)
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
		assert(m_currentFactionSelected == FactionName::Yellow);
		m_currentFactionSelected = FactionName::Blue;
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
	if (m_currentFactionSelected == FactionName::Yellow)
	{
		//m_GUI.reset(m_player2);
		m_currentFactionSelected = FactionName::Blue;
	}

	if (m_startBattle)
	{
		OverWorldGUI::CURRENT_WINDOW = eBattle;
		
		m_battle = std::make_unique<Battle>(m_player1.m_selectedEntities, m_player2.m_selectedEntities);
		m_startBattle = false;
	}
}