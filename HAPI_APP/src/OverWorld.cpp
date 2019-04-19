#include  "Overworld.h"
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
	: m_currentPlayer(0),
	m_selectNextPlayer(false),
	m_players(),
	m_GUI(),
	m_battle(),
	m_startBattle(false)
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
		bool selectNextPlayer = false;
		m_GUI.onLeftClick(mouseData, m_players[m_currentPlayer], selectNextPlayer);
		if (selectNextPlayer && m_currentPlayer <= static_cast<int>(m_players.size()) - 1)
		{
			++m_currentPlayer;

			if (m_currentPlayer <= static_cast<int>(m_players.size()) - 1)
			{
				m_GUI.reset(m_players[m_currentPlayer].m_entities);
			}
		}
		if (m_currentPlayer == static_cast<int>(m_players.size()))
		{
			m_startBattle = true;
			m_currentPlayer = 0;
			return;
		}
		
	}
	if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		m_GUI.onRightClick(mouseData, m_players[m_currentPlayer]);
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	m_GUI.onMouseMove(mouseData, m_players[m_currentPlayer]);
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
		
		std::vector<std::pair<FactionName, std::vector<EntityProperties*>>> playersInBattle;
		for (auto& player : m_players)
		{
			std::pair<FactionName, std::vector<EntityProperties*>> p;
			p.first = player.m_factionName;
			p.second = player.m_selectedEntities;
			playersInBattle.push_back(p);
		}
		m_GUI.clear();
		m_battle = std::make_unique<Battle>(playersInBattle);
		
		for (auto& player : m_players)
		{
			player.m_selectedEntities.clear();
		}

		m_startBattle = false;
	}
}