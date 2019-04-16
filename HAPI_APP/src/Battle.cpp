#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;

Battle::Battle(std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2) 
	: m_player1(),
	m_player2(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_currentPlayerTurn(PlayerName::Player1),
	m_battleUI(*this, player1, player2)
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
	//Will simplify at some point
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
	assert(m_currentPhase == BattlePhase::Movement);
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_battleProperties.m_movementPathSize);
}

void Battle::activateEntityWeapon(EntityBattleProperties& battleProperties)
{
	assert(m_currentPhase == BattlePhase::Attack);
	battleProperties.m_readyToFire = true;
}

void Battle::fireEntityWeaponAtPosition(const BattleEntity& player, BattleEntity& enemy, const std::vector<const Tile*>& targetArea)
{
	assert(m_currentPhase == BattlePhase::Attack);
	assert(player.m_battleProperties.m_readyToFire);

	//auto tileCoordinate = enemy.m_battleProperties.m_currentPosition;
	//auto cIter = std::find_if(targetArea.cbegin(), targetArea.cend(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
	////Enemy within range of weapon
	//if (cIter != targetArea.cend())
	//{
	//	enemy.m_battleProperties.takeDamage(enemy.m_entityProperties, player.m_entityProperties.m_damage);
	//}
}

void Battle::insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, PlayerName playerName)
{
	assert(m_currentPhase == BattlePhase::ShipPlacement);
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
			m_currentPlayerTurn = PlayerName::Player1;
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
			m_currentPlayerTurn = PlayerName::Player1;
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
			m_currentPlayerTurn = PlayerName::Player1;
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