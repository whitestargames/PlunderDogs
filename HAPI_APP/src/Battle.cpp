#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;

Battle::Battle(std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2) 
	: m_player1(),
	m_player2(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this, player1, player2),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_currentPlayerTurn(PlayerName::Player1)
{}

void Battle::render() const
{
	m_map.drawMap();
	
	for (const auto& entity : m_player1)
	{
		entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
	}

	for (const auto& entity : m_player2)
	{
		entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
	}

	m_battleUI.render();
}

void Battle::update(float deltaTime)
{
	if (m_currentPhase == BattlePhase::Movement)
	{
		if (m_currentPlayerTurn == PlayerName::Player1)
		{
			bool allEntitiesReachedDestination = true;
			for (auto& entity : m_player1)
			{
				entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties);
				if (!entity->m_battleProperties.m_movedToDestination)
				{
					allEntitiesReachedDestination = false;
				}
			}

			if (allEntitiesReachedDestination)
			{
				for (auto& entity : m_player1)
				{
					entity->m_battleProperties.m_movedToDestination = false;
				}
				nextTurn();
			}
		}
		else
		{
			bool allEntitiesReachedDestination = true;
			for (auto& entity : m_player2)
			{
				entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties);
				if (!entity->m_battleProperties.m_movedToDestination)
				{
					allEntitiesReachedDestination = false;
				}
			}

			if (allEntitiesReachedDestination)
			{
				for (auto& entity : m_player2)
				{
					entity->m_battleProperties.m_movedToDestination = false;
				}
				nextTurn();
			}
		}
	}
}

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination)
{
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_battleProperties.m_movementPathSize);
}

void Battle::activateEntityWeapon(EntityBattleProperties& battleProperties)
{
	battleProperties.m_readyToFire = true;
}

void Battle::fireEntityWeaponAtPosition(std::pair<int, int> coord, const std::vector<const Tile*>& targetArea, BattleEntity& battleEntity)
{

}

void Battle::insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, PlayerName playerName)
{
	switch (playerName)
	{
	case PlayerName::Player1 :
	{
		m_player1.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, playerName));
		break;
	}
	case PlayerName::Player2:
	{
		m_player2.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, playerName));
		break;
	}
	}
}

void Battle::nextTurn()
{
	switch (m_currentPhase)
	{
	case BattlePhase::ShipPlacement :
	{
		if (m_currentPlayerTurn == PlayerName::Player1)
		{
			m_currentPlayerTurn = PlayerName::Player2;
			m_battleUI.newTurn(m_currentPlayerTurn);
		}
		else
		{
			m_currentPhase = BattlePhase::Movement;
			m_battleUI.newPhase();
		}
		
		break;
	}
	case BattlePhase::Movement :
	{
		if (m_currentPlayerTurn == PlayerName::Player1)
		{
			m_currentPlayerTurn = PlayerName::Player2;
			m_battleUI.newTurn(m_currentPlayerTurn);
		}
		else
		{
			m_currentPhase = BattlePhase::Attack;
			m_battleUI.newPhase();
		}
		
		break;
	}
	case BattlePhase::Attack :
	{
		if (m_currentPlayerTurn == PlayerName::Player1)
		{
			m_currentPlayerTurn = PlayerName::Player2;
			m_battleUI.newTurn(m_currentPlayerTurn);
		}
		else
		{
			m_currentPhase = BattlePhase::Movement;
			m_battleUI.newPhase();
		}
		break;
	}
	}
}

const Map & Battle::getMap() const
{
	return m_map;
}

BattlePhase Battle::getCurrentPhase() const
{
	return m_currentPhase;
}

PlayerName Battle::getCurentPlayer() const
{
	return m_currentPlayerTurn;
}