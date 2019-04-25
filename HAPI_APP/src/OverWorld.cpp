#include  "Overworld.h"
#include "Textures.h"
#include "GameEventMessenger.h"


//TODO: Will change
std::vector<EntityProperties> assignEntities(FactionName name)
{
	std::vector<EntityProperties> entities;
	for (int i = 0; i < 20; i++)
	{
		EntityProperties newEntity(name, (EntityType)(rand()%4));

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
	
	GameEventMessenger::getInstance().subscribe(std::bind(&OverWorld::onReset, this), "OverWorld", GameEvent::eResetBattle);
	//m_players.emplace_back(FactionName::eYellow);
	//m_players.emplace_back(FactionName::eBlue);
	//m_players.emplace_back(FactionName::eGreen);
	m_players.emplace_back(FactionName::eRed);
	
	
	m_GUI.reset(m_players[m_currentPlayer].m_entities);
}

OverWorld::~OverWorld()
{
	GameEventMessenger::getInstance().unsubscribe("OverWorld", GameEvent::eResetBattle);
}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{

	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
	
		bool selectNextPlayer = false;
		bool resetPlayer = false;
		m_GUI.onLeftClick(mouseData, m_players[m_currentPlayer], selectNextPlayer, resetPlayer);
		if (selectNextPlayer && m_currentPlayer <= static_cast<int>(m_players.size()) - 1)
		{
			++m_currentPlayer;

			if (m_currentPlayer <= static_cast<int>(m_players.size()) - 1)
			{
				m_GUI.reset(m_players[m_currentPlayer].m_entities);
			}
		}
		if (resetPlayer)
		{
			m_currentPlayer = 0;
			m_GUI.setShipSelectionTrigger(false);
			m_GUI.reset(m_players[m_currentPlayer].m_entities);
			return;
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
	m_GUI.reset(m_players[m_currentPlayer].m_entities);
	m_GUI.getM_Player(m_players);
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
		m_battle.update(deltaTime);
	}
	
}

void OverWorld::startBattle()
{
	if (m_startBattle)
	{
		m_GUI.setShipSelectionTrigger(false);
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
		
		m_battle.startBattle(m_GUI.getSelectedMap(), playersInBattle);
		
		for (auto& player : m_players)
		{
			player.m_selectedEntities.clear();
		}

		m_startBattle = false;
	}
}


void OverWorld::onReset()
{
	m_currentPlayer = 0;
	m_selectNextPlayer = false;
	m_players.clear();
	m_players.emplace_back(FactionName::eYellow);
	//m_players.emplace_back(FactionName::eBlue);
	//m_players.emplace_back(FactionName::eGreen);
	//m_players.emplace_back(FactionName::eRed);
	m_GUI.reset(m_players[m_currentPlayer].m_entities);
}
